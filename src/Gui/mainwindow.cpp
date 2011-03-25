#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "src/Gui/QMapControl/qmapcontrol.h"
#include <iostream>
#include "src/Routing/astar.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dbreader(0),
    startPos(),
    endPos()
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
    
    mapcontrol->setMaximumSize(1680, 1050);
    mapcontrol->setZoom(11);
    QList<QPointF> view;
    view.append(QPointF(7.48, 51.356));
    view.append(QPointF(7.49, 51.357));
    mapcontrol->setView(view);
    
    layout = new QHBoxLayout(ui->mapwidget);
    layout->addWidget(mapcontrol);
    
    mapcontrol->resize(ui->mapwidget->size());
    
    connect(mapcontrol, SIGNAL(mouseEventCoordinate (const QMouseEvent*, const QPointF)), this, SLOT(mouseEventCoordinate (const QMouseEvent*, const QPointF)));
    
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(menuCloseClicked()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(menuOpenClicked()));
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
    mapcontrol->resize(QSize(ui->mapwidget->size().width()-20, ui->mapwidget->size().height()-20));
}

void MainWindow::mouseEventCoordinate ( const QMouseEvent* evnt, const QPointF coordinate )
{
    static GPSPosition oldPos;
    static bool setStartingPoint=true;
    
    GPSPosition actPos = GPSPosition(coordinate.x(), coordinate.y());
    if ((evnt->button() == Qt::LeftButton) && (evnt->type() == QEvent::MouseButtonRelease) && (actPos==oldPos))
    {
        if (setStartingPoint)
        {
            setStartingPoint = false;
            startPos = actPos;
        }
        else
        {
            endPos = actPos;
            setStartingPoint = true;
            
            if (dbreader->isOpen())
            {
                //Route berechnen
                qDebug() << "AStar zusammenbauen";
                AStar astar(dbreader, new BikeMetric(dbreader, 10), new BinaryHeap<AStarRoutingNode>(), new HashClosedList());
                qDebug() << "Route berechnen";
                GPSRoute r = astar.calcShortestRoute(startPos, endPos);
                qDebug() << "Route anzeigen";
                showRoute(r);
            }
        }
    }
    oldPos = actPos;
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

void MainWindow::showRoute(GPSRoute r)
{
    qmapcontrol::Layer* l = new qmapcontrol::MapLayer("Route", mapadapter);
    
    mapcontrol->layers().clear();
    mapcontrol->addLayer(l);
    
    // create a LineString
    QList<qmapcontrol::Point*> points;
    
    // "Blind" Points
    qDebug() << "Route length: " << r.calcLength() << " meters";
    for (int i=0; i<r.size(); i++)
    {
        points.append(new qmapcontrol::Point(r.getWaypoint(i).getLon(), r.getWaypoint(i).getLat(), ""));
    }

    // A QPen also can use transparency
    QPen* linepen = new QPen(QColor(0, 0, 255, 100));
    linepen->setWidth(5);
    // Add the Points and the QPen to a LineString 
    qmapcontrol::LineString* ls = new qmapcontrol::LineString(points, "Route", linepen);
    
    // Add the LineString to the layer
    l->addGeometry(ls);
    
    mapcontrol->repaint();
    mapcontrol->updateRequestNew();
}
