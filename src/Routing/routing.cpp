#include "src/Routing/routing.hpp"

RoutingNode::RoutingNode(OSMNode parent, ID_Datatype predecessor_, float overAllCost_) :
		OSMNode(parent.getID(), GPSPosition(parent.getLon(), parent.getLat()), parent.getProperties()), predecessor(predecessor_), overallCost(overAllCost_)
{
	
}

uint qHash(RoutingNode& node)
{
	return node.getID();
}
uint qHash(boost::shared_ptr<RoutingNode>& node)
{
	return node->getID();
}


Router::Router(OSMDatabaseReader* db_, RouterMetric* metric_) :
		db(db_), metric(metric_)
{
	
}

OSMNode Router::calcStartEndNode(GPSPosition pos, double radius)
{
    //TODO
    radius=0.0;
    pos=GPSPosition(0.0, 0.0);
	//Toolbox::log("Router::calcStartEndNode, radius:%1.0f", radius, Toolbox::FINER);
	////db->getNodes(pos, radius, &PropertyTreeNodeAlwaysTrue());
	//QList<Way> wayList;
	
	////Suche alle Ways heraus, die so in der Umgebung sind.
	//if (radius<0)
	//{
		//wayList = db->getWays(pos, 50.0, metric->getAssociatedPropertyTree());
		//if (wayList.size() == 0)
			//wayList = db->getWays(pos, 150.0, metric->getAssociatedPropertyTree());
		//if (wayList.size() == 0)
			//wayList = db->getWays(pos, 500.0, metric->getAssociatedPropertyTree());
	//}
	//else
		//QList<Way> wayList = db->getWays(pos, radius, metric->getAssociatedPropertyTree());
	
	//Toolbox::log("    found %1.0f Ways in range.", (double)wayList.size(), Toolbox::FINEST);
	//Way minDistanceWay = wayList.first();
	//Node minDistanceNode(0,Position(0.0,0.0),QList<Property>());
	//double minDistance = 99999999999999.0;
	//Node distanceNode = minDistanceNode;
	//double distance;
	
	//Toolbox::log("    cycling through ways....", Toolbox::FINEST);
	////von den Ways in der Umgebung wird jetzt der gewählt, der ganz nah dran ist.
	//for (QList<Way>::iterator it = wayList.begin(); it < wayList.end(); it++)
	//{
		//Toolbox::log("       getting minimal distance node....", Toolbox::FINEST);
		//distanceNode = it->getMinDistanceNode(pos);
		//Toolbox::log("       calculating distance....", Toolbox::FINEST);
		//distance = pos.calcDistance(distanceNode);
		//if (distance < minDistance)
		//{
			//Toolbox::log("       distance < minDistance! (%lf < %lf)", distance, minDistance, Toolbox::FINEST);
			//minDistanceNode = distanceNode;
			//minDistanceWay = *it;
			//minDistance = distance;
		//}
	//}
	////davon der nächste Punkt, der wird zurückgegeben.
	//Toolbox::log("Router::calcStartEndNode finished.", Toolbox::FINER);
	//return minDistanceNode;
    return OSMNode(0, pos, QList<OSMProperty>());
}
Router::~Router()
{
	delete metric;
	//db nicht löschen! brauchen wir ja noch soanders.
}

void HashClosedList::addNode(ID_Datatype nodeID)
{
	hashtable << nodeID;
}
void HashClosedList::removeNode(ID_Datatype nodeID)
{
	hashtable.remove(nodeID);
}
bool HashClosedList::contains(ID_Datatype nodeID)
{
	return hashtable.contains(nodeID);
}
int HashClosedList::size()
{
	return hashtable.size();
}
