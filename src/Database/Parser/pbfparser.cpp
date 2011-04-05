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

#include "pbfparser.hpp"

template <bool parseNodes, bool parseWays, bool parseRelations>
PBFParser<parseNodes, parseWays, parseRelations>::PBFParser(OSMDatabaseWriter& dbWriter) : dbWriter(dbWriter)
{
    
}

template <bool parseNodes, bool parseWays, bool parseRelations>
PBFParser<parseNodes, parseWays, parseRelations>::~PBFParser()
{
    
}

template <bool parseNodes, bool parseWays, bool parseRelations>
bool PBFParser<parseNodes, parseWays, parseRelations>::parse(QString /*filename*/)
{
    return false;
}




template class PBFParser<false, false, false>;
template class PBFParser<true, false, false>;
template class PBFParser<false, true, false>;
template class PBFParser<true, true, false>;
template class PBFParser<true, true, true>;
