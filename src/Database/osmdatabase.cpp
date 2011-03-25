#include "osmdatabase.hpp"
#include "src/Database/Parser/osmparser.hpp"

OSMDatabaseReader::OSMDatabaseReader()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    downloader = new SrtmDownloader("http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/", "./data");
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
    closeDatabase();
    curve = new ZOrderCurve();
}

OSMInMemoryDatabase::OSMInMemoryDatabase(SpaceFillingCurve* curve)
        : dbOpen(false), curve(curve)
{
    
}

OSMInMemoryDatabase::~OSMInMemoryDatabase()
{
    delete curve;
}

QList<boost::shared_ptr<OSMNode> > OSMInMemoryDatabase::getNodes(const GPSPosition& searchMidPoint, double radius, OSMPropertyTree& /*props*/)
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
        nodeList << nodePlaceMap.values(i);
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
    OSMParser<true, true, false> parser(*this);
    return (dbOpen=parser.parse(filename));
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
