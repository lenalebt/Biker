#ifndef OSMDATABASE_HPP
#define OSMDATABASE_HPP

#include <QList>
#include "src/DataPrimitives/DataPrimitives.hpp"
#include "src/DataPrimitives/osmnode.hpp"
#include "src/DataPrimitives/osmedge.hpp"
#include "src/DataPrimitives/osmway.hpp"
#include "src/DataPrimitives/osmpropertytree.hpp"
#include "src/Database/srtm/srtm.h"


class OSMDatabaseReader
{
public:
    SrtmDownloader* downloader;
    /**
     * Sucht aus dem Datenbestand alle Nodes heraus, die sich in einem gewissen
     * Abstand um einen Mittelpunkt befinden, und die gewisse Eigenschaften haben.
     * @param searchMidPoint
     * @param radius Der Radius, in dem Punkte gefunden werden sollen. Er ist mehr als
     * 		ungefähre Angabe zu verstehen: Möglicherweise werden mehr Punkte herausgesucht, als
     * 		sich in diesem Radius befinden (etwa, weil immer im Sinne der schnellen Suche
     * 		alle Punkte in einem Rechteck herausgesucht werden
     * @param props Eigenschaften der Knoten. Hier sollte normalerweise ein PropertyBinaryTree
     * 		drinstehen, aber evtl. steht auch nur eine einzelne Eigenschaft darin.
     * @return
     */
    virtual QList<OSMNode*> getNodes(const GPSPosition& searchMidPoint, double radius, OSMPropertyTree& props)=0;
    /**
     * Sucht aus dem Datenbestand alle Nodes innerhalb eines gegebenen Polygons heraus,
     * die gewissen Eigenschaften genügen.
     * @param searchPolygon
     * @param props
     * @return
     */
    //virtual QList<OSMNode*> getNodes(const Polygon& searchPolygon, OSMPropertyTree& props)=0;
    virtual OSMNode* getNode(ID_Datatype id)=0;

    virtual QList<OSMEdge*> getEdges(const OSMNode& startNode)=0;
    virtual QList<OSMEdge*> getEdges(const OSMNode& startNode, OSMPropertyTree& props)=0;

    virtual QList<OSMWay*> getWays(const GPSPosition& searchMidPoint, double radius, OSMPropertyTree& props)=0;

    virtual void openDatabase(std::string filename)=0;
    virtual bool isOpen()=0;
    virtual void closeDatabase()=0;

    virtual float getAltitude(float lon, float lat);

    OSMDatabaseReader();
    OSMDatabaseReader(QString srtmUrl, QString cachedir);
    virtual ~OSMDatabaseReader();
protected:
};

#endif // OSMDATABASE_HPP
