#ifndef ASTAR_HPP
#define ASTAR_HPP

#include "src/Routing/routing.hpp"
#include "src/Database/osmdatabase.hpp"

class AStarRoutingNode : public RoutingNode
{
private:
	float heuristicCost;
public:
	AStarRoutingNode(OSMNode parent, ID_Datatype predecessor, float overallCost, float heuristicCost);
	float getHeuristicCost() {return heuristicCost;}
	void setHeuristicCost(float cost) {heuristicCost = cost;}
	bool operator<(const AStarRoutingNode& other) {return (this->overallCost + this->heuristicCost) < (other.overallCost + other.heuristicCost);}
};


class AStar : public Router
{
private:
	Heap<AStarRoutingNode>* openlist;
	ClosedList* closedlist;
	QMap<ID_Datatype, boost::shared_ptr<AStarRoutingNode> > nodeMap;
	GPSRoute calcShortestRoute(AStarRoutingNode startNode, OSMNode endNode);
public:
	AStar(OSMDatabaseReader* db, RouterMetric* metric, Heap<AStarRoutingNode>* ds_, ClosedList* cl_);
	~AStar();
	GPSRoute calcShortestRoute(GPSPosition startPosition, GPSPosition endPosition);
	GPSRoute calcShortestRoute(OSMNode startNode, GPSPosition endPosition);
	GPSRoute calcShortestRoute(GPSPosition startPosition, OSMNode endNode);
    GPSRoute calcShortestRoute(OSMNode startNode, OSMNode endNode);
};

#endif // ASTAR_HPP
