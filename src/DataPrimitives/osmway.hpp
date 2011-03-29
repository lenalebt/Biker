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
    void addProperty(OSMProperty prop) {properties << prop;}
    QList<OSMProperty> getProperties() const {return properties;}
};

#endif // OSMWAY_HPP
