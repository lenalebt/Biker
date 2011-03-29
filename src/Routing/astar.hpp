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
