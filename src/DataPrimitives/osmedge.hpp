#ifndef OSMEDGE_HPP
#define OSMEDGE_HPP

#include "src/DataPrimitives/DataPrimitiveDefines.hpp"
#include "src/DataPrimitives/osmproperty.hpp"
#include <QList>

class OSMEdge
{
protected:
    ID_Datatype startNodeID;
    ID_Datatype endNodeID;
    QList<OSMProperty> properties;

public:
    ID_Datatype getStartNodeID() const {return startNodeID;}
    ID_Datatype getEndNodeID() const {return endNodeID;}
    void setStartNodeID(ID_Datatype id) {startNodeID = id;}
    void setEndNodeID(ID_Datatype id) {endNodeID = id;}

    QList<OSMProperty> getProperties() const {return properties;}
    void addProperty(OSMProperty& prop) {properties << prop;}
    OSMEdge(ID_Datatype startNodeID_, ID_Datatype endNodeID_, QList<OSMProperty> props) :
            startNodeID(startNodeID_), endNodeID(endNodeID_), properties(props) {}
};

class OSMEdgeWithID : public OSMEdge
{
private:
    ID_Datatype id;
public:
    ID_Datatype getID() const {return id;}
    void setID(ID_Datatype id) {this->id = id;}

    OSMEdge getOSMEdge() {return OSMEdge(startNodeID, endNodeID, properties);}

    OSMEdgeWithID(ID_Datatype id_, ID_Datatype startNodeID_, ID_Datatype endNodeID_, QList<OSMProperty> props) :
            OSMEdge(startNodeID_, endNodeID_, props), id(id_) {}
};

#endif // OSMEDGE_HPP
