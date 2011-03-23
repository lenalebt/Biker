#############################################################################
# Makefile for building: Biker
# Generated by qmake (2.01a) (Qt 4.7.0) on: Mi. M�r 23 15:13:04 2011
# Project:  Biker.pro
# Template: app
# Command: /usr/bin/qmake -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile Biker.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4 -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib -lQtXml -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = src/Database/osmdatabase.cpp \
		src/DataPrimitives/osmproperty.cpp \
		src/main.cpp \
		src/DataPrimitives/gpsposition.cpp \
		src/DataPrimitives/osmnode.cpp \
		src/DataPrimitives/osmedge.cpp \
		src/DataPrimitives/osmway.cpp \
		src/DataPrimitives/gpsroute.cpp \
		src/DataPrimitives/osmpropertytree.cpp 
OBJECTS       = osmdatabase.o \
		osmproperty.o \
		main.o \
		gpsposition.o \
		osmnode.o \
		osmedge.o \
		osmway.o \
		gpsroute.o \
		osmpropertytree.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		Biker.pro
QMAKE_TARGET  = Biker
DESTDIR       = 
TARGET        = Biker

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: Biker.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/libQtXml.prl \
		/usr/lib/libQtGui.prl \
		/usr/lib/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile Biker.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/libQtXml.prl:
/usr/lib/libQtGui.prl:
/usr/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile Biker.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/Biker1.0.0 || $(MKDIR) .tmp/Biker1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/Biker1.0.0/ && $(COPY_FILE) --parents src/Database/osmdatabase.hpp src/DataPrimitives/osmproperty.hpp src/DataPrimitives/DataPrimitives.hpp src/DataPrimitives/gpsposition.hpp src/DataPrimitives/osmnode.hpp src/DataPrimitives/DataPrimitiveDefines.hpp src/DataPrimitives/osmedge.hpp src/DataPrimitives/osmway.hpp src/DataPrimitives/gpsroute.hpp src/DataPrimitives/osmpropertytree.hpp .tmp/Biker1.0.0/ && $(COPY_FILE) --parents src/Database/osmdatabase.cpp src/DataPrimitives/osmproperty.cpp src/main.cpp src/DataPrimitives/gpsposition.cpp src/DataPrimitives/osmnode.cpp src/DataPrimitives/osmedge.cpp src/DataPrimitives/osmway.cpp src/DataPrimitives/gpsroute.cpp src/DataPrimitives/osmpropertytree.cpp .tmp/Biker1.0.0/ && (cd `dirname .tmp/Biker1.0.0` && $(TAR) Biker1.0.0.tar Biker1.0.0 && $(COMPRESS) Biker1.0.0.tar) && $(MOVE) `dirname .tmp/Biker1.0.0`/Biker1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/Biker1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

osmdatabase.o: src/Database/osmdatabase.cpp src/Database/osmdatabase.hpp \
		src/DataPrimitives/DataPrimitives.hpp \
		src/DataPrimitives/osmproperty.hpp \
		src/DataPrimitives/DataPrimitiveDefines.hpp \
		src/DataPrimitives/gpsposition.hpp \
		src/DataPrimitives/gpsroute.hpp \
		src/DataPrimitives/osmnode.hpp \
		src/DataPrimitives/osmedge.hpp \
		src/DataPrimitives/osmway.hpp \
		src/DataPrimitives/osmpropertytree.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o osmdatabase.o src/Database/osmdatabase.cpp

osmproperty.o: src/DataPrimitives/osmproperty.cpp src/DataPrimitives/osmproperty.hpp \
		src/DataPrimitives/DataPrimitiveDefines.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o osmproperty.o src/DataPrimitives/osmproperty.cpp

main.o: src/main.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o src/main.cpp

gpsposition.o: src/DataPrimitives/gpsposition.cpp src/DataPrimitives/gpsposition.hpp \
		src/DataPrimitives/DataPrimitiveDefines.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gpsposition.o src/DataPrimitives/gpsposition.cpp

osmnode.o: src/DataPrimitives/osmnode.cpp src/DataPrimitives/osmnode.hpp \
		src/DataPrimitives/DataPrimitiveDefines.hpp \
		src/DataPrimitives/gpsposition.hpp \
		src/DataPrimitives/osmproperty.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o osmnode.o src/DataPrimitives/osmnode.cpp

osmedge.o: src/DataPrimitives/osmedge.cpp src/DataPrimitives/osmedge.hpp \
		src/DataPrimitives/DataPrimitiveDefines.hpp \
		src/DataPrimitives/osmproperty.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o osmedge.o src/DataPrimitives/osmedge.cpp

osmway.o: src/DataPrimitives/osmway.cpp src/DataPrimitives/osmway.hpp \
		src/DataPrimitives/DataPrimitiveDefines.hpp \
		src/DataPrimitives/osmproperty.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o osmway.o src/DataPrimitives/osmway.cpp

gpsroute.o: src/DataPrimitives/gpsroute.cpp src/DataPrimitives/gpsroute.hpp \
		src/DataPrimitives/DataPrimitiveDefines.hpp \
		src/DataPrimitives/gpsposition.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gpsroute.o src/DataPrimitives/gpsroute.cpp

osmpropertytree.o: src/DataPrimitives/osmpropertytree.cpp src/DataPrimitives/osmpropertytree.hpp \
		src/DataPrimitives/DataPrimitiveDefines.hpp \
		src/DataPrimitives/osmproperty.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o osmpropertytree.o src/DataPrimitives/osmpropertytree.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:
