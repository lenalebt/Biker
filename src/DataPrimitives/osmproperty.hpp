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
