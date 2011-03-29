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
