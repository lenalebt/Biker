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
    
    QHBoxLayout* layout;
    
    OSMDatabaseReader* dbreader;
    
    void showRoute(QList<GPSRoute> routeSections);
    
    QList<GPSPosition> waypointList;
    QList<GPSRoute> routeSections;
    
    void calcRouteSection();
    
    
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
};

#endif // MAINWINDOW_HPP
