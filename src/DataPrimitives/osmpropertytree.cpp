#include "osmpropertytree.hpp"

#include <QList>
#include "src/DataPrimitives/osmproperty.hpp"

OSMPropertyTree* OSMPropertyTree::convertToOrPropertyTree(QList<OSMProperty> propList)
{
    if (propList.size() > 1)
    {
        OSMProperty prop1 = propList.last();
        propList.removeLast();
        OSMProperty prop2 = propList.last();
        propList.removeLast();

        OSMPropertyTreeBinaryOrNode* node = new OSMPropertyTreeBinaryOrNode(new OSMPropertyTreePropertyNode(prop1), new OSMPropertyTreePropertyNode(prop2));
        while (propList.size() >= 1)
        {
            OSMPropertyTreeBinaryOrNode* newNode = new OSMPropertyTreeBinaryOrNode(new OSMPropertyTreePropertyNode(propList.last()), node);
            propList.removeLast();
            node=newNode;
        }
        return node;
    }
    else if (propList.size() == 1)
    {
        return new OSMPropertyTreePropertyNode(propList[0]);
    }
    else
        return 0;
}
