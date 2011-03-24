#include "osmdatabasewriter.hpp"

OSMDatabaseWriter::OSMDatabaseWriter()
{
}

void OSMDatabaseWriter::addNode(OSMNode* node)
{
    delete node;
}

void OSMDatabaseWriter::addWay(OSMWay* way)
{
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

void OSMDatabaseWriter::addEdge(OSMEdge* edge)
{
    delete edge;
}

void OSMDatabaseWriter::addEdgeWithID(OSMEdgeWithID* edgeWithID)
{
    delete edgeWithID;
}

void OSMDatabaseWriter::addRelation(OSMRelation* relation)
{
    delete relation;
}
