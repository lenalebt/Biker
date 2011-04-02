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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "src/Gui/QMapControl/qmapcontrol.h"
#include <iostream>
#include "src/Routing/astar.hpp"
#include "src/Toolbox/Settings.hpp"
#include "src/Gui/elevationprofiledialog.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    routeLayer(0),
    poiLayer(0),
    dbreader(0)
{
    ui->setupUi(this);
    setWindowTitle("Biker");
    
    mapcontrol = new qmapcontrol::MapControl(QSize(1024, 768));
    mapcontrol->showScale(true);

    // create mapadapter, for mainlayer and overlay
    mapadapter = new qmapcontrol::OSMMapAdapter();

    // create a layer with the mapadapter and type MapLayer
    mainlayer = new qmapcontrol::MapLayer("OpenStreetMap-Layer", mapadapter);

    // add Layer to the MapControl
    mapcontrol->addLayer(mainlayer);
    mapcontrol->enablePersistentCache(QDir(QDir::homePath() + "/.biker/data/tiles/osmmapnik/"));
    
    mapcontrol->setMaximumSize(1680, 1050);
    mapcontrol->setZoom(11);
    
    layout = new QHBoxLayout(ui->mapwidget);
    layout->addWidget(mapcontrol);
    
    mapcontrol->resize(QSize(ui->mapwidget->size().width()-20, ui->mapwidget->size().height()-20));
    
    connect(mapcontrol, SIGNAL(mouseEventCoordinate (const QMouseEvent*, const QPointF)), this, SLOT(mouseEventCoordinate (const QMouseEvent*, const QPointF)));
    
    //Menü
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(menuCloseClicked()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(menuOpenClicked()));
    connect(ui->actionOpenAndAdd, SIGNAL(triggered()), this, SLOT(menuOpenAndAddClicked()));
    connect(ui->actionOpenRoute, SIGNAL(triggered()), this, SLOT(openRoute()));
    connect(ui->actionSaveRoute, SIGNAL(triggered()), this, SLOT(saveRoute()));
    connect(ui->actionCamping, SIGNAL(toggled(bool)), this, SLOT(showCampingPOIs(bool)));
    connect(ui->actionShowSpecialPOI, SIGNAL(toggled(bool)), this, SLOT(showSpecialPOIs(bool)));
    connect(ui->actionShowElevationProfile, SIGNAL(triggered()), this, SLOT(showElevationProfile()));
    connect(ui->actionRemoveLastWaypoint, SIGNAL(triggered()), this, SLOT(removeLastWaypoint()));
    connect(ui->actionResetRoute, SIGNAL(triggered()), this, SLOT(resetRoute()));
    connect(ui->actionRecalculateRoute, SIGNAL(triggered()), this, SLOT(recalculateRoute()));
    connect(ui->actionReverseRoute, SIGNAL(triggered()), this, SLOT(reverseRoute()));
    connect(ui->actionRecalculateLastStage, SIGNAL(triggered()), this, SLOT(recalculateLastStage()));
    
    //Weiterschalten der Seitenleiste
    connect(ui->butChangeOptionPageL_1, SIGNAL(clicked()), this, SLOT(changeOptionPageL()));
    connect(ui->butChangeOptionPageR_1, SIGNAL(clicked()), this, SLOT(changeOptionPageR()));
    connect(ui->butChangeOptionPageL_2, SIGNAL(clicked()), this, SLOT(changeOptionPageL()));
    connect(ui->butChangeOptionPageR_2, SIGNAL(clicked()), this, SLOT(changeOptionPageR()));
    
    
    //Kram auf Seite1
    connect(ui->butResetRoute, SIGNAL(clicked()), this, SLOT(resetRoute()));
    connect(ui->butRemoveLastWaypoint, SIGNAL(clicked()), this, SLOT(removeLastWaypoint()));
    connect(ui->butReverseRoute, SIGNAL(clicked()), this, SLOT(reverseRoute()));
    connect(ui->butRecalculateLastStage, SIGNAL(clicked()), this, SLOT(recalculateLastStage()));
    connect(ui->cmbMapType, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMapAdapter(int)));

    //Kram auf Seite2
    connect(ui->cmbRoutingMetric, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRoutingOptionPage(int)));
    connect(ui->butRecalculateRoute, SIGNAL(clicked()), this, SLOT(recalculateRoute()));
    
    //Einstellungen laden
    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
    if (dbreader != 0)
        dbreader->closeDatabase();
    delete dbreader;
}

void MainWindow::resizeEvent ( QResizeEvent * /*event*/ )
{
    int height, width;
    width = ui->mapwidget->size().width()-20;
    height = ui->mapwidget->size().height()-20;
    if (width < 400) width = 400;
    if (height < 400) height = 400;
    mapcontrol->resize(QSize(width, height));
}

void MainWindow::mouseEventCoordinate ( const QMouseEvent* evnt, const QPointF coordinate )
{
    static GPSPosition clickPos;
    bool dragged = false;
    if ((evnt->button() == Qt::LeftButton) && (evnt->type() == QEvent::MouseButtonPress))
    {
        clickPos.setLat(evnt->posF().x());
        clickPos.setLon(evnt->posF().y());
    }
    else if ((evnt->button() == Qt::LeftButton) && (evnt->type() == QEvent::MouseButtonRelease))
    {
        GPSPosition releasePos;
        releasePos.setLat(evnt->posF().x());
        releasePos.setLon(evnt->posF().y());
        dragged = !(clickPos == releasePos);
    }
    
    GPSPosition actPos = GPSPosition(coordinate.x(), coordinate.y());
    
    if ((evnt->button() == Qt::LeftButton) && (evnt->type() == QEvent::MouseButtonRelease))
    {
        if ((dbreader != 0) && !dragged && dbreader->isOpen())
        {
            waypointList << actPos;
            if (waypointList.size()>1)
            {
                GPSRoute route = calcRouteSection(waypointList[waypointList.size()-2], waypointList[waypointList.size()-1]);
                if (!route.isEmpty())
                {
                    routeSections << route;
                }
                else
                    waypointList.removeLast();
            }
            showRoute(routeSections);
        }
        else if (dbreader == 0 && !dragged)
        {
            QMessageBox msgBox; msgBox.setText(QString::fromUtf8("You need to open a database file to be able to set waypoints.")); msgBox.exec();
        }
    }
}
void MainWindow::menuOpenClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, QString::fromUtf8("Datenbank öffnen"), "", "*.osm");
    if (filename.endsWith(".osm"))
    {
        if (dbreader != 0)
        {
            dbreader->closeDatabase();
            delete dbreader;
        }
        
        QMessageBox msgBox;
        msgBox.setText("Extended parsing?");
        msgBox.setInformativeText("Extended parsing lets you show some more POIs, but it takes longer to load the data file.\n\n Loading OSM files takes some time.");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        
        setWindowTitle("Biker - loading...");
        
        dbreader = new OSMInMemoryDatabase();
        bool success = false;
        if (ret == QMessageBox::Yes)
            success = ((OSMInMemoryDatabase*)dbreader)->openDatabase(filename, true);
        else
            success = ((OSMInMemoryDatabase*)dbreader)->openDatabase(filename, false);
        if (!success)
        {
            setWindowTitle("Biker");
            QMessageBox msgBox; msgBox.setText(QString::fromUtf8("Error while loading database.")); msgBox.exec();
            delete dbreader;
        }
        else
        {
            setWindowTitle(QString("Biker - ") + filename);
        }
    }
}
void MainWindow::menuOpenAndAddClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, QString::fromUtf8("Datenbank öffnen/hinzufügen"), "", "*.osm");
    if (filename.endsWith(".osm"))
    {
        QMessageBox msgBox;
        msgBox.setText("Extended parsing?");
        msgBox.setInformativeText("Extended parsing lets you show some more POIs, but it takes longer to load the data file.\n\n Loading OSM files takes some time.");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        
        setWindowTitle("Biker - loading...");
        
        if (dbreader == 0)
            dbreader = new OSMInMemoryDatabase();
        bool success = false;
        if (ret == QMessageBox::Yes)
            success = ((OSMInMemoryDatabase*)dbreader)->addDatabase(filename, true);
        else
            success = ((OSMInMemoryDatabase*)dbreader)->addDatabase(filename, false);
        if (!success)
        {
            setWindowTitle("Biker");
            QMessageBox msgBox; msgBox.setText(QString::fromUtf8("Error while loading database.")); msgBox.exec();
            delete dbreader;
        }
        else
        {
            setWindowTitle(QString("Biker - ") + filename);
        }
    }
}
void MainWindow::menuCloseClicked()
{
    exit(1);
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::showRoute(QList<GPSRoute> routes)
{
    if (routeLayer == 0)
    {
        routeLayer = new qmapcontrol::MapLayer("Route", mapadapter);
        mapcontrol->addLayer(routeLayer);
    }
    
    routeLayer->clearGeometries();
    
    // create a LineString
    QList<qmapcontrol::Point*> points;
    
    if (!waypointList.isEmpty())
        points.append(new qmapcontrol::ImagePoint(waypointList[0].getLon(), waypointList[0].getLat(), "images/marker-green.png", "", qmapcontrol::Point::Middle));
    
    double routeLength = 0.0;
    if (!routes.isEmpty())
    {
        for (QList<GPSRoute>::iterator it = routes.begin(); it < routes.end(); it++)
        {
            for (int i=0; i<it->size(); i++)
            {
                points.append(new qmapcontrol::Point(it->getWaypoint(i).getLon(), it->getWaypoint(i).getLat(), ""));
            }
            points.append(new qmapcontrol::ImagePoint(it->getWaypoint(it->size()-1).getLon(), it->getWaypoint(it->size()-1).getLat(), "images/marker-red.png", "", qmapcontrol::Point::Middle));
            routeLength += it->calcLength();
        }
    }
    QLocale locale(QLocale::C);
    ui->lblRouteLength->setText(locale.toString(routeLength/1000, 'f', 2 ) + " km");

    // A QPen also can use transparency
    QPen* linepen = new QPen(QColor(0, 0, 255, 100));
    linepen->setWidth(5);
    // Add the Points and the QPen to a LineString 
    qmapcontrol::LineString* ls = new qmapcontrol::LineString(points, "Route", linepen);
    
    // Add the LineString to the layer
    routeLayer->addGeometry(ls);
    
    mapcontrol->repaint();
    mapcontrol->updateRequestNew();
}

void MainWindow::changeOptionPageL()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::changeOptionPageR()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void MainWindow::resetRoute()
{
    routeSections.clear();
    waypointList.clear();
    showRoute(routeSections);
}
void MainWindow::saveRoute()
{
    QString filename = QFileDialog::getSaveFileName(this, QString::fromUtf8("Route speichern"), "", "*.gpx");
    GPSRoute route;
    if (routeSections.size() > 0)
    {
        route = routeSections[0];
        for (QList<GPSRoute>::iterator it = routeSections.begin()+1; it < routeSections.end(); it++)
        {
            route.addRoute(*it);
        }
    }
    GPSRoute::exportGPX(filename, route);
}

void MainWindow::openRoute()
{
    QString filename = QFileDialog::getOpenFileName(this, QString::fromUtf8("Route öffnen"), "", "*.gpx");
    GPSRoute route = GPSRoute::importGPX(filename);
    routeSections << route;
    waypointList << route.getWaypoint(0) << route.getWaypoint(route.size()-1);
    showRoute(routeSections);
}

GPSRoute MainWindow::calcRouteSection(GPSPosition start, GPSPosition end)
{
    if ((dbreader != 0) && dbreader->isOpen())
    {
        AStar* astar;
        if (ui->cmbRoutingMetric->currentIndex() == 0)
        {
            BikeMetric* bm = new BikeMetric(dbreader);
            bm->setPenalty_Altitude(ui->txtAltitudePenalty->value());
            bm->setPenalty_Barrier_Cycle(ui->txtCycleBarrier->value());
            bm->setPenalty_Highway_Trafficlights(ui->txtHighwayTrafficlights->value());
            
            bm->setPenalty_NoCycleway(ui->txtNoCycleway->value() / 100.0);
            bm->setPenalty_NoCycleway_HighwayPath(ui->txtNoCycleway_HighwayPath->value() / 100.0);
            bm->setPenalty_NoCycleway_HighwayPrimary(ui->txtNoCycleway_HighwayPrimary->value() / 100.0);
            bm->setPenalty_NoCycleway_HighwaySecondary(ui->txtNoCycleway_HighwaySecondary->value() / 100.0);
            bm->setPenalty_NoCycleway_HighwayTertiary(ui->txtNoCycleway_HighwayTertiary->value() / 100.0);
            bm->setPenalty_NoCycleway_Dismount(ui->txtNoCycleway_Dismount->value() / 100.0);
            
            bm->setPenalty_Cycleway_HighwayPath(ui->txtCycleway_HighwayPath->value() / 100.0);
            bm->setPenalty_Cycleway_HighwayPrimary(ui->txtCycleway_HighwayPrimary->value() / 100.0);
            bm->setPenalty_Cycleway_HighwaySecondary(ui->txtCycleway_HighwaySecondary->value() / 100.0);
            bm->setPenalty_Cycleway_HighwayTertiary(ui->txtCycleway_HighwayTertiary->value() / 100.0);
            
            astar = new AStar(dbreader, bm, new BinaryHeap<AStarRoutingNode>(), new HashClosedList());
        }
        else if (ui->cmbRoutingMetric->currentIndex() == 1)
        {
            astar = new AStar(dbreader, new EuclidianMetric(), new BinaryHeap<AStarRoutingNode>(), new HashClosedList());
        }
        else if (ui->cmbRoutingMetric->currentIndex() == 2)
        {
            astar = new AStar(dbreader, new CarMetric(), new BinaryHeap<AStarRoutingNode>(), new HashClosedList());
        }
        else if (ui->cmbRoutingMetric->currentIndex() == 3)
        {
            astar = new AStar(dbreader, new FastRoutingMetric(), new BinaryHeap<AStarRoutingNode>(), new HashClosedList());
        }
        else
        {
            astar = new AStar(dbreader, new BikeMetric(dbreader, ui->txtAltitudePenalty->value()), new BinaryHeap<AStarRoutingNode>(), new HashClosedList());
        }
        GPSRoute newRouteSection = astar->calcShortestRoute(start, end);
        delete astar;
        if (!newRouteSection.isEmpty())
            return newRouteSection;
        else
        {
            QMessageBox msgBox; msgBox.setText(QString::fromUtf8("No Route found.")); msgBox.exec();
            return GPSRoute();
        }
    }
    else
        return GPSRoute();
}

void MainWindow::removeLastWaypoint()
{
    if (routeSections.size()>0) routeSections.removeLast();
    if (waypointList.size()>0) waypointList.removeLast();
    showRoute(routeSections);
}

void MainWindow::showPOIList(QList<boost::shared_ptr<OSMNode> > pois)
{
    if (poiLayer == 0)
    {
        poiLayer = new qmapcontrol::GeometryLayer("POIs", mapadapter);
        mapcontrol->addLayer(poiLayer);
    }
    
    poiLayer->clearGeometries();
    
    for (QList<boost::shared_ptr<OSMNode> >::iterator it = pois.begin(); it < pois.end(); it++)
    {
        poiLayer->addGeometry(new qmapcontrol::ImagePoint((*it)->getLon(), (*it)->getLat(), "images/marker-big-green.png", "", qmapcontrol::Point::BottomLeft));
    }
    
    mapcontrol->repaint();
    mapcontrol->updateRequestNew();
}
void MainWindow::showCampingPOIs(bool show)
{
    if ((dbreader != 0) && show && dbreader->isOpen())
    {
        OSMProperty camp_site("tourism", "camp_site");
        OSMPropertyTree* tree = new OSMPropertyTreePropertyNode(camp_site);
        GPSPosition pos = GPSPosition(mapcontrol->currentCoordinate().x(), mapcontrol->currentCoordinate().y());
        poiList = dbreader->getNodes(pos, 20000.0, *tree);
        delete tree;
        qDebug() << "found " << poiList.size() << " points.";
        showPOIList(poiList);
    }
    else
    {
        poiList.clear();
        showPOIList(poiList);
    }
}

void MainWindow::showSpecialPOIs(bool show)
{
    if ((dbreader != 0) && show && dbreader->isOpen())
    {
        bool ok;
        QString key = QInputDialog::getText(this, tr("Enter OSM Key"), tr("OSM key:"), QLineEdit::Normal, "amenity", &ok);
        if (!ok) return;
        QString value = QInputDialog::getText(this, tr("Enter OSM Value"), tr("OSM value:"), QLineEdit::Normal, "post_box", &ok);
        if (!ok) return;
        double radius = QInputDialog::getDouble(this, tr("Enter search radius"), tr("Radius:"), 4000.0, 500, 50000, 0, &ok);
        if (!ok) return;
        
        OSMProperty camp_site(key, value);
        OSMPropertyTree* tree = new OSMPropertyTreePropertyNode(camp_site);
        GPSPosition pos = GPSPosition(mapcontrol->currentCoordinate().x(), mapcontrol->currentCoordinate().y());
        poiList = dbreader->getNodes(pos, radius, *tree);
        delete tree;
        qDebug() << "found " << poiList.size() << " points.";
        showPOIList(poiList);
    }
    else
    {
        poiList.clear();
        showPOIList(poiList);
    }
}

void MainWindow::changeRoutingOptionPage(int index)
{
    ui->stackedRoutingOptionWidget->setCurrentIndex(index);
}

void MainWindow::loadSettings()
{
    Settings* s = Settings::getInstance();
    s->loadXML();
    ui->txtAltitudePenalty->setValue(s->getPenalty_Altitude());
    ui->txtCycleBarrier->setValue(s->getPenalty_Barrier_Cycle());
    ui->txtCycleway_HighwayPath->setValue(s->getPenalty_Cycleway_HighwayPath());
    ui->txtCycleway_HighwayPrimary->setValue(s->getPenalty_Cycleway_HighwayPrimary());
    ui->txtCycleway_HighwaySecondary->setValue(s->getPenalty_Cycleway_HighwaySecondary());
    ui->txtCycleway_HighwayTertiary->setValue(s->getPenalty_Cycleway_HighwayTertiary());
    ui->txtHighwayTrafficlights->setValue(s->getPenalty_Highway_Trafficlights());
    ui->txtNoCycleway->setValue(s->getPenalty_NoCycleway());
    ui->txtNoCycleway_Dismount->setValue(s->getPenalty_NoCycleway_Dismount());
    ui->txtNoCycleway_HighwayPath->setValue(s->getPenalty_NoCycleway_HighwayPath());
    ui->txtNoCycleway_HighwayPrimary->setValue(s->getPenalty_NoCycleway_HighwayPrimary());
    ui->txtNoCycleway_HighwaySecondary->setValue(s->getPenalty_NoCycleway_HighwaySecondary());
    ui->txtNoCycleway_HighwayTertiary->setValue(s->getPenalty_NoCycleway_HighwayTertiary());
    
    QList<QPointF> view;
    view.append(QPointF(s->getStandardPosition().getLon(), s->getStandardPosition().getLat()));
    mapcontrol->setView(view);
    mapcontrol->setZoom(s->getZoom());
}
void MainWindow::saveSettings()
{
    Settings* s = Settings::getInstance();
    s->setPenalty_Altitude(ui->txtAltitudePenalty->value());
    s->setPenalty_Barrier_Cycle(ui->txtCycleBarrier->value());
    s->setPenalty_Cycleway_HighwayPath(ui->txtCycleway_HighwayPath->value());
    s->setPenalty_Cycleway_HighwayPrimary(ui->txtCycleway_HighwayPrimary->value());
    s->setPenalty_Cycleway_HighwaySecondary(ui->txtCycleway_HighwaySecondary->value());
    s->setPenalty_Cycleway_HighwayTertiary(ui->txtCycleway_HighwayTertiary->value());
    s->setPenalty_Highway_Trafficlights(ui->txtHighwayTrafficlights->value());
    s->setPenalty_NoCycleway(ui->txtNoCycleway->value());
    s->setPenalty_NoCycleway_Dismount(ui->txtNoCycleway_Dismount->value());
    s->setPenalty_NoCycleway_HighwayPath(ui->txtNoCycleway_HighwayPath->value());
    s->setPenalty_NoCycleway_HighwayPrimary(ui->txtNoCycleway_HighwayPrimary->value());
    s->setPenalty_NoCycleway_HighwaySecondary(ui->txtNoCycleway_HighwaySecondary->value());
    s->setPenalty_NoCycleway_HighwayTertiary(ui->txtNoCycleway_HighwayTertiary->value());
    s->setStandardPosition(GPSPosition(mapcontrol->currentCoordinate().x(), mapcontrol->currentCoordinate().y()));
    s->setZoom(mapcontrol->currentZoom());
    s->saveXML();
}
void MainWindow::showElevationProfile()
{
    ElevationProfileDialog widget(dbreader, routeSections, this);
    widget.exec();
}

void MainWindow::recalculateRoute()
{
    routeSections.clear();
    for (int i=1; i<waypointList.size(); i++)
    {
        routeSections << calcRouteSection(waypointList[i-1], waypointList[i]);
    }
    showRoute(routeSections);
}

void MainWindow::reverseRoute()
{
    QList<GPSPosition> revList;
    while (!waypointList.isEmpty())
        revList << waypointList.takeLast();
    waypointList = revList;
    recalculateRoute();
}
void MainWindow::recalculateLastStage()
{
    if (routeSections.size() != 0)
    {
        routeSections.removeLast();
        routeSections << calcRouteSection(waypointList[waypointList.size()-2], waypointList[waypointList.size()-1]);
    }
    showRoute(routeSections);
}
void MainWindow::changeMapAdapter(int index)
{
    qmapcontrol::MapAdapter* tmpadapter = mapadapter;

    if (index == 0)
    {//OSM Mapnik
        mapadapter = new qmapcontrol::OSMMapAdapter();
        mapcontrol->enablePersistentCache(QDir(QDir::homePath() + "/.biker/data/tiles/osmmapnik/"));
    } else if (index == 1)
    {//OSM Mapnik
        mapadapter = new qmapcontrol::OpenCycleMapAdapter();
        mapcontrol->enablePersistentCache(QDir(QDir::homePath() + "/.biker/data/tiles/opencyclemap/"));
    } else if (index == 2)
    {//OSM Mapnik
        mapadapter = new qmapcontrol::OEPNVKarteMapAdapter();
        mapcontrol->enablePersistentCache(QDir(QDir::homePath() + "/.biker/data/tiles/oepnvkarte/"));
    } else if (index == 3)
    {//OSM Mapnik
        mapadapter = new qmapcontrol::GoogleMapAdapter();
        mapcontrol->enablePersistentCache(QDir(QDir::homePath() + "/.biker/data/tiles/googlemap/"));
    } else if (index == 4)
    {//OSM Mapnik
        mapadapter = new qmapcontrol::GoogleSatMapAdapter();
        mapcontrol->enablePersistentCache(QDir(QDir::homePath() + "/.biker/data/tiles/googlesat/"));
    }/* else if (index == 5)
    {//OSM Mapnik
        mapadapter = new qmapcontrol::OSMMapAdapter();
    } else if (index == 6)
    {//OSM Mapnik
        mapadapter = new qmapcontrol::OSMMapAdapter();
    } else if (index == 7)
    {//OSM Mapnik
        mapadapter = new qmapcontrol::OSMMapAdapter();
    }*/

    GPSPosition pos(mapcontrol->currentCoordinate().x(), mapcontrol->currentCoordinate().y());
    int zoom(mapcontrol->currentZoom());

    mainlayer->setMapAdapter(mapadapter);
    /*TODO:
        setting zoom is causing the widget to zoom in - step by step.
        this is not okay! (known bug from qmapcontrol). this is a
        bad hack (setting invisible) to speed things up.
      */
    mapcontrol->setVisible(false);
    mapcontrol->setZoom(zoom);
    mapcontrol->setView(QPointF(pos.getLon(), pos.getLat()));
    mapcontrol->setVisible(true);
    delete tmpadapter;
}
