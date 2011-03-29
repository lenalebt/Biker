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
