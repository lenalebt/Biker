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
    OSMDatabaseWriter* dbWriter;

    NodeType nodeType;

    OSMNode* node;
    OSMWay* way;
    OSMEdge* edge;

    int nodeCount;
    int edgeCount;
    int wayCount;
    int relationCount;

public:
    OSMParser( OSMDatabaseWriter* dbWriter);
    ~OSMParser();
    bool startDocument();
    bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );
    bool startElement ( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts );
    bool endDocument ();
    bool fatalError ( const QXmlParseException & exception );

    bool parse(QString filename);
};

#endif // OSMPARSER_HPP
