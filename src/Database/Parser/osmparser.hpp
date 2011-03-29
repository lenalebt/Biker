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

#ifndef OSMPARSER_HPP
#define OSMPARSER_HPP

#include "src/DataPrimitives/DataPrimitives.hpp"
#include <QtXml>
#include "src/Database/osmdatabasewriter.hpp"

enum NodeType {NODE, WAY, RELATION, NONE};

template <bool parseNodes, bool parseWays, bool parseRelations>
class OSMParser : public QXmlDefaultHandler
{
private:
    OSMDatabaseWriter& dbWriter;

    NodeType nodeType;

    OSMNode* node;
    OSMWay* way;
    OSMEdge* edge;
    OSMRelation* relation;

    int nodeCount;
    int wayCount;
    int relationCount;

public:
    OSMParser( OSMDatabaseWriter& dbWriter);
    ~OSMParser();
    bool startDocument();
    bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );
    bool startElement ( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts );
    bool endDocument ();
    bool fatalError ( const QXmlParseException & exception );

    bool parse(QString filename);
};


#endif // OSMPARSER_HPP
