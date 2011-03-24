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
        bool doRouting=true;
        boost::shared_ptr<OSMNode> startNode;
        boost::shared_ptr<OSMNode> endNode;
        
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
            double lon1, lat1, lon2, lat2;
            cout << "Punkt1 (x/y): ";
            cin >> lon1 >> lat1;
            if (lat1==0)
            {
                startNode = db.getNode(lon1);
            }
            else
            {
                
            }
            if (startNode == 0)
                cerr << "StartNode not found!" << endl;
            
            cout << "Punkt2 (x/y): ";
            cin >> lon2 >> lat2;
            
            if (lat2==0)
            {
                endNode = db.getNode(lon2);
            }
            else
            {
                
            }
            if (endNode == 0)
                cerr << "EndNode not found!" << endl;
            
            //AStar astar(&db, new BikeMetric(&db), new BinaryHeap<AStarRoutingNode>(), new HashClosedList());
            //GPSRoute r = astar.calcShortestRoute(*startNode, *endNode);
            
            Dijkstra dijkstra(&db, new BikeMetric(&db), new BinaryHeap<RoutingNode>(), new HashClosedList());
            GPSRoute r = dijkstra.calcShortestRoute(*startNode, *endNode);
            
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

#else

#endif
