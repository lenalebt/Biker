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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "src/Gui/QMapControl/qmapcontrol.h"
#include "src/Database/osmdatabase.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    

protected:
    void changeEvent(QEvent *e);
    void resizeEvent ( QResizeEvent * event );

private:
    Ui::MainWindow *ui;
    qmapcontrol::MapControl* mapcontrol;
    qmapcontrol::MapAdapter* mapadapter;
    qmapcontrol::Layer* mainlayer;
    qmapcontrol::Layer* routeLayer;
    qmapcontrol::Layer* poiLayer;
    
    QHBoxLayout* layout;
    
    OSMDatabaseReader* dbreader;
    
    void showRoute(QList<GPSRoute> routeSections);
    void showPOIList(QList<boost::shared_ptr<OSMNode> > pois);
    
    QList<GPSPosition> waypointList;
    QList<GPSRoute> routeSections;
    QList<boost::shared_ptr<OSMNode> > poiList;
    
    GPSRoute calcRouteSection(GPSPosition start, GPSPosition end);
    void loadSettings();
    void saveSettings();
    
    
public slots:
    void mouseEventCoordinate ( const QMouseEvent* evnt, const QPointF coordinate );
    void menuOpenClicked();
    void menuOpenAndAddClicked();
    void menuCloseClicked();
    void changeOptionPageL();
    void changeOptionPageR();
    void resetRoute();
    void saveRoute();
    void openRoute();
    void removeLastWaypoint();
    void showCampingPOIs(bool show);
    void showSpecialPOIs(bool show);
    void changeRoutingOptionPage(int index);
    void showElevationProfile();
    void recalculateRoute();
    void recalculateLastStage();
    void reverseRoute();
    void changeMapAdapter(int index);
};

#endif // MAINWINDOW_HPP
