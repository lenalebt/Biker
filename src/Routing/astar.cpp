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

#include "astar.hpp"
#include "src/Database/osmdatabase.hpp"
#include <QList>
#include <QDebug>


AStar::AStar(OSMDatabaseReader* db, RouterMetric* metric, Heap<AStarRoutingNode>* ds_, ClosedList* cl_) :
		Router(db, metric), openlist(ds_), closedlist(cl_)
{
	
}

AStarRoutingNode::AStarRoutingNode(OSMNode parent, ID_Datatype predecessor_, float overAllCost_, float heuristicCost_) :
		RoutingNode(parent, predecessor_, overAllCost_), heuristicCost(heuristicCost_)
{
	
}

GPSRoute AStar::calcShortestRoute(GPSPosition startPosition, GPSPosition endPosition)
{
	OSMNode startNode = calcStartEndNode(startPosition);
    if (startNode.getID() == 0)
        startNode = calcStartEndNode(startPosition, 200.0);
    if (startNode.getID() == 0)
        startNode = calcStartEndNode(startPosition, 1000.0);
    
	OSMNode endNode = calcStartEndNode(endPosition);
    if (endNode.getID() == 0)
        endNode = calcStartEndNode(endPosition, 200.0);
    if (endNode.getID() == 0)
        endNode = calcStartEndNode(endPosition, 1000.0);
    
	AStarRoutingNode startRoutingNode = AStarRoutingNode(startNode, 0, 0.0, startNode.calcDistance(endNode));
	
	return calcShortestRoute(startRoutingNode, endNode);
}
GPSRoute AStar::calcShortestRoute(OSMNode startNode, GPSPosition endPosition)
{
	OSMNode endNode = calcStartEndNode(endPosition);
    if (endNode.getID() == 0)
        endNode = calcStartEndNode(endPosition, 200.0);
    if (endNode.getID() == 0)
        endNode = calcStartEndNode(endPosition, 1000.0);
    
	AStarRoutingNode startRoutingNode = AStarRoutingNode(startNode, 0, 0.0, metric->calcCost(startNode, endNode));
	
	return calcShortestRoute(startRoutingNode, endNode);
}
GPSRoute AStar::calcShortestRoute(GPSPosition startPosition, OSMNode endNode)
{
	OSMNode startNode = calcStartEndNode(startPosition);
    if (startNode.getID() == 0)
        startNode = calcStartEndNode(startPosition, 200.0);
    if (startNode.getID() == 0)
        startNode = calcStartEndNode(startPosition, 1000.0);
    
	AStarRoutingNode startRoutingNode = AStarRoutingNode(startNode, 0, 0.0, metric->calcCost(startNode, endNode));
	
	return calcShortestRoute(startRoutingNode, endNode);
}
GPSRoute AStar::calcShortestRoute(OSMNode startNode, OSMNode endNode)
{
	AStarRoutingNode startRoutingNode = AStarRoutingNode(startNode, 0, 0.0, metric->calcCost(startNode, endNode));
	
	return calcShortestRoute(startRoutingNode, endNode);
}


GPSRoute AStar::calcShortestRoute(AStarRoutingNode startNode, OSMNode endNode)
{
	boost::shared_ptr<AStarRoutingNode> startNodePointer(new AStarRoutingNode(startNode, 0, 0.0, metric->calcCost(startNode, endNode)));
	openlist->addNode(startNodePointer);
	nodeMap[startNode.getID()] = startNodePointer;
	boost::shared_ptr<AStarRoutingNode> actNode;
	
	//solang noch Punkte zu untersuchen sind...
	while (!openlist->isEmpty())
	{
		actNode = openlist->removeMinimumCostNode();
		closedlist->addNode(actNode->getID());
		if (actNode->getID() == endNode.getID())
		{
			break;	//Supi, Ende gefunden, Weg gefunden, Freude.
		}
		
		QList<boost::shared_ptr<OSMEdge> > edgeList = db->getEdges(*actNode, *(metric->getAssociatedPropertyTree()));
		for (QList<boost::shared_ptr<OSMEdge> >::iterator it = edgeList.begin(); it < edgeList.end(); it++)
		{
			if (!closedlist->contains((*it)->getEndNodeID()))
			{
				boost::shared_ptr<OSMNode> actEdgeEndNode = db->getNode((*it)->getEndNodeID());
				
                if (actEdgeEndNode.get() == 0)
                    continue;   //WORKAROUND
				
                if (!openlist->contains(actEdgeEndNode->getID()))
				{
					boost::shared_ptr<AStarRoutingNode> actEdgeRoutingEndNode(new AStarRoutingNode(*actEdgeEndNode, actNode->getID(), actNode->getOverallCost() + metric->calcCost(*actNode, *actEdgeEndNode, **it), metric->calcCost(*actEdgeEndNode, endNode)));
					if (actEdgeRoutingEndNode.get() == 0)
						qCritical() << "WARNING: no memory left.";
					openlist->addNode(actEdgeRoutingEndNode);
					nodeMap[actEdgeEndNode->getID()] = actEdgeRoutingEndNode;
				}
				else
				{
					boost::shared_ptr<AStarRoutingNode> actEdgeRoutingEndNode = nodeMap[actEdgeEndNode->getID()];
					//decrease wird nur ausgeführt, wenn der Wert wirklich kleiner wird.
					//float test = /*actNode->getOverallCost() +*/ metric->calcCost(*actNode, *actEdgeEndNode, **it);
					float newOverallCost = actNode->getOverallCost() + metric->calcCost(*actNode, *actEdgeEndNode, **it);
					if (newOverallCost < actEdgeRoutingEndNode->getOverallCost())
					{
						actEdgeRoutingEndNode->setOverallCost(newOverallCost);
						actEdgeRoutingEndNode->setPredecessor(actNode->getID());
						actEdgeRoutingEndNode->setHeuristicCost(metric->calcCost(*actEdgeEndNode, endNode));
					}
					openlist->decreaseKey(actEdgeRoutingEndNode);
					//nodeMap muss nicht aktualisiert werden; sind ja alles Pointer.
				}
			}
		}
	}
	
	qDebug() << "A* fertig... Suchraum hatte " << nodeMap.size() << " Elemente.";
    
	/* An diesem Punkt ist A* selbst schon fertig,
	 * es muss nur noch die berechnete Route rekonstruiert
	 * werden.
	 */
	if (actNode->getID() == endNode.getID())
	{
		boost::shared_ptr<AStarRoutingNode> actRoutePoint = actNode;
		GPSRoute r;
		while ((actRoutePoint->getPredecessor() != 0) && (actRoutePoint->getPredecessor() != actRoutePoint->getID()))	//solange es noch Vorgänger gibt
		{
			r.addWaypointInFront(*actRoutePoint);
			actRoutePoint = nodeMap[actRoutePoint->getPredecessor()];	//Vorgänger raussuchen
		}
		r.addWaypointInFront(*actRoutePoint);	//Startpunkt auch hinzufügen.
        nodeMap.clear();
		return r;
	}
	else
	{
        qDebug() << "AStar: Keine Route gefunden.";
		return GPSRoute();
	}
}

AStar::~AStar()
{
	delete openlist;
	delete closedlist;
}
