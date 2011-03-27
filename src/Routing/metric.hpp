#ifndef METRIC_HPP
#define METRIC_HPP

#include "src/DataPrimitives/DataPrimitives.hpp"
#include "src/DataPrimitives/osmpropertytree.hpp"
#include "src/Database/osmdatabase.hpp"
#include <boost/shared_ptr.hpp>

/**
 * In dieser Klasse wird die Metrik festgelegt, mit der bestimmt wird
 * welche Kosten einem Weg zugeschlagen werden in Abhängigkeit von
 * den Eigenschaften eines Weges. Interface, von dem geerbt werden sollte.
 */
class RouterMetric
{
protected:
	
public:
	virtual double calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge)=0;
	virtual double calcCost(const OSMNode& startNode, const OSMNode& endNode) {return startNode.calcDistance(endNode);}
	virtual boost::shared_ptr<OSMPropertyTree> getAssociatedPropertyTree()=0;
    virtual ~RouterMetric();
};

class EuclidianMetric : public RouterMetric
{
public:
	double calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge);
	boost::shared_ptr<OSMPropertyTree> getAssociatedPropertyTree();
};
class BikeMetric : public RouterMetric
{
private:
    OSMDatabaseReader* db;
    float penalty_Altitude;
    float penalty_NoCycleway;
    float penalty_NoCycleway_HighwayPrimary;    //Penalty to add in parts of distance
    float penalty_NoCycleway_HighwaySecondary;  //Penalty to add in parts of distance
    float penalty_NoCycleway_HighwayTertiary;   //Penalty to add in parts of distance
    float penalty_NoCycleway_HighwayPath;       //Penalty to add in parts of distance
    float penalty_NoCycleway_Dismount;          //Penalty to add in parts of distance
    float penalty_Cycleway_HighwayPrimary;      //Penalty to add in parts of distance
    float penalty_Cycleway_HighwaySecondary;    //Penalty to add in parts of distance
    float penalty_Cycleway_HighwayTertiary;     //Penalty to add in parts of distance
    float penalty_Cycleway_HighwayPath;         //Penalty to add in parts of distance
    float penalty_Highway_Trafficlights;        //Penalty in meters
    float penalty_Barrier_Cycle;                //Penalty in meters
public:
    BikeMetric(OSMDatabaseReader* dbReader, float penalty_Altitude=30.0,
               float penalty_NoCycleway=0.1,
               float penalty_NoCycleway_HighwayPrimary=0.3,
               float penalty_NoCycleway_HighwaySecondary=0.2,
               float penalty_NoCycleway_HighwayTertiary=0.15,
               float penalty_NoCycleway_HighwayPath=0.2,
               float penalty_NoCycleway_Dismount=3.00,
               float penalty_Cycleway_HighwayPrimary=0.1,
               float penalty_Cycleway_HighwaySecondary=0.05,
               float penalty_Cycleway_HighwayTertiary=0.0,
               float penalty_Cycleway_HighwayPath=0.1,
               float penalty_Highway_Trafficlights=60,
               float penalty_Barrier_Cycle=50)
                   : db(dbReader),
                    penalty_Altitude(penalty_Altitude),
                    penalty_NoCycleway(penalty_NoCycleway),
                    penalty_NoCycleway_HighwayPrimary(penalty_NoCycleway_HighwayPrimary),
                    penalty_NoCycleway_HighwaySecondary(penalty_NoCycleway_HighwaySecondary),
                    penalty_NoCycleway_HighwayTertiary(penalty_NoCycleway_HighwayTertiary),
                    penalty_NoCycleway_HighwayPath(penalty_NoCycleway_HighwayPath),
                    penalty_NoCycleway_Dismount(penalty_NoCycleway_Dismount),
                    penalty_Cycleway_HighwayPrimary(penalty_Cycleway_HighwayPrimary),
                    penalty_Cycleway_HighwaySecondary(penalty_Cycleway_HighwaySecondary),
                    penalty_Cycleway_HighwayTertiary(penalty_Cycleway_HighwayTertiary),
                    penalty_Cycleway_HighwayPath(penalty_Cycleway_HighwayPath),
                    penalty_Highway_Trafficlights(penalty_Highway_Trafficlights),
                    penalty_Barrier_Cycle(penalty_Barrier_Cycle)
    {}
    BikeMetric() : db(0) {}
	double calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge);
	boost::shared_ptr<OSMPropertyTree> getAssociatedPropertyTree();
    
    float getPenalty_Altitude()                     {return penalty_Altitude;}
    float getPenalty_NoCycleway()                   {return penalty_NoCycleway;}
    float getPenalty_NoCycleway_HighwayPrimary()    {return penalty_NoCycleway_HighwayPrimary;}
    float getPenalty_NoCycleway_HighwaySecondary()  {return penalty_NoCycleway_HighwaySecondary;}
    float getPenalty_NoCycleway_HighwayTertiary()   {return penalty_NoCycleway_HighwayTertiary;}
    float getPenalty_NoCycleway_HighwayPath()       {return penalty_NoCycleway_HighwayPath;}
    float getPenalty_NoCycleway_Dismount()          {return penalty_NoCycleway_Dismount;}
    float getPenalty_Cycleway_HighwayPrimary()      {return penalty_Cycleway_HighwayPrimary;}
    float getPenalty_Cycleway_HighwaySecondary()    {return penalty_Cycleway_HighwaySecondary;}
    float getPenalty_Cycleway_HighwayTertiary()     {return penalty_Cycleway_HighwayTertiary;}
    float getPenalty_Cycleway_HighwayPath()         {return penalty_Cycleway_HighwayPath;}
    float getPenalty_Highway_Trafficlights()        {return penalty_Highway_Trafficlights;}
    float getPenalty_Barrier_Cycle()                {return penalty_Barrier_Cycle;}
    
    void setPenalty_Altitude(float p)                   {penalty_Altitude=p;}
    void setPenalty_NoCycleway(float p)                 {penalty_NoCycleway=p;}
    void setPenalty_NoCycleway_HighwayPrimary(float p)  {penalty_NoCycleway_HighwayPrimary=p;}
    void setPenalty_NoCycleway_HighwaySecondary(float p){penalty_NoCycleway_HighwaySecondary=p;}
    void setPenalty_NoCycleway_HighwayTertiary(float p) {penalty_NoCycleway_HighwayTertiary=p;}
    void setPenalty_NoCycleway_HighwayPath(float p)     {penalty_NoCycleway_HighwayPath=p;}
    void setPenalty_NoCycleway_Dismount(float p)        {penalty_NoCycleway_Dismount=p;}
    void setPenalty_Cycleway_HighwayPrimary(float p)    {penalty_Cycleway_HighwayPrimary=p;}
    void setPenalty_Cycleway_HighwaySecondary(float p)  {penalty_Cycleway_HighwaySecondary=p;}
    void setPenalty_Cycleway_HighwayTertiary(float p)   {penalty_Cycleway_HighwayTertiary=p;}
    void setPenalty_Cycleway_HighwayPath(float p)       {penalty_Cycleway_HighwayPath=p;}
    void setPenalty_Highway_Trafficlights(float p)      {penalty_Highway_Trafficlights=p;}
    void setPenalty_Barrier_Cycle(float p)              {penalty_Barrier_Cycle=p;}
};
class CarMetric : public RouterMetric
{
public:
	double calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge);
	boost::shared_ptr<OSMPropertyTree> getAssociatedPropertyTree();
};
class FastRoutingMetric : public RouterMetric
{
public:
	double calcCost(const OSMNode& startNode, const OSMNode& endNode, const OSMEdge& edge);
	boost::shared_ptr<OSMPropertyTree> getAssociatedPropertyTree();
};

#endif // METRIC_HPP
