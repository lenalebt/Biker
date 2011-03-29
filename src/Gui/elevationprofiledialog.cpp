#include "elevationprofiledialog.hpp"
#include "ui_elevationprofiledialog.h"
#include <qwt_plot_curve.h>
#include <QDebug>

ElevationProfileDialog::ElevationProfileDialog(OSMDatabaseReader *reader, QList<GPSRoute> routes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ElevationProfileDialog),
    dbreader(reader),
    routes(routes)
{
    ui->setupUi(this);
    
    ui->qwtPlot->setAxisTitle(2, "Distance (km)");
    ui->qwtPlot->setAxisTitle(0,   "Height (m)");
    
    GPSRoute route;
    for (QList<GPSRoute>::iterator it = routes.begin(); it < routes.end(); it++)
        route.addRoute(*it);
    
    QwtPlotCurve* curve = new QwtPlotCurve("Route 1");
    QwtArray<double> xdata;
    QwtArray<double> ydata;
    
    double routeLength = route.calcLength();
    double height=0, oldHeight=-5000.0, heightDiff=0, ascent=0, descent=0, minHeight=32000, maxHeight=-32000;
    
    for (int i=0; i<route.size(); i++)
    {
        xdata.append((routeLength - route.calcLengthStartingAtID(i))/1000.0);
        height = reader->getAltitude(route.getWaypoint(i).getLon(), route.getWaypoint(i).getLat());
        ydata.append(height);
        
        if (height < minHeight)
            minHeight = height;
        if (height > maxHeight)
            maxHeight = height;
        
        heightDiff = ((oldHeight!=-5000.0) ? (height-oldHeight) : 0.0);
        ascent +=  (heightDiff > 0.0 ? heightDiff : 0.0);
        descent += (heightDiff < 0.0 ? -heightDiff : 0.0);
        oldHeight = height;
    }
    curve->setData(xdata, ydata);
    
    QLocale loc = QLocale(QLocale::C);
    ui->lblAscent->setText(loc.toString(ascent, 'f', 0) + " m");
    ui->lblDescent->setText(loc.toString(descent, 'f', 0) + " m");
    ui->lblMinimum->setText(loc.toString(minHeight, 'f', 0) + " m");
    ui->lblMaximum->setText(loc.toString(maxHeight, 'f', 0) + " m");
    ui->lblHeightDifferenceMinMax->setText(loc.toString(maxHeight-minHeight, 'f', 0) + " m");
    ui->lblDistance->setText(loc.toString(routeLength/1000.0, 'f', 2) + " km");
    curve->attach(ui->qwtPlot);
    ui->qwtPlot->replot();
    
}

ElevationProfileDialog::~ElevationProfileDialog()
{
    delete ui;
}

void ElevationProfileDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
