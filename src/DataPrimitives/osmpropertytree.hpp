#ifndef OSMPROPERTYTREE_HPP
#define OSMPROPERTYTREE_HPP

#include "src/DataPrimitives/DataPrimitiveDefines.hpp"
#include "src/DataPrimitives/osmproperty.hpp"
#include <boost/shared_ptr.hpp>

class OSMPropertyTree
{
private:
protected:
public:
    /**
     * Ist erfüllt, wenn der Baum bei den aktuell gefundenen Teilelementen
     * @return
     */
    virtual bool evaluate() const=0;
    virtual bool containsWildcards() const=0;
    virtual void propertyFound(const OSMProperty& prop)=0;
    virtual void resetPropertiesFound()=0;
    virtual QString toString()=0;

    virtual ~OSMPropertyTree() {}
    //virtual OSMPropertyTree* clone()=0;

    /**
     * Wandelt eine Eigenschaftenliste um in einen PropertyTree, der mit ODER-
     * Knoten zusammengehalten wird.
     * @param propList
     * @return
     */
    static boost::shared_ptr<OSMPropertyTree> convertToOrPropertyTree(QList<OSMProperty> propList);
};



/**
  * Stellt einen Binärbaum dar.
  */
class OSMPropertyTreeBinaryNode : public OSMPropertyTree
{
protected:
    boost::shared_ptr<OSMPropertyTree> lChild;
    boost::shared_ptr<OSMPropertyTree> rChild;
public:
    OSMPropertyTreeBinaryNode(OSMPropertyTree* lNode, OSMPropertyTree* rNode) : lChild(lNode), rChild(rNode) {}
    OSMPropertyTreeBinaryNode(boost::shared_ptr<OSMPropertyTree> lNode, boost::shared_ptr<OSMPropertyTree> rNode) : lChild(lNode), rChild(rNode) {}
    boost::shared_ptr<OSMPropertyTree> getLChild() {return lChild;}
    boost::shared_ptr<OSMPropertyTree> getRChild() {return rChild;}
    void setLChild(OSMPropertyTree* lNode) {lChild.reset(lNode);}
    void setRChild(OSMPropertyTree* rNode) {rChild.reset(rNode);}
    void setLChild(boost::shared_ptr<OSMPropertyTree> lNode) {lChild = lNode;}
    void setRChild(boost::shared_ptr<OSMPropertyTree> rNode) {rChild = rNode;}
    void propertyFound(const OSMProperty& prop_)
    {
        lChild->propertyFound(prop_);
        rChild->propertyFound(prop_);
    }
    void resetPropertiesFound()
    {
        lChild->resetPropertiesFound();
        rChild->resetPropertiesFound();
    }
    bool containsWildcards() const {return lChild->containsWildcards() || rChild->containsWildcards();}

    ~OSMPropertyTreeBinaryNode()
    {
        lChild.reset();
        rChild.reset();
    }
};

/**
 * Stellt eine Und-Verknüpfung dar.
 */
class OSMPropertyTreeBinaryAndNode : public OSMPropertyTreeBinaryNode
{
protected:
public:
    OSMPropertyTreeBinaryAndNode(OSMPropertyTree* lNode, OSMPropertyTree* rNode) : OSMPropertyTreeBinaryNode(lNode, rNode) {}
    OSMPropertyTreeBinaryAndNode(boost::shared_ptr<OSMPropertyTree> lNode, boost::shared_ptr<OSMPropertyTree> rNode) : OSMPropertyTreeBinaryNode(lNode, rNode) {}
    bool evaluate() const {return lChild->evaluate() && rChild->evaluate();}
    //OSMPropertyTreeBinaryAndNode* clone() {return new OSMPropertyTreeBinaryAndNode(lChild->clone(), rChild->clone());}
    QString toString() {return "(" + lChild->toString() + " AND " + rChild->toString() + ")";}
};

/**
 * Stellt eine Oder-Verknüpfung dar.
 */
class OSMPropertyTreeBinaryOrNode : public OSMPropertyTreeBinaryNode
{
protected:
public:
    OSMPropertyTreeBinaryOrNode(OSMPropertyTree* lNode, OSMPropertyTree* rNode) : OSMPropertyTreeBinaryNode(lNode, rNode) {}
    OSMPropertyTreeBinaryOrNode(boost::shared_ptr<OSMPropertyTree> lNode, boost::shared_ptr<OSMPropertyTree> rNode) : OSMPropertyTreeBinaryNode(lNode, rNode) {}
    bool evaluate() const {return lChild->evaluate() || rChild->evaluate();}
    //OSMPropertyTreeBinaryOrNode* clone() {return new OSMPropertyTreeBinaryOrNode(lChild->clone(), rChild->clone());}
    QString toString() {return "(" + lChild->toString() + " OR " + rChild->toString() + ")";}
};


class OSMPropertyTreePropertyNode : public OSMPropertyTree
{
private:
    bool propertyFound_;
    OSMProperty property;
protected:
public:
    OSMPropertyTreePropertyNode() : propertyFound_(false){}
    OSMPropertyTreePropertyNode(OSMProperty& prop) : propertyFound_(false), property(prop) {}
    bool evaluate() const {return propertyFound_;}
    bool containsWildcards() const {return property.containsWildcards();}
    void propertyFound(const OSMProperty& otherProperty) {propertyFound_ = propertyFound_ || (property == otherProperty);}
    void resetPropertiesFound() {propertyFound_ = false;}

    void setProperty(OSMProperty& prop)
    {
        property = prop;
        propertyFound_ = false;
    }
    OSMProperty& getProperty() {return property;}

    ~OSMPropertyTreePropertyNode() {}
    //OSMPropertyTreePropertyNode* clone() {return new OSMPropertyTreePropertyNode(property);}
    
    QString toString() {return property.getKey() + "=" + property.getValue();}
};

class OSMPropertyTreeAlwaysTrueNode : public OSMPropertyTreePropertyNode
{
private:
protected:
public:
    OSMPropertyTreeAlwaysTrueNode() {}
    bool evaluate() const {return true;}
};


#endif // OSMPROPERTYTREE_HPP
