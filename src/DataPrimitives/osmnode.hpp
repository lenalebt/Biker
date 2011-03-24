#ifndef OSMNODE_HPP
#define OSMNODE_HPP

#include "src/DataPrimitives/DataPrimitiveDefines.hpp"
#include "src/DataPrimitives/gpsposition.hpp"
#include "src/DataPrimitives/osmproperty.hpp"
#include <QList>
#include <iostream>

class OSMNode : public GPSPosition
{
private:
    ID_Datatype id;
    QList<OSMProperty> properties;

public:
    OSMNode() : GPSPosition(), id(0), properties(QList<OSMProperty>()) {}
    OSMNode(ID_Datatype id_, GPSPosition pos, QList<OSMProperty> props) : GPSPosition(pos), id(id_), properties(props) {}
    ~OSMNode() {properties.clear();}
    QList<OSMProperty> getProperties() const {return properties;}
    void addProperty(OSMProperty prop) {properties << prop;}
    ID_Datatype getID() const {return id;}
    void setID(ID_Datatype id) {this->id = id;}
};

#endif // OSMNODE_HPP
