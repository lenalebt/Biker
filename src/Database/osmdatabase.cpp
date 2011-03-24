#include "osmdatabase.hpp"
#include "src/Database/Parser/osmparser.hpp"

OSMDatabaseReader::OSMDatabaseReader()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    downloader = new SrtmDownloader("http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/", "./data/");
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
        : dbOpen(false)
{
    
}

OSMInMemoryDatabase::~OSMInMemoryDatabase()
{
    closeDatabase();
}

QList<OSMNode*> OSMInMemoryDatabase::getNodes(const GPSPosition& searchMidPoint, double radius, OSMPropertyTree& /*props*/)
{
    double lBoundX, uBoundX, lBoundY, uBoundY;
	lBoundX = searchMidPoint.calcPositionInDistance(270.0, radius).getLon();
	uBoundX = searchMidPoint.calcPositionInDistance(90.0, radius).getLon();
	lBoundY = searchMidPoint.calcPositionInDistance(180.0, radius).getLat();
	uBoundY = searchMidPoint.calcPositionInDistance(0.0, radius).getLat();
    
    //TODO: RStarVisitor bauen, der alle Knoten aufschreibt
    return QList<OSMNode*>();
}
//QList<Node*> OSMInMemoryDatabase::getNodes(const Polygon& searchPolygon, OSMPropertyTree& props){}
QList<OSMEdge*> OSMInMemoryDatabase::getEdges(const OSMNode& startNode)
{
    return edgeMap.values(startNode.getID());
}
QList<OSMEdge*> OSMInMemoryDatabase::getEdges(const OSMNode& startNode, OSMPropertyTree& props)
{
    QList<OSMEdge*> edgeList = edgeMap.values(startNode.getID());   //TODO: Raussieben!
    QList<OSMEdge*> filteredEdgeList;
    
    for (QList<OSMEdge*>::iterator edgeIt = edgeList.begin(); edgeIt < edgeList.end(); edgeIt++)
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
QList<OSMWay*> OSMInMemoryDatabase::getWays(const GPSPosition& /*searchMidPoint*/, double /*radius*/, OSMPropertyTree& /*props*/){return QList<OSMWay*>();}//TODO
OSMNode* OSMInMemoryDatabase::getNode(ID_Datatype id)
{
    return nodeMap.value(id);
}

void OSMInMemoryDatabase::openDatabase(QString filename)
{
    OSMParser<true, true, false> parser(*this);
    parser.parse(filename);
}//TODO
bool OSMInMemoryDatabase::isOpen() {return dbOpen;}
void OSMInMemoryDatabase::closeDatabase()
{
    //Speicher der Nodes freigeben...
    for (QMapIterator<ID_Datatype, OSMNode*> it(nodeMap); it.hasNext(); )
    {
        it.next();
        delete it.value();
    }
    nodeMap.clear();
    
    //Speicher der Edges freigeben...
    for (QMapIterator<ID_Datatype, OSMEdge*> it(edgeMap); it.hasNext(); )
    {
        it.next();
        delete it.value();
    }
    edgeMap.clear();
}


void OSMInMemoryDatabase::addEdge(OSMEdge* edge)
{
    edgeMap.insert(edge->getStartNodeID(), edge);
}
void OSMInMemoryDatabase::addNode(OSMNode* node)
{
    //nodeTree.Insert(node, bounds<float>(node->getLon(), node->getLat(), node->getLon(), node->getLat()));
    nodeMap.insert(node->getID(), node);
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
