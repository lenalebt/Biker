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

#include "src/Toolbox/Settings.hpp"
#include "src/DataPrimitives/DataPrimitives.hpp"
#include <QtXml>

Settings* Settings::getInstance()
{
	static Settings* instance;
	if (instance == 0)
	{
		instance = new Settings();
	}
	return instance;
}

void Settings::createStandardSettingsFile(QString filename)
{
	standardDBFilename = "";
	standardPosition = GPSPosition(7.5, 51.3);
    penalty_Altitude = 20;
    penalty_NoCycleway = 10;
    penalty_NoCycleway_HighwayPrimary = 30;
    penalty_NoCycleway_HighwaySecondary = 20;
    penalty_NoCycleway_HighwayTertiary = 15;
    penalty_NoCycleway_HighwayPath = 20;
    penalty_NoCycleway_Dismount = 300;
    penalty_Cycleway_HighwayPrimary = 10;
    penalty_Cycleway_HighwaySecondary = 5;
    penalty_Cycleway_HighwayTertiary = 0;
    penalty_Cycleway_HighwayPath = 10;
    penalty_Highway_Trafficlights = 60;
    penalty_Barrier_Cycle = 50;
	
	saveXML(filename);
}
void Settings::loadXML(QString filename)
{
    if (filename == "")
        filename = QDir::homePath() + "/.biker/settings.xml";
    qDebug() << "Load settings: " << filename;
    
	QDomDocument doc("Settings");
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		createStandardSettingsFile();
		return;
	}
	if (!doc.setContent(&file))
	{
		file.close();
		qDebug() << QString("DOM failed to read file ") + filename;
		throw std::ios_base::failure("DOM failed to read file.");
	}
	file.close();	//Alles eingelesen, prima. Datei wird nicht mehr gebraucht!
	
	
	QDomElement root = doc.documentElement();
	if (root.tagName().toLower() != "settings")
	{
		qDebug() << "File seems not to be a settings-file.";
		throw std::ios_base::failure("File seems not to be a settings-file.");
	}
	
    QLocale locale(QLocale::C);
    
	QDomNode item = root.firstChild();
	//Durch alle <item>s laufen.
	while (!item.isNull())
	{
		if (item.toElement().tagName().toLower() == "standardposition")
		{
			standardPosition = GPSPosition(item.toElement().attribute("lon","7.5").toDouble(), item.toElement().attribute("lat","51.3").toDouble());
		}
		else if (item.toElement().tagName().toLower() == "standarddatabase")
		{
			standardDBFilename = item.toElement().attribute("filename","nrw-ausschnitt.db");
		}
        else if (item.toElement().tagName() == "penalty_Altitude")
        {
            penalty_Altitude = item.toElement().attribute("value", locale.toString(penalty_Altitude, 'f', 2)).toFloat();
        }
        else if (item.toElement().tagName() == "penalty_NoCycleway")
        {
            penalty_NoCycleway = item.toElement().attribute("value", locale.toString(penalty_NoCycleway, 'f', 2)).toFloat();
        }
        else if (item.toElement().tagName() == "penalty_NoCycleway_HighwayPrimary")
        {
            penalty_NoCycleway_HighwayPrimary = item.toElement().attribute("value", locale.toString(penalty_NoCycleway_HighwayPrimary, 'f', 2)).toFloat();
        }
        else if (item.toElement().tagName() == "penalty_NoCycleway_HighwaySecondary")
        {
            penalty_NoCycleway_HighwaySecondary = item.toElement().attribute("value", locale.toString(penalty_NoCycleway_HighwaySecondary, 'f', 2)).toFloat();
        }
        else if (item.toElement().tagName() == "penalty_NoCycleway_HighwayTertiary")
        {
            penalty_NoCycleway_HighwayTertiary = item.toElement().attribute("value", locale.toString(penalty_NoCycleway_HighwayTertiary, 'f', 2)).toFloat();
        }
        else if (item.toElement().tagName() == "penalty_NoCycleway_HighwayPath")
        {
            penalty_NoCycleway_HighwayPath = item.toElement().attribute("value", locale.toString(penalty_NoCycleway_HighwayPath, 'f', 2)).toFloat();
        }
        else if (item.toElement().tagName() == "penalty_NoCycleway_Dismount")
        {
            penalty_NoCycleway_Dismount = item.toElement().attribute("value", locale.toString(penalty_NoCycleway_Dismount, 'f', 2)).toFloat();
        }
        else if (item.toElement().tagName() == "penalty_Cycleway_HighwayPrimary")
        {
            penalty_Cycleway_HighwayPrimary = item.toElement().attribute("value", locale.toString(penalty_Cycleway_HighwayPrimary, 'f', 2)).toFloat();
        }
        else if (item.toElement().tagName() == "penalty_Cycleway_HighwaySecondary")
        {
            penalty_Cycleway_HighwaySecondary = item.toElement().attribute("value", locale.toString(penalty_Cycleway_HighwaySecondary, 'f', 2)).toFloat();
        }
        else if (item.toElement().tagName() == "penalty_Cycleway_HighwayTertiary")
        {
            penalty_Cycleway_HighwayTertiary = item.toElement().attribute("value", locale.toString(penalty_Cycleway_HighwayTertiary, 'f', 2)).toFloat();
        }
        else if (item.toElement().tagName() == "penalty_Cycleway_HighwayPath")
        {
            penalty_Cycleway_HighwayPath = item.toElement().attribute("value", locale.toString(penalty_Cycleway_HighwayPath, 'f', 2)).toFloat();
        }
        else if (item.toElement().tagName() == "penalty_Highway_Trafficlights")
        {
            penalty_Highway_Trafficlights = item.toElement().attribute("value", locale.toString(penalty_Highway_Trafficlights, 'f', 2)).toFloat();
        }
        else if (item.toElement().tagName() == "penalty_Barrier_Cycle")
        {
            penalty_Barrier_Cycle = item.toElement().attribute("value", locale.toString(penalty_Barrier_Cycle, 'f', 2)).toFloat();
        }
        else if (item.toElement().tagName() == "zoom")
        {
            zoom = item.toElement().attribute("value", locale.toString(zoom)).toInt();
        }
		item = item.nextSibling();	//auf zum naechsten Item!
	}
}
void Settings::saveXML(QString filename)
{
    if (filename == "")
        filename = QDir::homePath() + "/.biker/settings.xml";
    qDebug() << "Save settings: " << filename;
    
    
	QDomDocument doc("Settings");
	QDomElement root = doc.createElement( "settings" );
	doc.appendChild( root );
	
	QDomElement posEl = doc.createElement( "standardposition" );
    QLocale locale(QLocale::C);
	posEl.setAttribute("lon", locale.toString(standardPosition.getLon(), 'f', 10));
	posEl.setAttribute("lat", locale.toString(standardPosition.getLat(), 'f', 10));
	root.appendChild(posEl);
	
	QDomElement standdardDBEl = doc.createElement( "standarddatabase" );
	standdardDBEl.setAttribute("filename", standardDBFilename);
	root.appendChild(standdardDBEl);
	
	
	QDomElement penalty = doc.createElement( "penalty_altitude" );
	penalty.setAttribute("value", locale.toString(penalty_Altitude, 'f', 2));
	root.appendChild(penalty);
	
    penalty = doc.createElement( "penalty_Altitude" );
    penalty.setAttribute("value", locale.toString(penalty_Altitude, 'f', 2));
    root.appendChild(penalty);
    penalty = doc.createElement( "penalty_Barrier_Cycle" );
    penalty.setAttribute("value", locale.toString(penalty_Barrier_Cycle, 'f', 2));
    root.appendChild(penalty);
    penalty = doc.createElement( "penalty_Cycleway_HighwayPath" );
    penalty.setAttribute("value", locale.toString(penalty_Cycleway_HighwayPath, 'f', 2));
    root.appendChild(penalty);
    penalty = doc.createElement( "penalty_Cycleway_HighwayPrimary" );
    penalty.setAttribute("value", locale.toString(penalty_Cycleway_HighwayPrimary, 'f', 2));
    root.appendChild(penalty);
    penalty = doc.createElement( "penalty_Cycleway_HighwaySecondary" );
    penalty.setAttribute("value", locale.toString(penalty_Cycleway_HighwaySecondary, 'f', 2));
    root.appendChild(penalty);
    penalty = doc.createElement( "penalty_Cycleway_HighwayTertiary" );
    penalty.setAttribute("value", locale.toString(penalty_Cycleway_HighwayTertiary, 'f', 2));
    root.appendChild(penalty);
    penalty = doc.createElement( "penalty_Highway_Trafficlights" );
    penalty.setAttribute("value", locale.toString(penalty_Highway_Trafficlights, 'f', 2));
    root.appendChild(penalty);
    penalty = doc.createElement( "penalty_NoCycleway" );
    penalty.setAttribute("value", locale.toString(penalty_NoCycleway, 'f', 2));
    root.appendChild(penalty);
    penalty = doc.createElement( "penalty_NoCycleway_Dismount" );
    penalty.setAttribute("value", locale.toString(penalty_NoCycleway_Dismount, 'f', 2));
    root.appendChild(penalty);
    penalty = doc.createElement( "penalty_NoCycleway_HighwayPath" );
    penalty.setAttribute("value", locale.toString(penalty_NoCycleway_HighwayPath, 'f', 2));
    root.appendChild(penalty);
    penalty = doc.createElement( "penalty_NoCycleway_HighwayPrimary" );
    penalty.setAttribute("value", locale.toString(penalty_NoCycleway_HighwayPrimary, 'f', 2));
    root.appendChild(penalty);
    penalty = doc.createElement( "penalty_NoCycleway_HighwaySecondary" );
    penalty.setAttribute("value", locale.toString(penalty_NoCycleway_HighwaySecondary, 'f', 2));
    root.appendChild(penalty);
    penalty = doc.createElement( "penalty_NoCycleway_HighwayTertiary" );
    penalty.setAttribute("value", locale.toString(penalty_NoCycleway_HighwayTertiary, 'f', 2));
    root.appendChild(penalty);
    penalty = doc.createElement( "zoom" );
    penalty.setAttribute("value", locale.toString(zoom));
    root.appendChild(penalty);
	
	QFile file(filename);
	if( !file.open( QIODevice::WriteOnly ) )
	{
		qDebug() << QString("Opening ") + filename + " file for writing failed.";
		throw std::ios_base::failure("Opening settings.xml file for writing failed.");
	}
	
	QTextStream ts( &file );
	ts << doc.toString();
	file.close();
}


Settings::Settings() :
        penalty_Altitude(20),
        penalty_NoCycleway(10),
        penalty_NoCycleway_HighwayPrimary(30),
        penalty_NoCycleway_HighwaySecondary(20),
        penalty_NoCycleway_HighwayTertiary(15),
        penalty_NoCycleway_HighwayPath(20),
        penalty_NoCycleway_Dismount(300),
        penalty_Cycleway_HighwayPrimary(10),
        penalty_Cycleway_HighwaySecondary(5),
        penalty_Cycleway_HighwayTertiary(0),
        penalty_Cycleway_HighwayPath(10),
        penalty_Highway_Trafficlights(60),
        penalty_Barrier_Cycle(50),
        zoom(13)
{
	//loadXML();
}
Settings::~Settings()
{
	//saveXML();
}

QString Settings::getLastOpenedDBFilename()
{
	return standardDBFilename;
}

GPSPosition Settings::getStandardPosition()
{
	return standardPosition;
}

void Settings::setStandardPosition(GPSPosition pos)
{
	standardPosition = pos;
}
void Settings::setLastOpenedDBFilename(QString filename)
{
	standardDBFilename = filename;
}

float Settings::getPenalty_Altitude()                     {return penalty_Altitude;}
float Settings::getPenalty_NoCycleway()                   {return penalty_NoCycleway;}
float Settings::getPenalty_NoCycleway_HighwayPrimary()    {return penalty_NoCycleway_HighwayPrimary;}
float Settings::getPenalty_NoCycleway_HighwaySecondary()  {return penalty_NoCycleway_HighwaySecondary;}
float Settings::getPenalty_NoCycleway_HighwayTertiary()   {return penalty_NoCycleway_HighwayTertiary;}
float Settings::getPenalty_NoCycleway_HighwayPath()       {return penalty_NoCycleway_HighwayPath;}
float Settings::getPenalty_NoCycleway_Dismount()          {return penalty_NoCycleway_Dismount;}
float Settings::getPenalty_Cycleway_HighwayPrimary()      {return penalty_Cycleway_HighwayPrimary;}
float Settings::getPenalty_Cycleway_HighwaySecondary()    {return penalty_Cycleway_HighwaySecondary;}
float Settings::getPenalty_Cycleway_HighwayTertiary()     {return penalty_Cycleway_HighwayTertiary;}
float Settings::getPenalty_Cycleway_HighwayPath()         {return penalty_Cycleway_HighwayPath;}
float Settings::getPenalty_Highway_Trafficlights()        {return penalty_Highway_Trafficlights;}
float Settings::getPenalty_Barrier_Cycle()                {return penalty_Barrier_Cycle;}

void Settings::setPenalty_Altitude(float p)                     {penalty_Altitude=p;}
void Settings::setPenalty_NoCycleway(float p)                   {penalty_NoCycleway=p;}
void Settings::setPenalty_NoCycleway_HighwayPrimary(float p)    {penalty_NoCycleway_HighwayPrimary=p;}
void Settings::setPenalty_NoCycleway_HighwaySecondary(float p)  {penalty_NoCycleway_HighwaySecondary=p;}
void Settings::setPenalty_NoCycleway_HighwayTertiary(float p)   {penalty_NoCycleway_HighwayTertiary=p;}
void Settings::setPenalty_NoCycleway_HighwayPath(float p)       {penalty_NoCycleway_HighwayPath=p;}
void Settings::setPenalty_NoCycleway_Dismount(float p)          {penalty_NoCycleway_Dismount=p;}
void Settings::setPenalty_Cycleway_HighwayPrimary(float p)      {penalty_Cycleway_HighwayPrimary=p;}
void Settings::setPenalty_Cycleway_HighwaySecondary(float p)    {penalty_Cycleway_HighwaySecondary=p;}
void Settings::setPenalty_Cycleway_HighwayTertiary(float p)     {penalty_Cycleway_HighwayTertiary=p;}
void Settings::setPenalty_Cycleway_HighwayPath(float p)         {penalty_Cycleway_HighwayPath=p;}
void Settings::setPenalty_Highway_Trafficlights(float p)        {penalty_Highway_Trafficlights=p;}
void Settings::setPenalty_Barrier_Cycle(float p)                {penalty_Barrier_Cycle=p;}

void Settings::setZoom(int zoom)                                {this->zoom = zoom;}
int Settings::getZoom()                                         {return zoom;}
