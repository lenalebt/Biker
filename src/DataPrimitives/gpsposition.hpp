#ifndef GPSPOSITION_HPP
#define GPSPOSITION_HPP

#include "src/DataPrimitives/DataPrimitiveDefines.hpp"
#include "math.h"

class GPSPosition
{
public:
    virtual Position_Datatype getLon() const;
    virtual Position_Datatype getLat() const;
    virtual Position_Datatype getRadLon() const;
    virtual Position_Datatype getRadLat() const;
    virtual void setLon(Position_Datatype lon);
    virtual void setLat(Position_Datatype lat);
    virtual void setRadLon(Position_Datatype lon);
    virtual void setRadLat(Position_Datatype lat);

    //Die folgenden Funktionen dienen zur Berechnung von Zusammenhängen mehrerer Koordinaten (Entfernung, ...)
    /**
     * Berechnet die Entfernung zweier Punkte in Metern.
     * Die Berechnung erfolgt nicht nach dem WGS84-Ellipsoid, kann daher
     * für größere Entfernungen ungenau werden.
     */
    virtual double calcDistance(GPSPosition p2) const;
    /**
     * Berechnet den Kurswinkel, gesehen vom Punkt auf p2 zu, in Grad.
     */
    virtual double calcCourseAngle(GPSPosition p2) const;

    /**
     * Wegpunktprojektion.
     * Berechnet einen Punkt in einer Entfernung von distance Metern unter einem Kurswinkel
     * von courseAngle.
     * @param courseAngle
     * @param distance
     * @return
     */
    virtual GPSPosition calcPositionInDistance(double courseAngle, double distance) const;
    virtual bool isInitialized() const;

    GPSPosition() : lon(0.0), lat(0.0) {}
    GPSPosition(Position_Datatype lon, Position_Datatype lat) : lon(lon), lat(lat) {}
    virtual ~GPSPosition() {}
protected:
    Position_Datatype lon;
    Position_Datatype lat;

private:
    double calcXi(GPSPosition p2) const;
    /**
     * Rechnet um von Grad nach Bogenmaß. Template-Funktion,
     * gedacht für float und double.
     */
    template<typename T>
    T deg2rad(T deg) const
    {
        return deg/180.0*M_PI;
    }

    /**
     * Rechnet um von Bogenmaß nach Grad. Template-Funktion,
     * gedacht für float und double.
     */
    template<typename T>
    T rad2deg(T rad) const
    {
        return rad*180.0/M_PI;
    }
};

class GPSPositionWithAltitude : public GPSPosition
{
private:
    float altitude;
public:
    void setAltitude(float altitude) {this->altitude = altitude;}
    float getAltitude() const {return altitude;}

    GPSPositionWithAltitude(Position_Datatype lon, Position_Datatype lat, float altitude) :
            GPSPosition(lon, lat), altitude(altitude) {}
};

bool operator==(const GPSPosition& p1, const GPSPosition& p2);
bool operator==(const GPSPositionWithAltitude& p1, const GPSPositionWithAltitude& p2);
bool operator!=(const GPSPosition& p1, const GPSPosition& p2);
bool operator!=(const GPSPositionWithAltitude& p1, const GPSPositionWithAltitude& p2);


#endif // GPSPOSITION_HPP
