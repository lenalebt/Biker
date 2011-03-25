#ifndef ROUTING_HPP
#define ROUTING_HPP

#include "src/DataPrimitives/DataPrimitives.hpp"
#include "src/Database/osmdatabase.hpp"
#include <boost/shared_ptr.hpp>
#include <QList>
#include <QHash>
#include <QSet>
#include "src/Routing/metric.hpp"


class RoutingNode : public OSMNode
{
protected:
	ID_Datatype predecessor;	//Vorgänger
	float overallCost;			//Speicher sparen! Deshalb float
public:
	/**
	 * 
	 * @param parent Knoten, über den geroutet werden soll
	 * @param predecessor Vorgängerknoten, wird benötigt zum Wiederherstellen der Route. Kein shared_ptr um Zyklen im Speicher zu vermeiden, die zu Speicherlecks führen würden.
	 * @param overallCost Gesamtkosten bisher, um zu diesem Knoten zu gelangen
	 * @return
	 */
	RoutingNode(OSMNode parent, ID_Datatype predecessor, float overallCost);

	void setPredecessor(ID_Datatype p) {predecessor = p;}
	ID_Datatype getPredecessor() {return predecessor;}
	virtual void setOverallCost(float cost) {overallCost = cost;}
	virtual float getOverallCost() {return overallCost;}
	virtual bool operator<(const RoutingNode& other) {return this->overallCost < other.overallCost;}
};
uint qHash(RoutingNode& node);
uint qHash(boost::shared_ptr<RoutingNode>& node);

template<typename T>
class Heap
{
public:
    virtual boost::shared_ptr<T> removeMinimumCostNode()=0;
    virtual void addNode(boost::shared_ptr<T> node)=0;
    virtual void decreaseKey(boost::shared_ptr<T> node)=0;
    virtual bool contains(ID_Datatype nodeID)=0;
    virtual bool isEmpty()=0;
    virtual int size()=0;
    virtual ~Heap() {}
};

/**
 * Implementierung der Datenstruktur, die im Dijkstra benötigt wird,
 * als Heap mit Feldern. In der Wurzel steht das minimale Element.
 */
template<typename T>
class BinaryHeap : public Heap<T>
{
private:
	QList<boost::shared_ptr<T> > heap;
	QHash<ID_Datatype, int> positionInHeap;	//nötig für decreaseKey
	void letSink(int i)
	{
		int j=2*i+1;
		while (j<heap.size())	//"sinken lassen"
		{
			/* schau die Kinder von i an, welches ist kleiner?
			 * j ist das linke Kind, j+1 ist das rechte Kind.*/
			if ((j+1>=heap.size()) || (*(heap[j]) < *(heap[j+1])))
			{	//linkes Kind ist kleiner
				boost::shared_ptr<T> tmpVal = heap[i];	//Tauschen
				heap[i] = heap[j];
				heap[j] = tmpVal;
				positionInHeap[heap[i]->getID()] = i;
				positionInHeap[heap[j]->getID()] = j;
				i=j;	//und weiter, zum linken Kind
			}
			else	//rechtes Kind ist kleiner
			{
				boost::shared_ptr<T> tmpVal = heap[i];	//Tauschen
				heap[i] = heap[j+1];
				heap[j+1] = tmpVal;
				positionInHeap[heap[i]->getID()] = i;
				positionInHeap[heap[j+1]->getID()] = j+1;
				i=j+1;	//weiter, zum rechten Kind
			}
			j=2*i+1;
		}
	}
	void letAscend(int i)
	{
		int j=(i-1)/2;
		while (i != 0)	//steigen lassen...
		{
			/* Papa von i ansehen: Ist der kleiner? Wenn nein, tauschen.
			 * Wenn ja, fertig.*/
			if ((j>=heap.size()) || (*(heap[j]) < *(heap[i])))
			{	//Papa Kind ist kleiner: fertig.
				break;
			}
			else	//Kind ist kleiner, tauschen!
			{
				boost::shared_ptr<T> tmpVal = heap[i];	//Tauschen
				heap[i] = heap[j];
				heap[j] = tmpVal;
				positionInHeap[heap[i]->getID()] = i;
				positionInHeap[heap[j]->getID()] = j;
				i=j;	//weiter, zum Papa
			}
			j=(i-1)/2;
		}
	}
public:
	boost::shared_ptr<T> removeMinimumCostNode()
	{
		//Rückgabewert herausnehmen
		boost::shared_ptr<T> retVal = heap[0];
		heap[0] = heap.last();	//Wurzel durch letztes Element ersetzen
		heap.removeLast();		//und dieses am Ende wegnehmen
		positionInHeap.remove(retVal->getID());
		
		if (!this->isEmpty())
		{
			positionInHeap[heap[0]->getID()] = 0;
			letSink(0);		//erstes Element sinken lassen
		}
		
		return retVal;
	}
	void addNode(boost::shared_ptr<T> node)
	{
		heap << node;
		positionInHeap[node->getID()] = heap.size()-1;
		letAscend(heap.size()-1);	//letztes Element aufsteigen lassen
	}
	void decreaseKey(boost::shared_ptr<T> node)
	{
		if (positionInHeap.contains(node->getID()))
		{
//			int i = positionInHeap[node->getID()];
			letAscend(positionInHeap[node->getID()]);
		}
		else
		{
			addNode(node);
		}
	}
	bool isEmpty() {return heap.isEmpty();}
	bool contains(ID_Datatype nodeID) {return positionInHeap.contains(nodeID);}
	int size() {return positionInHeap.size();}
    
    ~BinaryHeap()
    {
        /*for (QListIterator<boost::shared_ptr<T> > it(heap); it.hasNext(); )
        {
            it.next().reset();
        }*/
        heap.clear();
    }
};

class ClosedList
{
public:
	virtual bool contains(ID_Datatype nodeID)=0;
	virtual void addNode(ID_Datatype nodeID)=0;
	virtual void removeNode(ID_Datatype nodeID)=0;
	virtual int size()=0;
};
class HashClosedList : public ClosedList
{
private:
	QSet<ID_Datatype> hashtable;
public:
	bool contains(ID_Datatype nodeID);
	void addNode(ID_Datatype nodeID);
	void removeNode(ID_Datatype nodeID);
	int size();
};

/**
 * Diese Klasse ist ein Grundgerüst für einen Routingalgorithmus.
 * Alle Routingalgorithmen sollen das gleiche Interface bieten, 
 * damit sie leicht gegeneinander ausgetauscht werden können.
 */
class Router
{
protected:
	OSMDatabaseReader* db;
	RouterMetric* metric;
public:
	/**
	 * Berechnet die kürzeste Route zwischen Start- und Endposition.
	 * Dabei wird zuerst ein Startknoten bestimmt im nahen Umkreis der
	 * Startposition, und ein Endknoten im nahen Umkreis der Endposition.
	 * Dann wird mit einem Routingalgorithmus nach Wahl (ableiten!) eine
	 * kürzeste Route zwischen den Punkten berechnet und als Route
	 * zurückgegeben.
	 * @param startPosition
	 * @param endPosition
	 * @return
	 */
	virtual GPSRoute calcShortestRoute(GPSPosition startPosition, GPSPosition endPosition)=0;
	virtual GPSRoute calcShortestRoute(OSMNode startNode, GPSPosition endPosition)=0;
	virtual GPSRoute calcShortestRoute(GPSPosition startPosition, OSMNode endNode)=0;
    virtual GPSRoute calcShortestRoute(OSMNode startNode, OSMNode endNode)=0;
	
	virtual OSMNode calcStartEndNode(GPSPosition pos, double radius=50.0);
	Router(OSMDatabaseReader* db, RouterMetric* metric);
	virtual ~Router();
};

#endif // ROUTING_HPP
