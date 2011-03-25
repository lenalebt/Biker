#include "gpsroute.hpp"

#include <limits>
#include <QtXml>
#include <stdexcept>
#include <iostream>




double GPSRoute::calcLength() const
{
    double length = 0.0;

    if (this->isEmpty())
        return 0.0;

    GPSPosition pOld;
    pOld = *(waypointList.begin());
    for(QList<GPSPosition>::const_iterator it = waypointList.begin()+1; it < waypointList.end(); it++)
    {
        if (!it->isInitialized())
            continue;	//sorgt für richtige Berechnung, wenn die Route (0.0/0.0) enthält (hoffentlich)
        length += pOld.calcDistance(*it);	//Entfernung zum nächsten Punkt ausrechnen und dazurechnen
        pOld = *it;
    }
    return length;
}
double GPSRoute::calcLengthStartingAtID(int id) const
{
    double length = 0.0;
    GPSPosition pOld;
    if ((waypointList.begin()+id) >= waypointList.end())
        return 0.0;

    pOld = *(waypointList.begin()+id);
    for(QList<GPSPosition>::const_iterator it = (waypointList.begin()+id+1); it < waypointList.end(); it++)
    {
        length += pOld.calcDistance(*it);	//Entfernung zum nächsten Punkt ausrechnen und dazurechnen
        pOld = *it;
    }
    return length;
}


int GPSRoute::getClosestWaypointID(GPSPosition p) const
{
    /*
     * Brute Force. Das ist zwar unschön, aber macht bei den paar Punkten kaum etwas aus,
     * ist den Aufwand eigentlich nicht wert, hier eine riesige Datenstruktur für zu bauen.
     * Vor allem sollte diese Funktion äußerst selten gebraucht werden.
     */
    double lowestDistance = std::numeric_limits<double>::max();	//größten double zuweisen als kleinste Entfernung
    double distance;
    int lowestDistancePointID = -1;

    for(int i = 0; i<waypointList.size(); i++)
    {
        distance = p.calcDistance(waypointList[i]);
        if (distance < lowestDistance)
        {
            lowestDistance = distance;
            lowestDistancePointID = i;
        }
    }

    return lowestDistancePointID;
}

GPSPosition GPSRoute::getClosestWaypoint(GPSPosition p) const
{
    return getWaypoint(getClosestWaypointID(p));
}

void GPSRoute::addWaypoint(GPSPosition p)
{
    if (p.isInitialized())
    {
        waypointList << p;
    }
}

void GPSRoute::addWaypointInFront(GPSPosition p)
{
    if (p.isInitialized())
    {
        waypointList.push_front(p);
    }
}

GPSPosition GPSRoute::getWaypoint(int i) const
{
    if ((i>=0) && (i<waypointList.size()))
        return waypointList.at(i);
    else
        return GPSPosition();
}

int GPSRoute::size() const
{
    return waypointList.size();	//2 Größer als die Anzahl Wegpunkte wegen Start- und Endpunkt.
}

GPSRoute::GPSRoute() :
        actWaypoint(0)
{

}

GPSRoute::GPSRoute(const GPSRoute& r) :
        waypointList(r.waypointList), actWaypoint(r.actWaypoint)
{
    /* Die WaypointList ist implicitly shared, d.h. das Objekt wird durch den Copy-Konstruktor nicht
     * kopiert, sondern erst in dem Moment, wo sich die Daten anfangen zu unterscheiden.
     */
}

GPSPosition GPSRoute::calcPositionInDistance(GPSPosition pStart, float maxDistance, int nextWaypointID) const
{
    float distance = pStart.calcDistance(getWaypoint(nextWaypointID));
    GPSPosition pOld = pStart;
    for (int i=nextWaypointID; i<size(); i++)
    {
        distance += pOld.calcDistance(getWaypoint(i));
        pOld = getWaypoint(i);
        if (distance >= maxDistance)
            return pOld;
    }
    return pOld;
}

bool GPSRoute::isEmpty() const
{
    return waypointList.empty();
}

void GPSRoute::clear()
{
    waypointList.clear();
}

GPSRoute GPSRoute::importGPX(QString gpxFilename)
{
    QDomDocument doc("gpxFile");
    GPSRoute r;
    QFile file(gpxFilename);

    if (!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Opening file for reading failed." << std::endl;
        throw std::ios_base::failure("Opening file for reading failed.");
        return r;
    }

    if (!doc.setContent(&file))
    {
        file.close();
        std::cerr << "DOM failed to read file." << std::endl;
        throw std::ios_base::failure("DOM failed to read file.");
        return r;
    }
    file.close();	//Alles eingelesen, prima. Datei wird nicht mehr gebraucht!

    QDomElement root = doc.documentElement();
    if (root.tagName().toLower() != "gpx")
    {
        std::cerr << "File seems not to be a GPX-file." << std::endl;
        throw std::ios_base::failure("File seems not to be a GPX-file.");
        return r;
    }

    //Trackpoints auflisten, durchwandern, und extrahieren
    QDomNode trkNode = root.firstChild();
    while (trkNode.isNull() || (trkNode.toElement().tagName().toLower() != "trk" ))
    {
        trkNode = trkNode.nextSibling();
        if (trkNode.isNull())
        {
            std::cerr << "No track found in GPX-file." << std::endl;
            throw std::ios_base::failure("No track found in GPX-file.");
            return r;
        }
    }
    if (!trkNode.isNull() && (trkNode.toElement().tagName().toLower() == "trk" ))
    {
        QDomNode trkSegNode = trkNode.firstChild();
        while (trkSegNode.isNull() || (trkSegNode.toElement().tagName().toLower() != "trkseg" ))
        {
            trkSegNode = trkSegNode.nextSibling();
            if (trkSegNode.isNull())
            {
                std::cerr << "Track segment not found." << std::endl;
                throw std::ios_base::failure("Track segment not found.");
                return r;
            }
        }
        if (!trkSegNode.isNull() && (trkSegNode.toElement().tagName().toLower() == "trkseg"))
        {
            QDomNode trkPointNode = trkSegNode.firstChild();
            while (!trkPointNode.isNull())
            {
                QDomElement trkEl = trkPointNode.toElement();

                if (!trkEl.isNull() && trkEl.tagName().toLower() == "trkpt")	//gültigen Trackpoint entdeckt
                {
                    //Trackpoint hinzufügen zur Liste.
                    r.addWaypoint(GPSPosition(trkEl.attribute("lon", "0.0").toDouble(), trkEl.attribute("lat", "0.0").toDouble()));
                }
                trkPointNode = trkPointNode.nextSibling();	//auf zum nächsten Trackpoint!
            }
        }
        else
        {
            std::cerr << "Track segment not found." << std::endl;
            throw std::ios_base::failure("Track segment not found.");
            return r;
        }
    }
    else
    {
        std::cerr << "No track found in GPX-file." << std::endl;
        throw std::ios_base::failure("No track found in GPX-file.");
        return r;
    }

    return r;
}

void GPSRoute::exportGPX(QString gpxFilename, GPSRoute r)
{
    QDomDocument doc("gpx");
    //zuerst das Grundelement erstellen, dann runter bis die Wegpunkte eingefügt werden.
    QDomElement root = doc.createElement("gpx");
    root.setAttribute("version", "1.0");
    root.setAttribute("xmlns", "");			//TODO: Werte dazu!
    root.setAttribute("xmlns:xsi", "");
    root.setAttribute("xsi:schemaLocation", "");
    root.setAttribute("creator", "LeNa");
    doc.appendChild(root);

    QDomElement trkNode = doc.createElement("trk");
    root.appendChild(trkNode);

    QDomElement trkSegNode = doc.createElement("trkseg");
    trkNode.appendChild(trkSegNode);

    QDomElement trkPoint;
    //Wegpunkt zur Liste hinzutun
    for (int i=0; i<r.size(); i++)
    {
        trkPoint = doc.createElement("trkpt");
        trkPoint.setAttribute("lon", r.getWaypoint(i).getLon());
        trkPoint.setAttribute("lat", r.getWaypoint(i).getLat());
        trkSegNode.appendChild(trkPoint);
    }

    //Datei öffnen und so
    QFile file(gpxFilename);
    if (!file.open(QIODevice::WriteOnly))	//Versuche, die Datei zu öffnen
    {
        std::cerr << "Opening file for writing failed." << std::endl;
        throw std::ios_base::failure("Opening file for writing failed.");
        return;
    }
    //jetzt noch Daten wegschreiben
    QTextStream stream(&file);
    stream << doc.toString();
    file.close();

}

void GPSRoute::reverse()
{
    QList<GPSPosition> reverseWaypointList;
    for (int i=waypointList.size()-1; i>=0; i--)
        reverseWaypointList << waypointList[i];
    waypointList = reverseWaypointList;
}

void GPSRoute::setDestination(GPSPosition dest)
{
    if (dest.isInitialized())
    {
        if (getDestination() != dest)
            waypointList << dest;
    }
}
GPSPosition GPSRoute::getDestination() const
{
    if (!waypointList.empty())
        return waypointList.at(waypointList.size()-1);
    else
        return GPSPosition();
}

void GPSRoute::setStartingPoint(GPSPosition start)
{
    if (start.isInitialized())
    {
        if (getStartingPoint() != start)
            waypointList.push_front(start);
    }
}
GPSPosition GPSRoute::getStartingPoint() const
{
    if (waypointList.empty())
        return waypointList.at(0);
    else
        return GPSPosition();
}

void GPSRoute::addRoute(GPSRoute route)
{
    //Vorteil dieses Ansatzes: (0.0/0.0) wird keinesfalls hinzugefügt zur Route...
    QList<GPSPosition> list = route.waypointList;
    for (QList<GPSPosition>::iterator it = list.begin(); it < list.end(); it++)
    {
        this->addWaypoint(*it);
    }
}
