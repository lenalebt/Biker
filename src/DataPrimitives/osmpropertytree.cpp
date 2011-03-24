#include "osmpropertytree.hpp"

#include <QList>
#include "src/DataPrimitives/osmproperty.hpp"

boost::shared_ptr<OSMPropertyTree> OSMPropertyTree::convertToOrPropertyTree(QList<OSMProperty> propList)
{
    if (propList.size() > 1)
    {
        OSMProperty prop1 = propList.last();
        propList.removeLast();
        OSMProperty prop2 = propList.last();
        propList.removeLast();

        boost::shared_ptr<OSMPropertyTree> node(new OSMPropertyTreeBinaryOrNode(new OSMPropertyTreePropertyNode(prop1), new OSMPropertyTreePropertyNode(prop2)));
        while (propList.size() >= 1)
        {
            boost::shared_ptr<OSMPropertyTreeBinaryOrNode> newNode(new OSMPropertyTreeBinaryOrNode(boost::shared_ptr<OSMPropertyTree>(new OSMPropertyTreePropertyNode(propList.last())), node));
            propList.removeLast();
            node=newNode;
        }
        return node;
    }
    else if (propList.size() == 1)
    {
        return boost::shared_ptr<OSMPropertyTree>(new OSMPropertyTreePropertyNode(propList[0]));
    }
    else
        return boost::shared_ptr<OSMPropertyTree>();
}
