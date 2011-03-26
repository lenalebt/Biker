#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "src/Gui/QMapControl/qmapcontrol.h"
#include <iostream>
#include "src/Routing/astar.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    routeLayer(0),
    poiLayer(0),
    dbreader(0)
{
    ui->setupUi(this);
    mapcontrol = new qmapcontrol::MapControl(QSize(1024, 768));
    mapcontrol->showScale(true);

    // create mapadapter, for mainlayer and overlay
    mapadapter = new qmapcontrol::OSMMapAdapter();

    // create a layer with the mapadapter and type MapLayer
    mainlayer = new qmapcontrol::MapLayer("OpenStreetMap-Layer", mapadapter);

    // add Layer to the MapControl
    mapcontrol->addLayer(mainlayer);
    mapcontrol->enablePersistentCache(QDir("./data/tiles"));
    
    mapcontrol->setMaximumSize(1680, 1050);
    mapcontrol->setZoom(11);
    QList<QPointF> view;
    view.append(QPointF(7.48, 51.356));
    view.append(QPointF(7.49, 51.357));
    mapcontrol->setView(view);
    
    layout = new QHBoxLayout(ui->mapwidget);
    layout->addWidget(mapcontrol);
    
    mapcontrol->resize(QSize(ui->mapwidget->size().width()-20, ui->mapwidget->size().height()-20));
    
    connect(mapcontrol, SIGNAL(mouseEventCoordinate (const QMouseEvent*, const QPointF)), this, SLOT(mouseEventCoordinate (const QMouseEvent*, const QPointF)));
    
    //Menü
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(menuCloseClicked()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(menuOpenClicked()));
    connect(ui->actionOpenRoute, SIGNAL(triggered()), this, SLOT(openRoute()));
    connect(ui->actionSaveRoute, SIGNAL(triggered()), this, SLOT(saveRoute()));
    connect(ui->actionCamping, SIGNAL(toggled(bool)), this, SLOT(showCampingPOIs(bool)));
    
    //Weiterschalten der Seitenleiste
    connect(ui->changeOptionPageL_1, SIGNAL(clicked()), this, SLOT(changeOptionPageL()));
    connect(ui->changeOptionPageR_1, SIGNAL(clicked()), this, SLOT(changeOptionPageR()));
    connect(ui->changeOptionPageL_2, SIGNAL(clicked()), this, SLOT(changeOptionPageL()));
    connect(ui->changeOptionPageR_2, SIGNAL(clicked()), this, SLOT(changeOptionPageR()));
    
    //Kram auf Seite1
    connect(ui->resetRoute, SIGNAL(clicked()), this, SLOT(resetRoute()));
    connect(ui->removeLastWaypoint, SIGNAL(clicked()), this, SLOT(removeLastWaypoint()));
    
    //Kram auf Seite2
}

MainWindow::~MainWindow()
{
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
    else
    {
        GPSPosition releasePos;
        releasePos.setLat(evnt->posF().x());
        releasePos.setLon(evnt->posF().y());
        dragged = !(clickPos == releasePos);
    }
    
    GPSPosition actPos = GPSPosition(coordinate.x(), coordinate.y());
    
    if ((evnt->button() == Qt::LeftButton) && (evnt->type() == QEvent::MouseButtonRelease))
    {
        if (!dragged && dbreader->isOpen())
        {
            waypointList << actPos;
            calcRouteSection();
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
        dbreader = new OSMInMemoryDatabase();
        dbreader->openDatabase(filename);
        //QMessageBox msgBox; msgBox.setText(QString::fromUtf8("Datenbank geöffnet.")); msgBox.exec();
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
    
    if (!routes.isEmpty())
    {
        for (QList<GPSRoute>::iterator it = routes.begin(); it < routes.end(); it++)
        {
            for (int i=0; i<it->size(); i++)
            {
                points.append(new qmapcontrol::Point(it->getWaypoint(i).getLon(), it->getWaypoint(i).getLat(), ""));
            }
            points.append(new qmapcontrol::ImagePoint(it->getWaypoint(it->size()-1).getLon(), it->getWaypoint(it->size()-1).getLat(), "images/marker-red.png", "", qmapcontrol::Point::Middle));
        }
    }

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

void MainWindow::calcRouteSection()
{
    if (waypointList.size()>1 && dbreader->isOpen())
    {
        AStar astar(dbreader, new BikeMetric(dbreader, ui->altitudePenalty->value()), new BinaryHeap<AStarRoutingNode>(), new HashClosedList());
        GPSRoute newRouteSection = astar.calcShortestRoute(waypointList[waypointList.size()-2], waypointList[waypointList.size()-1]);
        routeSections << newRouteSection;
    }
    showRoute(routeSections);
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
        poiLayer = new qmapcontrol::MapLayer("POIs", mapadapter);
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
    if (show && dbreader->isOpen())
    {
        OSMProperty camp_site("amenity", "recycling");
        OSMPropertyTree* tree = new OSMPropertyTreePropertyNode(camp_site);
        GPSPosition pos = GPSPosition(mapcontrol->currentCoordinate().x(), mapcontrol->currentCoordinate().y());
        poiList = dbreader->getNodes(pos, 4000.0, *tree);
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
