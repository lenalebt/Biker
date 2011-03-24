#ifndef SPACEFILLINGCURVES_HPP
#define SPACEFILLINGCURVES_HPP

#include <stdint.h>
#include <QList>
#include "src/DataPrimitives/DataPrimitiveDefines.hpp"

/**
 * Diese Klasse implementiert ein Zahlenintervall. Es ist sichergestellt, dass
 * zu jedem Zeitpunkt p1<=p2 gilt.
 */
template <typename T>
class Interval
{
public:
	Interval(T p1_, T p2_) : p1(p1_), p2(p2_) {preserveOrder();}
	Interval() : p1(T()), p2(T()) {}
	T getP1() {return p1;}
	T getP2() {return p2;}
	void setP1(T p1_) {p1 = p1_; preserveOrder();}
	void setP2(T p2_) {p2 = p2_; preserveOrder();}
private:
	T p1, p2;
	inline void preserveOrder()
	{
		if (p1>p2)
		{
			T tmp = p1;
			p1=p2;
			p2=tmp;
		}
	}
};

/**
 * Dies ist das Interface für eine raumfüllende Kurve. So kann jederzeit die benutzte
 * raumfüllende Kurve für die Datenbank ausgetauscht werden, ohne viele andere Code-
 * zeilen anfassen zu müssen.
 */
class SpaceFillingCurve
{
private:
    double minLon, minLat, maxLon, maxLat;
    uint16_t meshSize;
public:
    SpaceFillingCurve(double minLon, double minLat, double maxLon, double maxLat) :
            minLon(minLon), minLat(minLat), maxLon(maxLon), maxLat(maxLat), meshSize(16) {}
    SpaceFillingCurve() :
            minLon(0.0), minLat(0.0), maxLon(180.0), maxLat(360.0), meshSize(16) {}
    
	/**
	 * Diese Funktion gibt zu einer bestimmten Koordinate und der Angabe,
	 * wie viele Iterationen mit der Raumfüllenden Kurve durchgeführt werden
	 * sollen, die entsprechende BucketID zurück. Das Ergebnis soll in einen
	 * 32bit-Integer passen, also dürfen die Eingabewerte höchstens 16bit-Integer
	 * sein.
	 */
	virtual uint32_t getBucketID(uint16_t x, uint16_t y, int iterationCount)=0;
	
    
    virtual uint32_t getBucketID(double lon, double lat)
    {
        double meshx = (maxLon-minLon);
        double meshy = (maxLat-minLat);
        
        ID_Datatype x,y;
        /* Rundungsfehler werden hässlich, wenn man das nicht unterscheidet.
         * Vergleich ist okay, weil die Werte im Fall gleichgesetzt wurden vorher,
         * und nicht Ergebnis einer Rechnung sind.
         */
        if (lon != maxLon)
        {
            if (lon != minLon)
                x = (ID_Datatype)((lon-minLon) / meshx * (2<<meshSize));
            else
                x = 0;
        }
        else
            x = (2<<meshSize);
        
        if (lat != maxLat)	//s.o.
        {
            if (lon != minLon)
                y = (ID_Datatype)((lat-minLat) / meshy * (2<<meshSize));
            else
                y = 0;
        }
        else
            y = (2<<meshSize);
        
        return this->getBucketID(x, y, 16);
    }

	/**
	 * Diese Funktion berechnet eine Liste von Indicies, die abgefragt werden müssen
	 * um alle Punkte in einem bestimmten Intervall zu erreichen - aber keinen mehr
	 * als nötig. Damit soll vermieden werden, unnötig viele Punkte aus der Datenbank
	 * zu laden und betrachten zu müssen.
	 */
	virtual QList<Interval<uint32_t> > getRectangleIndexList(uint16_t upperLeftX,
			uint16_t upperLeftY, uint16_t bottomRightX, uint16_t bottomRightY,
			int iterationCount)=0;
};

/**
 * Implementiert die Z-Kurve. Die Implementierung hat, bis auf Schleifen, 2 Zeilen :D.
 */
class ZOrderCurve : public SpaceFillingCurve
{
public:
	uint32_t getBucketID(uint16_t x, uint16_t y, int iterationCount);
	QList<Interval<uint32_t> > getRectangleIndexList(uint16_t upperLeftX,
			uint16_t upperLeftY, uint16_t bottomRightX, uint16_t bottomRightY,
			int iterationCount);
private:
	uint32_t spreadInteger(uint32_t number);
};


#endif // SPACEFILLINGCURVES_HPP
