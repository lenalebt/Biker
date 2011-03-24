#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "src/Routing/routing.hpp"
#include "src/Database/osmdatabase.hpp"

/**
 * Der Algorithmus von Dijkstra als Router. Klassisch und
 * bekannt.
 */
class Dijkstra : public Router
{
private:
	Heap<RoutingNode>* datastructure;
	ClosedList* closedlist;
	QMap<ID_Datatype, boost::shared_ptr<RoutingNode> > nodeMap;
    
    GPSRoute calcShortestRoute(RoutingNode startNode, OSMNode endNode);
public:
	Dijkstra(OSMDatabaseReader* db, RouterMetric* metric, Heap<RoutingNode>* ds_, ClosedList* cl_);
	~Dijkstra();
	GPSRoute calcShortestRoute(GPSPosition startPosition, GPSPosition endPosition);
	GPSRoute calcShortestRoute(OSMNode startNode, GPSPosition endPosition);
	GPSRoute calcShortestRoute(GPSPosition startPosition, OSMNode endNode);
	GPSRoute calcShortestRoute(OSMNode startNode, OSMNode endNode);
};

#endif // DIJKSTRA_HPP
