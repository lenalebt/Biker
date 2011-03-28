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
};

#endif // MAINWINDOW_HPP
