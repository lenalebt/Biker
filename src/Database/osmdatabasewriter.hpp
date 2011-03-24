#ifndef OSMDATABASEWRITER_HPP
#define OSMDATABASEWRITER_HPP

#include "src/DataPrimitives/DataPrimitives.hpp"

class OSMDatabaseWriter
{
public:
    OSMDatabaseWriter();

    virtual void addNode(OSMNode* node);
    virtual void addEdge(OSMEdge* edge);
    virtual void addEdgeWithID(OSMEdgeWithID* edgeWithID);
    virtual void addWay(OSMWay* way);
    virtual void addRelation(OSMRelation* relation);
    virtual void finished()=0;
};

#endif // OSMDATABASEWRITER_HPP
