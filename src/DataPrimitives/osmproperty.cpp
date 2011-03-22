#include "osmproperty.hpp"
#include <QRegExp>

/**
 * Vergleicht, ob 2 Property-Elemente als gleich anzusehen sind. Das ist der Fall, wenn:
 *   -Key und Value übereinstimmen
 *   -Keys übereinstimmen, sofern einer der beiden leer ist
 *   -bei der Value sind auch Wildcards erlaubt, um z.B. nach Straßennamenteilen zu suchen etc.
 * @param prop
 * @return
 */
bool OSMProperty::operator==(const OSMProperty& prop) const
{
    if (this->key == prop.key)	//key gleicht: Voraussetzung für die meisten Sachen.
    {
        if (this->value == prop.value || this->value.get().isEmpty() || prop.value.get().isEmpty())
            return true;

        if (this->containsWildcards())
        {	//this->value enthält Wildcards!
            QRegExp regexp(this->value);
            regexp.setPatternSyntax(QRegExp::Wildcard);
            regexp.setCaseSensitivity(Qt::CaseInsensitive);
            return regexp.exactMatch(prop.value);
        }
        else if (prop.containsWildcards())
        {	//prop.value enthält Wildcards!
            QRegExp regexp(prop.value);
            regexp.setPatternSyntax(QRegExp::Wildcard);
            regexp.setCaseSensitivity(Qt::CaseInsensitive);
            return regexp.exactMatch(this->value);
        }
        else
            return false;
    }
    else
        return false;
}
bool OSMProperty::containsWildcards() const
{
        return this->value.get().contains("?") || this->value.get().contains("*");
}

std::size_t hash_value(QString const& b)
{
    return boost::hash_value(b.toStdString());
}
