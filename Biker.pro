QT += xml \
    network
INCLUDEPATH += /usr/include/ \
    /usr/include/qwt5/
LIBS += -L/usr/lib \
    -lcurl \
    -lzzip \
    -lqwt
OBJECTS_DIR = obj
MOC_DIR = obj
UI_DIR = obj
RCC_DIR = obj

# DESTDIR = bin
profiling { 
    QMAKE_CFLAGS += -pg
    QMAKE_CXXFLAGS += -pg
    QMAKE_LFLAGS += -pg
}
HEADERS += src/Database/osmdatabase.hpp \
    src/DataPrimitives/osmproperty.hpp \
    src/DataPrimitives/DataPrimitives.hpp \
    src/DataPrimitives/gpsposition.hpp \
    src/DataPrimitives/osmnode.hpp \
    src/DataPrimitives/DataPrimitiveDefines.hpp \
    src/DataPrimitives/osmedge.hpp \
    src/DataPrimitives/osmway.hpp \
    src/DataPrimitives/gpsroute.hpp \
    src/DataPrimitives/osmpropertytree.hpp \
    src/Database/srtm/srtm.h \
    src/Database/srtm/zip.h \
    src/Database/Parser/osmparser.hpp \
    src/Database/osmdatabasewriter.hpp \
    src/DataPrimitives/osmrelation.hpp \
    src/Routing/routing.hpp \
    src/Routing/metric.hpp \
    src/Routing/dijkstra.hpp \
    src/Routing/astar.hpp \
    src/Database/spacefillingcurves.hpp \
    src/Gui/QMapControl/qmapcontrol.h \
    src/Gui/QMapControl/src/yahoomapadapter.h \
    src/Gui/QMapControl/src/wmsmapadapter.h \
    src/Gui/QMapControl/src/tilemapadapter.h \
    src/Gui/QMapControl/src/point.h \
    src/Gui/QMapControl/src/osmmapadapter.h \
    src/Gui/QMapControl/src/openaerialmapadapter.h \
    src/Gui/QMapControl/src/mapnetwork.h \
    src/Gui/QMapControl/src/maplayer.h \
    src/Gui/QMapControl/src/mapcontrol.h \
    src/Gui/QMapControl/src/mapadapter.h \
    src/Gui/QMapControl/src/linestring.h \
    src/Gui/QMapControl/src/layermanager.h \
    src/Gui/QMapControl/src/layer.h \
    src/Gui/QMapControl/src/imagepoint.h \
    src/Gui/QMapControl/src/imagemanager.h \
    src/Gui/QMapControl/src/gps_position.h \
    src/Gui/QMapControl/src/googlesatmapadapter.h \
    src/Gui/QMapControl/src/googlemapadapter.h \
    src/Gui/QMapControl/src/geometrylayer.h \
    src/Gui/QMapControl/src/geometry.h \
    src/Gui/QMapControl/src/fixedimageoverlay.h \
    src/Gui/QMapControl/src/emptymapadapter.h \
    src/Gui/QMapControl/src/curve.h \
    src/Gui/QMapControl/src/circlepoint.h \
    src/Gui/mainwindow.hpp \
    src/Toolbox/Settings.hpp \
    src/Gui/elevationprofiledialog.hpp \
    src/Gui/QMapControl/src/oepnvkartemapadapter.h \
    src/Gui/QMapControl/src/opencyclemapadapter.h \
    src/Database/Parser/pbfparser.hpp \
    src/Database/Parser/pbf/osmformat.pb.h \
    src/Database/Parser/pbf/fileformat.pb.h
SOURCES += src/Database/osmdatabase.cpp \
    src/DataPrimitives/osmproperty.cpp \
    src/main.cpp \
    src/DataPrimitives/gpsposition.cpp \
    src/DataPrimitives/osmnode.cpp \
    src/DataPrimitives/osmedge.cpp \
    src/DataPrimitives/osmway.cpp \
    src/DataPrimitives/gpsroute.cpp \
    src/DataPrimitives/osmpropertytree.cpp \
    src/Database/srtm/srtm.cpp \
    src/Database/srtm/zip.cpp \
    src/Database/Parser/osmparser.cpp \
    src/Database/osmdatabasewriter.cpp \
    src/DataPrimitives/osmrelation.cpp \
    src/Routing/routing.cpp \
    src/Routing/metric.cpp \
    src/Routing/dijkstra.cpp \
    src/Routing/astar.cpp \
    src/Database/spacefillingcurves.cpp \
    src/Gui/QMapControl/src/yahoomapadapter.cpp \
    src/Gui/QMapControl/src/wmsmapadapter.cpp \
    src/Gui/QMapControl/src/tilemapadapter.cpp \
    src/Gui/QMapControl/src/point.cpp \
    src/Gui/QMapControl/src/osmmapadapter.cpp \
    src/Gui/QMapControl/src/openaerialmapadapter.cpp \
    src/Gui/QMapControl/src/mapnetwork.cpp \
    src/Gui/QMapControl/src/maplayer.cpp \
    src/Gui/QMapControl/src/mapcontrol.cpp \
    src/Gui/QMapControl/src/mapadapter.cpp \
    src/Gui/QMapControl/src/linestring.cpp \
    src/Gui/QMapControl/src/layermanager.cpp \
    src/Gui/QMapControl/src/layer.cpp \
    src/Gui/QMapControl/src/imagepoint.cpp \
    src/Gui/QMapControl/src/imagemanager.cpp \
    src/Gui/QMapControl/src/gps_position.cpp \
    src/Gui/QMapControl/src/googlesatmapadapter.cpp \
    src/Gui/QMapControl/src/googlemapadapter.cpp \
    src/Gui/QMapControl/src/geometrylayer.cpp \
    src/Gui/QMapControl/src/geometry.cpp \
    src/Gui/QMapControl/src/fixedimageoverlay.cpp \
    src/Gui/QMapControl/src/emptymapadapter.cpp \
    src/Gui/QMapControl/src/curve.cpp \
    src/Gui/QMapControl/src/circlepoint.cpp \
    src/Gui/mainwindow.cpp \
    src/Toolbox/Settings.cpp \
    src/Gui/elevationprofiledialog.cpp \
    src/Gui/QMapControl/src/oepnvkartemapadapter.cpp \
    src/Gui/QMapControl/src/opencyclemapadapter.cpp \
    src/Database/Parser/pbfparser.cpp \
    src/Database/Parser/pbf/osmformat.pb.cc \
    src/Database/Parser/pbf/fileformat.pb.cc
FORMS += src/Gui/mainwindow.ui \
    src/Gui/elevationprofiledialog.ui
OTHER_FILES += src/Database/Parser/pbf/osmformat.proto \
    src/Database/Parser/pbf/fileformat.proto
