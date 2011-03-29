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

#include "src/Routing/metric.hpp"

RouterMetric::~RouterMetric()
{
    
}

double EuclidianMetric::calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge)
{
	if ((edge.getStartNodeID() == startNode.getID()) && (edge.getEndNodeID() == endNode.getID()))
	{
		return startNode.calcDistance(endNode);
	}
	else
	{
		if (std::numeric_limits<double>::has_infinity)
			return std::numeric_limits<double>::infinity();
		else
			return std::numeric_limits<double>::max();
	}
}
boost::shared_ptr<OSMPropertyTree> EuclidianMetric::getAssociatedPropertyTree()
{
	static boost::shared_ptr<OSMPropertyTree> propTree;
	if (propTree.get() == 0)
	{
		QList<OSMProperty> propList;
		propList << OSMProperty("highway","primary");
		propList << OSMProperty("highway","secondary");
		propList << OSMProperty("highway","tertiary");
		propList << OSMProperty("highway","primary_link");
		propList << OSMProperty("highway","secondary_link");
		propList << OSMProperty("highway","tertiary_link");
		propList << OSMProperty("highway","residential");
		propList << OSMProperty("highway","road");
		propList << OSMProperty("highway","unclassified");
		propList << OSMProperty("highway","footway");
		propList << OSMProperty("highway","cycleway");
		propList << OSMProperty("highway","pedestrian");
		propList << OSMProperty("highway","path");
		propList << OSMProperty("highway","steps");
		propList << OSMProperty("highway","living_street");
		propTree = OSMPropertyTree::convertToOrPropertyTree(propList);
	}
	return propTree;
	//return new PropertyTreeNodeAlwaysTrue();
}
double CarMetric::calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge)
{
	if ((edge.getStartNodeID() == startNode.getID()) && (edge.getEndNodeID() == endNode.getID()))
	{
		double distance=0, penalty=0;
		distance = startNode.calcDistance(endNode);
		
		/* Hier werden Hindernisse mit Strafwerten belegt. Ampeln und Gitterumfahrungen nerven eben.
		 * Der Weg wird einfach entsprechend verlängert, wenn auf ein solches Hindernis
		 * getroffen wird. Die Werte sind erstmal irgendwie festgelegt und müssen noch
		 * angepasst werden. Idealerweise kann der Benutzer sie wählen.
		 */
		QList<OSMProperty> propList = startNode.getProperties();
		for (QList<OSMProperty>::iterator it = propList.begin(); it < propList.end(); it++)
		{
			if (*it == OSMProperty("highway", "traffic_lights"))
				penalty += 100.0;	//30m Strafweg für Ampeln.
			else if (*it == OSMProperty("barrier", "cycle_barrier"))
				penalty += 100000.0;	//100km Strafweg für Umlaufgitter. Sollte reichen.
			else if (*it == OSMProperty("barrier", "bollard"))
				penalty += 100000.0;	//100km Strafweg für Poller. Sollte reichen.
			else if (*it == OSMProperty("barrier", "gate"))
				penalty += 1000.0;	//1km Strafweg für Tore. Sollte reichen.
			else if (*it == OSMProperty("barrier", "lift_gate"))
				penalty += 500.0;	//500m Strafweg für Schranken.
			else if (*it == OSMProperty("traffic_calming", "bump"))
				penalty += 100.0;	//100m Strafweg für Bodenwellen.
			else if (*it == OSMProperty("traffic_calming", "chicane"))
				penalty += 100.0;	//100m Strafweg für Schikanen (oft bei Ortseingängen).
		}
		
		/* Hier werden bestimmte Straßenverhältnisse mit Strafwerten belegt. Je nach Länge der
		 * ursprünglichen Strecke sind die Wege entsprechend teurer.
		 */
		propList = edge.getProperties();
		
		for (QList<OSMProperty>::iterator it = propList.begin(); it < propList.end(); it++)
		{
			if ((*it == OSMProperty("highway", "motorway")) || (*it == OSMProperty("highway", "motorway_link")))
				penalty += 0;	//Autobahnen sind gut, keine Strafe.
			else if ((*it == OSMProperty("highway", "trunk")) || (*it == OSMProperty("highway", "trunk_link")))
				penalty += distance * 0.1;	//10% Strafe für autobahnähnliche Straßen. Autobahn ist schöner und schneller.
			else if ((*it == OSMProperty("highway", "primary")) || (*it == OSMProperty("highway", "primary_link")))
				penalty += distance * 0.3;	//30% Strafe für große Straßen
			else if ((*it == OSMProperty("highway", "secondary")) || (*it == OSMProperty("highway", "secondary_link")))
				penalty += distance * 0.4;	//40% Strafe für normale Straßen
			else if ((*it == OSMProperty("highway", "tertiary")) || (*it == OSMProperty("highway", "tertiary_link")))
				penalty += distance * 0.5;	//50% Strafe für kleinere Straßen
			else if (*it == OSMProperty("highway", "residential"))
				penalty += distance * 0.7;	//70% Strafe für Anwohnerstraßen
			else if (*it == OSMProperty("highway", "road"))
				penalty += distance * 0.5;	//50% Strafe für nicht klassifizierte Straßen
			else if (*it == OSMProperty("highway", "unclassified"))
				penalty += distance * 0.5;	//50% Strafe für nicht klassifizierte Straßen
			else if (*it == OSMProperty("highway", "living_street"))
				penalty += distance * 0.9;	//90% Strafe für "Spielstraßen"
		}
		return distance + penalty;
	}
	else
	{
		if (std::numeric_limits<double>::has_infinity)
			return std::numeric_limits<double>::infinity();
		else
			return std::numeric_limits<double>::max();
	}
}
boost::shared_ptr<OSMPropertyTree> CarMetric::getAssociatedPropertyTree()
{
	static boost::shared_ptr<OSMPropertyTree> propTree;
	if (propTree.get() == 0)
	{
		QList<OSMProperty> propList;
		propList << OSMProperty("highway","motorway");
		propList << OSMProperty("highway","motorway_link");
		propList << OSMProperty("highway","trunk");
		propList << OSMProperty("highway","trunk_link");
		propList << OSMProperty("highway","primary");
		propList << OSMProperty("highway","secondary");
		propList << OSMProperty("highway","tertiary");
		propList << OSMProperty("highway","primary_link");
		propList << OSMProperty("highway","secondary_link");
		propList << OSMProperty("highway","tertiary_link");
		propList << OSMProperty("highway","residential");
		propList << OSMProperty("highway","road");
		propList << OSMProperty("highway","unclassified");
		propList << OSMProperty("highway","living_street");
		propTree = OSMPropertyTree::convertToOrPropertyTree(propList);
	}
	return propTree;
	//return new PropertyTreeNodeAlwaysTrue();
}

double BikeMetric::calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge)
{
	if ((edge.getStartNodeID() == startNode.getID()) && (edge.getEndNodeID() == endNode.getID()))
	{
		double distance=0, penalty=0;
		distance = startNode.calcDistance(endNode);
		
		/* Hier werden Hindernisse mit Strafwerten belegt. Ampeln und Gitterumfahrungen nerven eben.
		 * Der Weg wird einfach entsprechend verlängert, wenn auf ein solches Hindernis
		 * getroffen wird. Die Werte sind erstmal irgendwie festgelegt und müssen noch
		 * angepasst werden. Idealerweise kann der Benutzer sie wählen.
		 */
		QList<OSMProperty> propList = startNode.getProperties();
		for (QList<OSMProperty>::iterator it = propList.begin(); it < propList.end(); it++)
		{
			if (*it == OSMProperty("highway", "traffic_lights"))
				penalty += penalty_Highway_Trafficlights;	//xm Strafweg für Ampeln.
			else if (*it == OSMProperty("barrier", "cycle_barrier"))
				penalty += penalty_Barrier_Cycle;	//40m Strafweg für Umlaufgitter.
			else if (*it == OSMProperty("barrier", "bollard"))
				penalty += penalty_Barrier_Cycle;		//5m Strafweg für Poller.
			else if (*it == OSMProperty("barrier", "gate"))
				penalty += penalty_Barrier_Cycle;	//10m Strafweg für Tore.
			else if (*it == OSMProperty("barrier", "lift_gate"))
				penalty += penalty_Barrier_Cycle;	//10m Strafweg für Schranken.
		}
		
		/* Hier werden bestimmte Straßenverhältnisse mit Strafwerten belegt. Je nach Länge der
		 * ursprünglichen Strecke sind die Wege entsprechend teurer.
		 */
		propList = edge.getProperties();
		
		bool cycleway=false;
		
		//erstmal nachsehen, obs irgendwie besonders positive Eigenschaften gibt (Radweg, ...)
		for (QList<OSMProperty>::iterator it = propList.begin(); it < propList.end(); it++)
		{
			if (*it == OSMProperty("highway", "cycleway") || *it == OSMProperty("cycleway", "*") || *it == OSMProperty("bicycle", "yes") || *it == OSMProperty("bicycle", "designated")|| *it == OSMProperty("bicycle", "dismount"))
				cycleway=true;	//Radweg vorhanden. Toll!
		}
		if (!cycleway)
		{	//mit Radweg ist alles etwas anders, daher trennen.
			for (QList<OSMProperty>::iterator it = propList.begin(); it < propList.end(); it++)
			{
				penalty += distance * penalty_NoCycleway;	//Kein Radweg? Erstmal 10% Strafweg.
				if (*it == OSMProperty("highway", "primary"))
					penalty += distance * penalty_NoCycleway_HighwayPrimary;	//30% Strafweg für große Straßen ohne Radweg. Ist ja gefährlich.
				else if (*it == OSMProperty("highway", "secondary"))
					penalty += distance * penalty_NoCycleway_HighwaySecondary;	//20% Strafe für kleinere Straßen
				else if (*it == OSMProperty("highway", "tertiary"))
					penalty += distance * penalty_NoCycleway_HighwayTertiary;	//15% Strafe für kleinere Straßen
				else if (*it == OSMProperty("highway", "steps"))
				{
					//Okay, mal sehen ob man die Stufenzahl sehen kann.
					int index = -1;//propList.indexOf(Property("step_count",""));//TODO
					if (index != -1)
					{
						double step_count = propList[index].getValue().toDouble();
						//50m Strafweg plus 10% der Distanz pro Treppenstufe.
						penalty += 50 + distance * step_count/10; 
					}
					else
					{
						//Treppen sind böse. 50m Strafweg plus 3 mal die Länge der Treppe.
						penalty += 50 + distance * 3;
					}
				}
                else if (*it == OSMProperty("highway", "path"))
					penalty += distance * penalty_NoCycleway_HighwayPath;	//20% Strafe für Feldwege und sowas
                else if ((*it == OSMProperty("bicycle", "dismount")) || (*it == OSMProperty("highway", "pedestrian")))
					penalty += distance * penalty_NoCycleway_Dismount;
				
			}
		}
        else
        {
			for (QList<OSMProperty>::iterator it = propList.begin(); it < propList.end(); it++)
			{
				if (*it == OSMProperty("highway", "primary"))
					penalty += distance * penalty_Cycleway_HighwayPrimary;	//30% Strafweg für große Straßen ohne Radweg. Ist ja gefährlich.
				else if (*it == OSMProperty("highway", "secondary"))
					penalty += distance * penalty_Cycleway_HighwaySecondary;	//20% Strafe für kleinere Straßen
				else if (*it == OSMProperty("highway", "tertiary"))
					penalty += distance * penalty_Cycleway_HighwayTertiary;	//15% Strafe für kleinere Straßen
				//else if (*it == OSMProperty("highway", "steps"))  //keine Treppen erlauben Fahrräder, und wenn ist das murks.
                else if (*it == OSMProperty("highway", "path"))
					penalty += distance * penalty_Cycleway_HighwayPath;	//20% Strafe für Feldwege und sowas
                else if (*it == OSMProperty("bicycle", "dismount"))
                    penalty += distance * penalty_NoCycleway_Dismount;  //fürs Absteigen gibts die gleiche Strafe wie ohne Radweg
			}
        }
        
        //TODO: Unabhängig ob Radweg oder nicht: Die Qualität der Straße sollte mit einfließen in die Berechnung, sofern Infos vorhanden.
        
        if (db != 0)
        {
            penalty += fabs((db->getAltitude(startNode.getLon(), startNode.getLat()) - db->getAltitude(endNode.getLon(), endNode.getLat())))
                        * penalty_Altitude; //jeder Höhenmeter wird bestraft mit x m Umweg.
        }
        
		return distance + penalty;
	}
	else
	{
		if (std::numeric_limits<double>::has_infinity)
			return std::numeric_limits<double>::infinity();
		else
			return std::numeric_limits<double>::max();
	}
}
boost::shared_ptr<OSMPropertyTree> BikeMetric::getAssociatedPropertyTree()
{
	static boost::shared_ptr<OSMPropertyTree> propTree;
	if (propTree.get() == 0)
	{
		QList<OSMProperty> propListA;
		propListA << OSMProperty("highway","primary");
		propListA << OSMProperty("highway","secondary");
		propListA << OSMProperty("highway","tertiary");
		propListA << OSMProperty("highway","primary_link");
		propListA << OSMProperty("highway","secondary_link");
		propListA << OSMProperty("highway","tertiary_link");
		propListA << OSMProperty("highway","residential");
		propListA << OSMProperty("highway","road");
		propListA << OSMProperty("highway","unclassified");
		propListA << OSMProperty("highway","cycleway");
		propListA << OSMProperty("highway","living_street");
		propListA << OSMProperty("highway","service");
		propListA << OSMProperty("highway","path");
        propListA << OSMProperty("cycleway","track");
        propListA << OSMProperty("cycleway","lane");
        propListA << OSMProperty("bicycle","yes");
		propListA << OSMProperty("highway","steps");		//nur erlaubt unter hohen Kosten (schieben).
        propListA << OSMProperty("highway","pedestrian");	//nur erlaubt unter hohen Kosten (schieben).
        propListA << OSMProperty("highway","footway");      //nur erlaubt unter hohen Kosten (schieben).
		propTree = (OSMPropertyTree::convertToOrPropertyTree(propListA));
	}
	return propTree;
	//return new PropertyTreeNodeAlwaysTrue();
}

double FastRoutingMetric::calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge)
{
	if ((edge.getStartNodeID() == startNode.getID()) && (edge.getEndNodeID() == endNode.getID()))
	{
		return startNode.calcDistance(endNode);
	}
	else
	{
		if (std::numeric_limits<double>::has_infinity)
			return std::numeric_limits<double>::infinity();
		else
			return std::numeric_limits<double>::max();
	}
}
boost::shared_ptr<OSMPropertyTree> FastRoutingMetric::getAssociatedPropertyTree()
{
	static boost::shared_ptr<OSMPropertyTree> propTree;
	if (propTree.get() == 0)
	{
		QList<OSMProperty> propList;
		//propList << OSMProperty("highway","motorway");
		//propList << OSMProperty("highway","motorway_link");
		//propList << OSMProperty("highway","trunk");
		//propList << OSMProperty("highway","trunk_link");
		propList << OSMProperty("highway","primary");
		propList << OSMProperty("highway","secondary");
		propList << OSMProperty("highway","tertiary");
		propList << OSMProperty("highway","primary_link");
		propList << OSMProperty("highway","secondary_link");
		propList << OSMProperty("highway","tertiary_link");
		//propList << OSMProperty("highway","residential");
		//propList << OSMProperty("highway","road");
		//propList << OSMProperty("highway","unclassified");
		//propList << OSMProperty("highway","living_street");
		propTree = OSMPropertyTree::convertToOrPropertyTree(propList);
	}
	return propTree;
	//return new PropertyTreeNodeAlwaysTrue();
}
