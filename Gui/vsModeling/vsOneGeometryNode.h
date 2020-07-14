#ifndef vsOneGeometryNode_H
#define vsOneGeometryNode_H

#include "vsNavigatorNode.h"



class vsOneGeometryNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneGeometryNode(OpenSim::Geometry *geometry,vsNavigatorNode *parentNode,QObject *parent);

    // vsNavigatorNode interface
public:
    virtual void setupNodeActions(QMenu *rootMenu) override;
};

#endif // vsOneGeometryNode_H
