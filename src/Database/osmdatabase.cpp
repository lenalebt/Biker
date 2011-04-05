/*
    Biker wants to be a routing software, intended to be useful for planning bike tours.
    Copyright (C) 2011  Lena Brueder

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/ 

#include "osmdatabase.hpp"
#include "src/Database/Parser/osmparser.hpp"
#include "src/Database/Parser/pbfparser.hpp"

OSMDatabaseReader::OSMDatabaseReader()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
#ifdef WINDOWS
    //TODO: Pfade anpassen!
    downloader = new SrtmDownloader("http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/", QDir::homePath() + "/biker/data/srtm/");
#else
    downloader = new SrtmDownloader("http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/", QDir::homePath() + "/.biker/data/srtm/");
#endif
}
OSMDatabaseReader::OSMDatabaseReader(QString srtmUrl, QString cachedir)
{   //TODO: Sicherheitsproblem: an dieser Stelle ist es evtl. möglich, ins Dateisystem einzudringen.
    curl_global_init(CURL_GLOBAL_DEFAULT);
    downloader = new SrtmDownloader(srtmUrl, cachedir);
}
OSMDatabaseReader::~OSMDatabaseReader()
{
    curl_global_cleanup();
    delete downloader;
}

float OSMDatabaseReader::getAltitude(float lon, float lat)
{
    return downloader->getAltitudeFromLatLon(lat, lon);
}



OSMInMemoryDatabase::OSMInMemoryDatabase()
        : dbOpen(false), extendedParsing(false)
{
    closeDatabase();
    curve = new ZOrderCurve();
}

OSMInMemoryDatabase::OSMInMemoryDatabase(SpaceFillingCurve* curve)
        : dbOpen(false), curve(curve), extendedParsing(false)
{
    
}

OSMInMemoryDatabase::~OSMInMemoryDatabase()
{
    delete curve;
}

QList<boost::shared_ptr<OSMNode> > OSMInMemoryDatabase::getNodes(const GPSPosition& searchMidPoint, double radius, OSMPropertyTree& props)
{
    double lBoundX, uBoundX, lBoundY, uBoundY;
	lBoundX = searchMidPoint.calcPositionInDistance(270.0, radius).getLon();
	uBoundX = searchMidPoint.calcPositionInDistance(90.0, radius).getLon();
	lBoundY = searchMidPoint.calcPositionInDistance(180.0, radius).getLat();
	uBoundY = searchMidPoint.calcPositionInDistance(0.0, radius).getLat();
    
    QList<boost::shared_ptr<OSMNode> > nodeList;
    int lBound = curve->getBucketID(lBoundX, lBoundY);
    int uBound = curve->getBucketID(uBoundX, uBoundY);
    for (int i=lBound; i<=uBound; i++)
    {
        QList<boost::shared_ptr<OSMNode> > tmpPointList = nodePlaceMap.values(i);
        for (QList<boost::shared_ptr<OSMNode> >::iterator it = tmpPointList.begin(); it < tmpPointList.end(); it++)
        {
            if (searchMidPoint.calcDistance(**it) < radius)
            {
                QList<OSMProperty> propList = (*it)->getProperties();
                props.resetPropertiesFound();
                for (QList<OSMProperty>::iterator propIt = propList.begin(); propIt < propList.end(); propIt++)
                {
                    props.propertyFound(*propIt);
                }
                if (props.evaluate())
                    nodeList << *it;
            }
        }
    }
    
    return nodeList;
}
//QList<Node*> OSMInMemoryDatabase::getNodes(const Polygon& searchPolygon, OSMPropertyTree& props){}
QList<boost::shared_ptr<OSMEdge> > OSMInMemoryDatabase::getEdges(const OSMNode& startNode)
{
    return edgeMap.values(startNode.getID());
}
QList<boost::shared_ptr<OSMEdge> > OSMInMemoryDatabase::getEdges(const OSMNode& startNode, OSMPropertyTree& props)
{
    QList<boost::shared_ptr<OSMEdge> > edgeList = edgeMap.values(startNode.getID());   //TODO: Raussieben!
    QList<boost::shared_ptr<OSMEdge> > filteredEdgeList;
    
    for (QList<boost::shared_ptr<OSMEdge> >::iterator edgeIt = edgeList.begin(); edgeIt < edgeList.end(); edgeIt++)
    {
        QList<OSMProperty> propList = (*edgeIt)->getProperties();
        props.resetPropertiesFound();
        for (QList<OSMProperty>::iterator propIt = propList.begin(); propIt < propList.end(); propIt++)
        {
            props.propertyFound(*propIt);
            
        }
        if (props.evaluate())
            filteredEdgeList << *edgeIt;
    }
    
    return filteredEdgeList;
}
QList<boost::shared_ptr<OSMWay> > OSMInMemoryDatabase::getWays(const GPSPosition& /*searchMidPoint*/, double /*radius*/, OSMPropertyTree& /*props*/){return QList<boost::shared_ptr<OSMWay> >();}//TODO
boost::shared_ptr<OSMNode>  OSMInMemoryDatabase::getNode(ID_Datatype id)
{
    return nodeMap.value(id);
}

bool OSMInMemoryDatabase::openDatabase(QString filename)
{
    if (filename.endsWith(".osm"))
    {
        OSMParser<true, true, false> parser(*this);
        edgeMap.clear();
        nodeMap.clear();
        nodePlaceMap.clear();
        return (dbOpen=parser.parse(filename));
    }
    else if (filename.endsWith(".pbf"))
    {
        PBFParser parser(*this);
        edgeMap.clear();
        nodeMap.clear();
        nodePlaceMap.clear();
        return (dbOpen=parser.parse(filename));
    }
    else
        return false;
}
bool OSMInMemoryDatabase::addDatabase(QString filename)
{
    if (filename.endsWith(".osm"))
    {
        OSMParser<true, true, false> parser(*this);
        return (dbOpen=parser.parse(filename));
    }
    else if (filename.endsWith(".pbf"))
    {
        PBFParser parser(*this);
        return (dbOpen=parser.parse(filename));
    }
    else
        return false;
}

bool OSMInMemoryDatabase::isOpen() {return dbOpen;}
void OSMInMemoryDatabase::closeDatabase()
{
    //Speicher der Nodes freigeben...
    nodeMap.clear();
    
    //Speicher der Edges freigeben...
    edgeMap.clear();
    
    dbOpen = false;
}


void OSMInMemoryDatabase::addEdge(OSMEdge* edge)
{
    edgeMap.insert(edge->getStartNodeID(), boost::shared_ptr<OSMEdge>(edge));
}
void OSMInMemoryDatabase::addNode(OSMNode* node)
{
    //nodeTree.Insert(node, bounds<float>(node->getLon(), node->getLat(), node->getLon(), node->getLat()));
    boost::shared_ptr<OSMNode> nodePtr(node);
    nodeMap.insert(node->getID(), nodePtr);
    nodePlaceMap.insert(curve->getBucketID(node->getLon(), node->getLat()), nodePtr);
}
void OSMInMemoryDatabase::addEdgeWithID(OSMEdgeWithID* edgeWithID)
{
    delete edgeWithID;
}
void OSMInMemoryDatabase::addWay(OSMWay* way)
{   //TODO: Anpassungen mit Einbahnstraßen!
    bool isOneWay = false;
    QList<OSMProperty> propList = way->getProperties();
    for (QList<OSMProperty>::iterator it = propList.begin(); it < propList.end(); it++)
    {
        if ((it->getKey() == "oneway") && (it->getValue() == "yes"))
        {
            isOneWay = true;
        }
    }

    //Einbahnstraßen werden ignoriert. Sollte später mal dazugemacht werden, solange: ignorieren ;D.
    QList<ID_Datatype> memberList = way->getMemberList();
    ID_Datatype oldID = *(memberList.begin());
    OSMEdge* edge;
    OSMEdgeWithID* edgeWithID;

    for (QList<ID_Datatype>::const_iterator it = memberList.begin()+1; it < memberList.end(); it++)
    {
        //hin
        edge = new OSMEdge(oldID, *it, way->getProperties());
        this->addEdge(edge);

        edgeWithID = new OSMEdgeWithID(way->getID(), oldID, *it, way->getProperties());
        this->addEdgeWithID(edgeWithID);

        if (!isOneWay)
        {
            //und zurück
            edge = new OSMEdge(*it, oldID, way->getProperties());
            this->addEdge(edge);

            edgeWithID = new OSMEdgeWithID(way->getID(), *it, oldID, way->getProperties());
            this->addEdgeWithID(edgeWithID);
        }
        oldID = *it;
    }
    
    /* Extended parsing soll POIs, die als closed Ways gespeichert sind,
     * der Menge Nodes hinzuzufügen. Dadurch wird eine Suche vereinfacht.
     *
     */
    if (extendedParsing)
    {
        if (memberList.size() > 2)
        {   //nur bei closed Ways, die mindestens 3 Elemente haben
            if (memberList[0] == memberList[memberList.size()-1])
            {
                if (way->getProperties().size() != 0)
                {   //closed Ways ohne Eigenschaften sind uninteressant
                    //Gebäude ausschließen, sofern sie keine anderen Eigenschaften haben
                    bool goodClosedWay = false;
                    QList<OSMProperty> propList = way->getProperties();
                    for (QList<OSMProperty>::iterator it = propList.begin(); it < propList.end(); it++)
                    {
                        if ((it->getKey() != "building") && (it->getKey() != "layer") && (it->getKey() != "source"))
                        {
                            goodClosedWay = true;
                            break;
                        }
                    }
                    if (goodClosedWay)
                    {
                        double meanLon=0, meanLat=0;
                        int count = 0;
                        for (QList<ID_Datatype>::iterator it = memberList.begin(); it < memberList.end(); it++)
                        {
                            boost::shared_ptr<OSMNode> node = nodeMap[*it];
                            if (node.get() != 0)
                            {
                                //System.err.println("Node Lon/Lat:" + node.getLon() + "/" + node.getLat());
                                meanLon += node->getLon();
                                meanLat += node->getLat();
                                count++;
                            }
                        }
                        meanLat /= count;
                        meanLon /= count;
                        
                        //neue nodeID erzeugen, die es noch nicht gibt...
                        ID_Datatype nodeID = ((rand() % 32768) << 16) + (rand() % 32768);
                        while ((nodeMap[nodeID].get()) != 0 && (nodeID == 0))
                        {
                            nodeID = ((rand() % 32768) << 16) + (rand() % 32768);
                        }
                        
                        OSMNode* node = new OSMNode(nodeID, GPSPosition(meanLon, meanLat), propList);
                        addNode(node);
                    }
                }
            }
        }
    }
    
    delete way;
}
void OSMInMemoryDatabase::addRelation(OSMRelation* relation)
{
    delete relation;
}

void OSMInMemoryDatabase::finished()
{
    //nichts zu tun.
}

bool OSMInMemoryDatabase::openDatabase(QString filename, bool extendedParsing)
{
    this->extendedParsing = extendedParsing;
    return this->openDatabase(filename);
}
bool OSMInMemoryDatabase::addDatabase(QString filename, bool extendedParsing)
{
    this->extendedParsing = extendedParsing;
    return this->addDatabase(filename);
}
