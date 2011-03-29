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

#ifndef OSMPROPERTY_HPP
#define OSMPROPERTY_HPP

#include <boost/flyweight.hpp>
#include "src/DataPrimitives/DataPrimitiveDefines.hpp"
#include <QString>

std::size_t hash_value(QString const& b);

class OSMProperty
{
private:
    boost::flyweight<QString> key;
    boost::flyweight<QString> value;
    
public:
    OSMProperty() : key(""), value("") {}
    OSMProperty(QString key_, QString value_) : key(key_), value(value_) {}
    
    QString getKey() const {return key;}
    QString getValue() const {return value;}
    
    void setKey(QString key) {this->key = key;}
    void setValue(QString value) {this->value = value;}
    
    bool containsWildcards() const;
    
    bool operator==(const OSMProperty& prop) const;
};

#endif // OSMPROPERTY_HPP
