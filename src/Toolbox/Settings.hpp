#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include "src/DataPrimitives/DataPrimitives.hpp"

class Settings
{
private:
	QString standardDBFilename;
	//std::string poiRecorderFilename;
	GPSPosition standardPosition;
    
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
    
    int zoom;
    
	Settings();
	void createStandardSettingsFile(QString filename = "");
public:
	static Settings* getInstance();
	QString getLastOpenedDBFilename();
	GPSPosition getStandardPosition();
	void setStandardPosition(GPSPosition pos);
	void setLastOpenedDBFilename(QString filename);
    void setZoom(int zoom);
    int getZoom();
	~Settings();
	
	void loadXML(QString filename = "");
	void saveXML(QString filename = "");
    
    float getPenalty_Altitude();
    float getPenalty_NoCycleway();
    float getPenalty_NoCycleway_HighwayPrimary();
    float getPenalty_NoCycleway_HighwaySecondary();
    float getPenalty_NoCycleway_HighwayTertiary();
    float getPenalty_NoCycleway_HighwayPath();
    float getPenalty_NoCycleway_Dismount();
    float getPenalty_Cycleway_HighwayPrimary();
    float getPenalty_Cycleway_HighwaySecondary();
    float getPenalty_Cycleway_HighwayTertiary();
    float getPenalty_Cycleway_HighwayPath();
    float getPenalty_Highway_Trafficlights();
    float getPenalty_Barrier_Cycle();
    
    void setPenalty_Altitude(float p);
    void setPenalty_NoCycleway(float p);
    void setPenalty_NoCycleway_HighwayPrimary(float p);
    void setPenalty_NoCycleway_HighwaySecondary(float p);
    void setPenalty_NoCycleway_HighwayTertiary(float p);
    void setPenalty_NoCycleway_HighwayPath(float p);
    void setPenalty_NoCycleway_Dismount(float p);
    void setPenalty_Cycleway_HighwayPrimary(float p);
    void setPenalty_Cycleway_HighwaySecondary(float p);
    void setPenalty_Cycleway_HighwayTertiary(float p);
    void setPenalty_Cycleway_HighwayPath(float p);
    void setPenalty_Highway_Trafficlights(float p);
    void setPenalty_Barrier_Cycle(float p);
};

#endif /* SETTINGS_HPP_ */
