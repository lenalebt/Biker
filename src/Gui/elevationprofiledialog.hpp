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
