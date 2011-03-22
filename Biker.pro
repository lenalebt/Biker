QT += xml

HEADERS += src/Database/osmdatabase.hpp \
    src/DataPrimitives/osmproperty.hpp \
    src/DataPrimitives/DataPrimitives.hpp \
    src/DataPrimitives/gpsposition.hpp \
    src/DataPrimitives/osmnode.hpp \
    src/DataPrimitives/DataPrimitiveDefines.hpp \
    src/DataPrimitives/osmedge.hpp \
    src/DataPrimitives/osmway.hpp \
    src/DataPrimitives/gpsroute.hpp \
    src/DataPrimitives/osmpropertytree.hpp
SOURCES += src/Database/osmdatabase.cpp \
    src/DataPrimitives/osmproperty.cpp \
    src/main.cpp \
    src/DataPrimitives/gpsposition.cpp \
    src/DataPrimitives/osmnode.cpp \
    src/DataPrimitives/osmedge.cpp \
    src/DataPrimitives/osmway.cpp \
    src/DataPrimitives/gpsroute.cpp \
    src/DataPrimitives/osmpropertytree.cpp
