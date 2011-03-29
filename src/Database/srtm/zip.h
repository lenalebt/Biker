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
    
    This file was originally taken from Hermann Kraus (c) 2009 and was available
    under a "MIT Style" license, see COPYING in this folder.
*/ 


/** \file
    Read SRTM zip files. */
#ifndef __SRTM_ZIP_H__
#define __SRTM_ZIP_H__

#include <QString>

/** Helper class to read SRTM zip files. */
class SrtmZipFile
{
    public:
        static int getData(QString filename, qint16 **buffer);
};

#endif
