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

#include "spacefillingcurves.hpp"

uint32_t ZOrderCurve::getBucketID(uint16_t x, uint16_t y, int iterationCount)
{
	/*
	 * Die folgende Zeile macht:
	 * **  Zuerst werden x und y um die rechts stehenden Bits beschnitten, je nachdem wie viele
	 *     "Iterationen" gemacht werden sollen (d.h., wieviele Buckets entstehen sollen).
	 * **  Dann werden die Bits "gespreizt", d.h. dass z.B. aus 1001 1_0_0_1_ wird, wobei an
	 *     den "_"-Positionen Nullen stehen müssen.
	 * **  Die y-Koordinaten werden dann um eins nach links geschoben und mit den x-Koordinaten
	 *     vermengt. Fertig sind die Z-Order-Buckets.
	 */
	return (spreadInteger(x >> (16-iterationCount)) << 1) | (spreadInteger(y >> (16-iterationCount)));
}
uint32_t ZOrderCurve::spreadInteger(uint32_t number)
{
	uint32_t retVal = 0;
	for (int i=0; i<16; i++)
	{
		//Diese Zeile schaut, was sich an Position i von number befindet, und schreibt das an Position 2*i von retVal.
		retVal |= (((number << (15-i)) & (1 << 15)) == (1 << 15)) ? (1 << (i*2)) : 0;
	}
	return retVal;
}
QList<Interval<uint32_t> > ZOrderCurve::getRectangleIndexList(uint16_t upperLeftX,
			uint16_t upperLeftY, uint16_t bottomRightX, uint16_t bottomRightY,
			int iterationCount)
{
	QList<Interval<uint32_t> > list;
	//TODO: Das ist erstmal nur, damit ich bei der Datenbank weiterbauen kann.
	list << Interval<uint32_t>(getBucketID(upperLeftX, upperLeftY, iterationCount), getBucketID(bottomRightX, bottomRightY, iterationCount));
	return list;
}

