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

#include <iostream>
#include "src/Database/Parser/osmparser.hpp"
#include "src/Database/osmdatabase.hpp"
#include "src/Database/osmdatabasewriter.hpp"
#include "src/Routing/routing.hpp"
#include "src/Routing/astar.hpp"
#include "src/Routing/dijkstra.hpp"
#include <boost/shared_ptr.hpp>
#include "src/Gui/mainwindow.hpp"
#include <QtGui>

using namespace std;

// #define TESTPROGRAM
#ifdef TESTPROGRAM
    int main(int argc, char** argv)
    {
        QString filename("data/berlin.osm.pbf");
        OSMInMemoryDatabase db;
        db.openDatabase(filename);
        
        return 0;
    }

#else
    int main(int argc, char** argv)
    {
        //nötig, damit die flyweights nicht abstürzen
        boost::flyweight<QString>::init();
        std::cout << "Warning: Alpha! Uses lots of RAM and might crash from time to time..." << std::endl;
        
        QApplication app(argc, argv);
           
        MainWindow mainWindow;
        mainWindow.show();
        return app.exec();
    }
#endif
