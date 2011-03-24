#include <iostream>
#include "src/Database/Parser/osmparser.hpp"
#include "src/Database/osmdatabase.hpp"
#include "src/Database/osmdatabasewriter.hpp"
#include "src/Routing/routing.hpp"
#include "src/Routing/astar.hpp"
#include "src/Routing/dijkstra.hpp"
#include <boost/shared_ptr.hpp>

using namespace std;

#define TESTPROGRAM
#ifdef TESTPROGRAM
    int main(int argc, char** argv)
    {
        QString filename("data/hagen.osm");
        
        if (argc > 1)
        {
            filename = QString(argv[1]);
        }
        
        cout << "Datenbank öffnen..." << endl;
        OSMInMemoryDatabase db;
        if (!db.openDatabase(filename))
        {
            qCritical() << "Datenbank konnte nicht geöffnet werden!";
            exit(1);
        }
        
        GPSPosition posHome(7.4805, 51.3567);
        GPSPosition posUni(7.267, 51.4469);
        //AStar astar(&db, new BikeMetric(&db, 15.0f), new BinaryHeap<AStarRoutingNode>(), new HashClosedList());
        AStar astar(&db, new CarMetric(), new BinaryHeap<AStarRoutingNode>(), new HashClosedList());
        GPSRoute r = astar.calcShortestRoute(posHome, posUni);
        
        return 0;
    }

#else
    int main(int argc, char** argv)
    {
        QString filename("data/hagen.osm");
        bool doRouting=true;
        boost::shared_ptr<OSMNode> startNode;
        boost::shared_ptr<OSMNode> endNode;
        
        GPSPosition startPos, endPos;
        
        if (argc > 1)
        {
            filename = QString(argv[1]);
        }
        
        cout << "Datenbank öffnen..." << endl;
        OSMInMemoryDatabase db;
        if (!db.openDatabase(filename))
        {
            qCritical() << "Datenbank konnte nicht geöffnet werden!";
            exit(1);
        }
        
        do
        {
            cout << "Knoteneingabe (ID oder Koordinaten, bei ID für den zweiten Wert 0 eintragen):" << endl;
            cerr << "Koordinaten noch nicht implementiert!" << endl;
            double lon1=0, lat1=0, lon2=0, lat2=0;
            cout << "Punkt1 (x/y): ";
            cin >> lon1;
            if (lon1<360)
                cin >> lat1;
            if (lat1==0)
            {
                startNode = db.getNode(lon1);
                if (startNode == 0)
                    cerr << "StartNode not found!" << endl;
            }
            else
            {
                startPos = GPSPosition(lon1, lat1);
            }
            
            cout << "Punkt2 (x/y): ";
            cin >> lon2 ;
            if (lon2<360)
                cin >> lat2;
            
            if (lat2==0)
            {
                endNode = db.getNode(lon2);
                if (endNode == 0)
                    cerr << "EndNode not found!" << endl;
            }
            else
            {
                endPos = GPSPosition(lon2, lat2);
            }
            
            float altitudePenalty;
            cout << "Wie viel Meter Umweg nimmst du pro gespartem Höhenmeter in Kauf? ";
            cin >> altitudePenalty;
            
            AStar astar(&db, new BikeMetric(&db, altitudePenalty), new BinaryHeap<AStarRoutingNode>(), new HashClosedList());
            GPSRoute r;
            if (lat1==0 && lat2==0)
            {
                r = astar.calcShortestRoute(*startNode, *endNode);
            }
            else if (lat1==0 && lat2!=0)
            {
                r = astar.calcShortestRoute(*startNode, endPos);
            }
            else if (lat1!=0 && lat2==0)
            {
                r = astar.calcShortestRoute(startPos, *endNode);
            }
            else if (lat1!=0 && lat2!=0)
            {
                r = astar.calcShortestRoute(startPos, endPos);
            }
            
            
            //Dijkstra dijkstra(&db, new BikeMetric(&db), new BinaryHeap<RoutingNode>(), new HashClosedList());
            //GPSRoute r = dijkstra.calcShortestRoute(*startNode, *endNode);
            
            std::string routeFilename;
            
            cout << "Route berechnet. Dateiname zum Speichern? ";
            cin >> routeFilename;
            
            GPSRoute::exportGPX(routeFilename.c_str(), r);
            
            
            cout << "Weitere Route berechnen? ";
            cin >> doRouting;
            
        } while (doRouting);
        
        /*
        Node* startNode = conn.getNode(434531445);
        Node* endNode = conn.getNode(1140778479);
        
        QList<Edge*> edgeList = conn.getEdges(*startNode);
        for (QList<Edge*>::iterator it = edgeList.begin(); it < edgeList.end(); it++)
        {
            cout << "Kante von " << (*it)->getStartNodeID() << " zu " << (*it)->getEndNodeID() << endl;
        }
        
        AStar astar(&conn, new BikeMetric(), new BinaryHeap<AStarRoutingNode>(), new HashClosedList());
        Route r = astar.calcShortestRoute(*startNode, *endNode);
        
        Route::exportGPX("test.gpx", r);
        */
        return 0;
    }
#endif
