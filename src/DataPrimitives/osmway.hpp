#ifndef OSMWAY_HPP
#define OSMWAY_HPP

#include "src/DataPrimitives/DataPrimitiveDefines.hpp"
#include "src/DataPrimitives/osmproperty.hpp"
#include <QList>

class OSMWay
{
private:
    ID_Datatype id;
    QList<ID_Datatype> memberIDList;
    QList<OSMProperty> properties;
public:
    OSMWay(ID_Datatype id_, QList<OSMProperty> propList) : id(id_), properties(propList) {};
    OSMWay(ID_Datatype id_, QList<ID_Datatype> memberIDList_, QList<OSMProperty> propList) : id(id_), memberIDList(memberIDList_), properties(propList) {};
    ID_Datatype getID() const {return id;}
    void setID(ID_Datatype id) {this->id = id;}
    QList<ID_Datatype> getMemberList() const {return memberIDList;}
    void addMember(ID_Datatype nodeID) {memberIDList << nodeID;}
    void addProperty(OSMProperty& prop) {properties << prop;}
    QList<OSMProperty> getProperties() const {return properties;}
};

#endif // OSMWAY_HPP
