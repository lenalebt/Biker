#include "gpsposition.hpp"


#define EARTH_RADIUS 6371000

Position_Datatype GPSPosition::getLon() const
{
    return this->lon;
}
Position_Datatype GPSPosition::getLat() const
{
    return this->lat;
}

void GPSPosition::setLon(Position_Datatype lon)
{
    this->lon = lon;
}

void GPSPosition::setLat(Position_Datatype lat)
{
    this->lat = lat;
}

Position_Datatype GPSPosition::getRadLon() const
{
    return deg2rad<Position_Datatype>(this->lon);
}
Position_Datatype GPSPosition::getRadLat() const
{
    return deg2rad<Position_Datatype>(this->lat);
}

void GPSPosition::setRadLon(Position_Datatype lon)
{
    this->lon = rad2deg<Position_Datatype>(lon);
}

void GPSPosition::setRadLat(Position_Datatype lat)
{
    this->lat = rad2deg<Position_Datatype>(lat);
}

double GPSPosition::calcCourseAngle(GPSPosition p2) const
{
    if (!p2.isInitialized())
        return 0.0;
    //Berechnung mittels sphärischer Trigonometrie.
    //Fallunterscheidung nötig, da sonst durch Null geteilt würde (xi==0 => sin(xi)==0)
    if (*this == p2)
    {
        return 0.0;
    }
    else
    {
        double xi=calcXi(p2);
        if (this->getLon() <= p2.getLon())
            return rad2deg<double>(acos( (sin(p2.getRadLat()) - sin(this->getRadLat())*cos(xi))/(cos(this->getRadLat()) * sin(xi)) ));
        else
            return 360.0 - rad2deg<double>(acos( (sin(p2.getRadLat()) - sin(this->getRadLat())*cos(xi))/(cos(this->getRadLat()) * sin(xi)) ));
    }
}

double GPSPosition::calcDistance(GPSPosition p2) const
{
    if (!p2.isInitialized())
        return 0.0;
    /* Berechnung bezieht nicht ein, dass die Erde ein Rotationsellipsoid ist, daher
     * können größere Entfernungen ggf. ungenau werden. Die Berechnung nach WGS84 ist
     * allerdings ungleich rechenintensiver, daher soll das hier so bleiben.
     */
    //Fallunterscheidung ist nötig, da sonst Ergebnis NaN.
    if (*this == p2)
        return 0.0;
    else
        return calcXi(p2)*EARTH_RADIUS;
}

/**
 * Berechnet xi, das bei der Entfernungs- und Winkelberechnung gebraucht wird.
 */
double GPSPosition::calcXi(GPSPosition p2) const
{
    return acos(sin(this->getRadLat()) * sin(p2.getRadLat()) + cos(this->getRadLat()) * cos(p2.getRadLat()) * cos(p2.getRadLon() - this->getRadLon()));
}

bool operator==(const GPSPosition& p1, const GPSPosition& p2)
{
    return ((p1.getLat() == p2.getLat()) && (p1.getLon() == p2.getLon()));
}

bool operator==(const GPSPositionWithAltitude& p1, const GPSPositionWithAltitude& p2)
{
    return ((p1.getLat() == p2.getLat()) && (p1.getLon() == p2.getLon()) && (p1.getAltitude() == p2.getAltitude()));
}
bool operator!=(const GPSPosition& p1, const GPSPosition& p2)
{
    return !(p1 == p2);
}
bool operator!=(const GPSPositionWithAltitude& p1, const GPSPositionWithAltitude& p2)
{
    return !(p1 == p2);
}


bool GPSPosition::isInitialized() const
{
    return !((lon == 0.0) && (lat == 0.0));
}

GPSPosition GPSPosition::calcPositionInDistance(double courseAngle, double distance) const
{
    if (!this->isInitialized())
        return GPSPosition();

    GPSPosition target;

    //Ich züchte hier einen Klammernwald. An Weihnachten kann man sich dann schöne Klammerbäume hinstellen.
    double xi = distance / EARTH_RADIUS;
    target.setLat(    rad2deg(asin(  sin(this->getRadLat())*cos(xi) + cos(this->getRadLat())*sin(xi)*cos(deg2rad(courseAngle))  ))    );
    target.setLon(    this->lon + rad2deg(asin(   sin(xi)/cos(this->getRadLat())*sin(deg2rad(courseAngle))   ))    );

    return target;
}

