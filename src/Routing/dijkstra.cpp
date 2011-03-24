#include "dijkstra.hpp"
#include <boost/shared_ptr.hpp>
#include <QList>
#include <QDebug>

Dijkstra::Dijkstra(OSMDatabaseReader* db, RouterMetric* metric, Heap<RoutingNode>* ds_, ClosedList* cl_) :
		Router(db, metric), datastructure(ds_), closedlist(cl_)
{
	
}

GPSRoute Dijkstra::calcShortestRoute(GPSPosition startPosition, GPSPosition endPosition)
{
	OSMNode startNode = calcStartEndNode(startPosition);
	OSMNode endNode = calcStartEndNode(endPosition);
	
	RoutingNode startRoutingNode = RoutingNode(startNode, 0, 0.0);
	
	return calcShortestRoute(startRoutingNode, endNode);
}
GPSRoute Dijkstra::calcShortestRoute(OSMNode startNode, GPSPosition endPosition)
{
	OSMNode endNode = calcStartEndNode(endPosition);
	
	RoutingNode startRoutingNode = RoutingNode(startNode, 0, 0.0);
	
	return calcShortestRoute(startRoutingNode, endNode);
}
GPSRoute Dijkstra::calcShortestRoute(GPSPosition startPosition, OSMNode endNode)
{
	OSMNode startNode = calcStartEndNode(startPosition);
	
	RoutingNode startRoutingNode = RoutingNode(startNode, 0, 0.0);
	
	return calcShortestRoute(startRoutingNode, endNode);
}
GPSRoute Dijkstra::calcShortestRoute(OSMNode startNode, OSMNode endNode)
{
	RoutingNode startRoutingNode = RoutingNode(startNode, 0, 0.0);
	
	return calcShortestRoute(startRoutingNode, endNode);
}

GPSRoute Dijkstra::calcShortestRoute(RoutingNode startNode, OSMNode endNode)
{
	boost::shared_ptr<RoutingNode> startNodePointer(new RoutingNode(startNode));
	datastructure->addNode(startNodePointer);
	nodeMap[startNode.getID()] = startNodePointer;
	boost::shared_ptr<RoutingNode> actNode;
		
	//solang noch Punkte zu untersuchen sind...
	while (!datastructure->isEmpty())
	{
		actNode = datastructure->removeMinimumCostNode();
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
				
				/* Man muss Dijkstra etwas anders behandeln, wenn man nicht allzuviel Speicher am Anfang
				 * für die Initialisierung verschwenden möchte. Ich habe bei A* geschmult und eine Art
				 * "closed List" verwendet.
				 * 
				 */
				if (!datastructure->contains(actEdgeEndNode->getID()))
				{
					boost::shared_ptr<RoutingNode> actEdgeRoutingEndNode(new RoutingNode(*actEdgeEndNode, actNode->getID(), actNode->getOverallCost() + metric->calcCost(*actNode, *actEdgeEndNode, **it)));
					datastructure->addNode(actEdgeRoutingEndNode);
					nodeMap[actEdgeEndNode->getID()] = actEdgeRoutingEndNode;
				}
				else
				{
					boost::shared_ptr<RoutingNode> actEdgeRoutingEndNode = nodeMap[actEdgeEndNode->getID()];
					//decrease wird nur ausgeführt, wenn der Wert wirklich kleiner wird.
					float newOverallCost = actNode->getOverallCost() + metric->calcCost(*actNode, *actEdgeEndNode, **it);
					if (newOverallCost < actEdgeRoutingEndNode->getOverallCost())
					{
						actEdgeRoutingEndNode->setOverallCost(newOverallCost);
						actEdgeRoutingEndNode->setPredecessor(actNode->getID());
					}
					datastructure->decreaseKey(actEdgeRoutingEndNode);
					//nodeMap muss nicht aktualisiert werden; sind ja alles Pointer.
				}
			}
		}
	}
	
	
	qDebug() << "Dijkstra fertig, Suchraum hatte " << nodeMap.size() << " Elemente.";
	
	/* An diesem Punkt ist Dijkstra selbst schon fertig,
	 * es muss nur noch die berechnete Route rekonstruiert
	 * werden.
	 */
	if (actNode->getID() == endNode.getID())
	{
		boost::shared_ptr<RoutingNode> actRoutePoint = actNode;
		GPSRoute r;
		while (actRoutePoint->getPredecessor() != 0)	//solange es noch Vorgänger gibt
		{
			r.addWaypointInFront(*actRoutePoint);
			actRoutePoint = nodeMap[actRoutePoint->getPredecessor()];	//Vorgänger raussuchen
		}
		r.addWaypointInFront(*actRoutePoint);	//Startpunkt auch hinzufügen.
		return r;
	}
	else
	{
        qDebug() << "Dijkstra fertig, keine Route gefunden.";
		return GPSRoute();
	}
}

Dijkstra::~Dijkstra()
{
	delete datastructure;
	delete closedlist;
}

