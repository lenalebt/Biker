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
