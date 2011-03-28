#ifndef ELEVATIONPROFILEDIALOG_HPP
#define ELEVATIONPROFILEDIALOG_HPP

#include <QDialog>
#include <QList>
#include "src/Database/osmdatabase.hpp"

namespace Ui {
    class ElevationProfileDialog;
}

class ElevationProfileDialog : public QDialog {
    Q_OBJECT
public:
    ElevationProfileDialog(OSMDatabaseReader* reader, QList<GPSRoute> routes, QWidget *parent = 0);
    ~ElevationProfileDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ElevationProfileDialog *ui;
    OSMDatabaseReader* dbreader;
    QList<GPSRoute> routes;
};

#endif // ELEVATIONPROFILEDIALOG_HPP
