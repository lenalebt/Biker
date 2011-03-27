#ifndef ELEVATIONPROFILEWIDGET_HPP
#define ELEVATIONPROFILEWIDGET_HPP

#include <QtGui>
#include "src/Database/osmdatabase.hpp"

namespace Ui {
    class ElevationProfileWidget;
}

class ElevationProfileWidget : public QWidget {
    Q_OBJECT
public:
    ElevationProfileWidget(OSMDatabaseReader* dbreader, QWidget *parent = 0);
    ~ElevationProfileWidget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ElevationProfileWidget *ui;
    OSMDatabaseReader* dbreader;
};


#endif // ELEVATIONPROFILEWIDGET_HPP
