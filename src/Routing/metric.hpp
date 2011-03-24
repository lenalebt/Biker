#ifndef METRIC_HPP
#define METRIC_HPP

#include "src/DataPrimitives/DataPrimitives.hpp"
#include "src/DataPrimitives/osmpropertytree.hpp"
#include "src/Database/osmdatabase.hpp"
#include <boost/shared_ptr.hpp>

/**
 * In dieser Klasse wird die Metrik festgelegt, mit der bestimmt wird
 * welche Kosten einem Weg zugeschlagen werden in Abhängigkeit von
 * den Eigenschaften eines Weges. Interface, von dem geerbt werden sollte.
 */
class RouterMetric
{
protected:
	
public:
	virtual double calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge)=0;
	virtual double calcCost(const OSMNode& startNode, const OSMNode& endNode) {return startNode.calcDistance(endNode);}
	virtual boost::shared_ptr<OSMPropertyTree> getAssociatedPropertyTree()=0;
    virtual ~RouterMetric();
};

class EuclidianMetric : public RouterMetric
{
public:
	double calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge);
	boost::shared_ptr<OSMPropertyTree> getAssociatedPropertyTree();
};
class BikeMetric : public RouterMetric
{
private:
    OSMDatabaseReader* db;
    float altitudePenalty;
public:
    BikeMetric(OSMDatabaseReader* dbReader, float altitudePenalty=30.0) : db(dbReader), altitudePenalty(altitudePenalty) {}
    BikeMetric() : db(0) {}
	double calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge);
	boost::shared_ptr<OSMPropertyTree> getAssociatedPropertyTree();
};
class CarMetric : public RouterMetric
{
public:
	double calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge);
	boost::shared_ptr<OSMPropertyTree> getAssociatedPropertyTree();
};
class FastRoutingMetric : public RouterMetric
{
public:
	double calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge);
	boost::shared_ptr<OSMPropertyTree> getAssociatedPropertyTree();
};

#endif // METRIC_HPP
