#include "osmparser.hpp"
#include <iostream>

template <bool parseNodes, bool parseWays, bool parseRelations>
OSMParser<parseNodes, parseWays, parseRelations>::OSMParser(OSMDatabaseWriter& dbWriter)
    : dbWriter(dbWriter), nodeType(NONE), nodeCount(0), wayCount(0), relationCount(0)
{

}

template <bool parseNodes, bool parseWays, bool parseRelations>
OSMParser<parseNodes, parseWays, parseRelations>::~OSMParser()
{

}

template <bool parseNodes, bool parseWays, bool parseRelations>
bool OSMParser<parseNodes, parseWays, parseRelations>::parse(QString filename)
{
    QFile file(filename);
    QXmlInputSource inputSource(&file);
    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    reader.setErrorHandler(this);
    return reader.parse(inputSource);
}

template <bool parseNodes, bool parseWays, bool parseRelations>
bool OSMParser<parseNodes, parseWays, parseRelations>::fatalError ( const QXmlParseException & exception )
{
    std::cerr << "FatalError" << std::endl;
    std::cerr << exception.lineNumber() << " " << exception.columnNumber() << " " << exception.message().toStdString() << std::endl;
    return false;
}

template <bool parseNodes, bool parseWays, bool parseRelations>
bool OSMParser<parseNodes, parseWays, parseRelations>::startDocument()
{
    nodeType = NONE;
    //std::cerr << "StartDocument" << std::endl;
    return true;
}

template <bool parseNodes, bool parseWays, bool parseRelations>
bool OSMParser<parseNodes, parseWays, parseRelations>::startElement ( const QString & /*namespaceURI*/, const QString & /*localName*/, const QString & qName, const QXmlAttributes & atts )
{
    if (nodeType == NONE)
    {
        if (parseNodes && qName == "node")
        {
            nodeCount++;
            nodeType = NODE;

            ID_Datatype id=0;
            double lon=0.0, lat=0.0;

            for (int i=0; i<atts.length(); i++)
            {
                if (atts.qName(i) == "id")
                    id = atts.value(i).toLong();
                else if (atts.qName(i) == "lon")
                    lon = atts.value(i).toDouble();
                else if (atts.qName(i) == "lat")
                    lat = atts.value(i).toDouble();
            }
            node = new OSMNode(id, GPSPosition(lon, lat), QList<OSMProperty>());

        }
        else if (parseWays && qName == "way")
        {
            wayCount++;
            nodeType = WAY;

            ID_Datatype id=0.0;

            for (int i=0; i<atts.length(); i++)
            {
                if (atts.qName(i) == "id")
                    id = atts.value(i).toLong();
            }
            way = new OSMWay(id, QList<OSMProperty>());
        }
        else if (parseRelations && qName == "relation")
        {
            relationCount++;
            nodeType = RELATION;
        }
    }
    else if (parseNodes && nodeType == NODE)
    {
        if (qName == "tag") //Eigenschaft
        {
            QString key, value;
            for (int i=0; i<atts.length(); i++)
            {
                if (atts.qName(i) == "k")
                    key = atts.value(i);
                else if (atts.qName(i) == "v")
                    value = atts.value(i);
            }
            if (key != "created_by")
                node->addProperty(OSMProperty(key, value));
        }
    }
    else if (parseWays && nodeType == WAY)
    {
        if (qName == "tag") //Eigenschaft
        {
            QString key, value;
            for (int i=0; i<atts.length(); i++)
            {
                if (atts.qName(i) == "k")
                    key = atts.value(i);
                else if (atts.qName(i) == "v")
                    value = atts.value(i);
            }
            if (key != "created_by")
                way->addProperty(OSMProperty(key, value));
        }
        else if (qName == "nd") //Elementknoten
        {
            way->addMember(atts.value("ref").toLong());
        }
    }
    else if (parseRelations && nodeType == RELATION)
    {
        if (qName == "tag") //Eigenschaft
        {

        }
    }

    return true;
}

template <bool parseNodes, bool parseWays, bool parseRelations>
bool OSMParser<parseNodes, parseWays, parseRelations>::endElement ( const QString & /*namespaceURI*/, const QString & /*localName*/, const QString & qName )
{
    if (nodeType == NONE)
    {

    }
    else if (parseNodes && nodeType == NODE)
    {
        if (qName == "node")
        {
            nodeType = NONE;
            dbWriter.addNode(node);
        }
    }
    else if (parseWays && nodeType == WAY)
    {
        if (qName == "way")
        {
            nodeType = NONE;
            dbWriter.addWay(way);
        }
    }
    else if (parseRelations && nodeType == RELATION)
    {
        if (qName == "relation")
        {
            nodeType = NONE;
            dbWriter.addRelation(relation);

            delete relation;
        }
    }

    return true;
}

template <bool parseNodes, bool parseWays, bool parseRelations>
bool OSMParser<parseNodes, parseWays, parseRelations>::endDocument ()
{
    dbWriter.finished();
    std::cerr << "EndDocument. NodeCount: " << nodeCount << " WayCount: " << wayCount
        << " RelationCount: " << relationCount << std::endl;
    return true;
}

template class OSMParser<false, false, false>;
template class OSMParser<true, false, false>;
template class OSMParser<false, true, false>;
template class OSMParser<true, true, false>;
template class OSMParser<true, true, true>;
