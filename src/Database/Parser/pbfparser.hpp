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

#ifndef PBFPARSER_HPP
#define PBFPARSER_HPP

#include "src/DataPrimitives/DataPrimitives.hpp"
#include "src/Database/osmdatabasewriter.hpp"
#include "src/Database/Parser/pbf/osmformat.pb.h"

template <bool parseNodes, bool parseWays, bool parseRelations>
class PBFParser
{
private:
    OSMDatabaseWriter& dbWriter;

    OSMNode* node;
    OSMWay* way;
    OSMEdge* edge;
    OSMRelation* relation;

    int nodeCount;
    int wayCount;
    int relationCount;

public:
    PBFParser( OSMDatabaseWriter& dbWriter );
    ~PBFParser();

    bool parse(QString filename);
};


#endif // PBFPARSER_HPP
