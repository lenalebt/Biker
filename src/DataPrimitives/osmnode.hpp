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
    ~OSMNode() {}
    QList<OSMProperty> getProperties() const {return properties;}
    void addProperty(OSMProperty prop) {properties << prop;}
    ID_Datatype getID() const {return id;}
    void setID(ID_Datatype id) {this->id = id;}
};

#endif // OSMNODE_HPP
