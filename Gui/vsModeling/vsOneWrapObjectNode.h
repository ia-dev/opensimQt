#ifndef vsOneWrapObjectNode_H
#define vsOneWrapObjectNode_H

#include "vsNavigatorNode.h"



class vsOneWrapObjectNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneWrapObjectNode(OpenSim::WrapObject *wrapObj,vsNavigatorNode *parentNode,QObject *parent);

    // vsNavigatorNode interface
public:
    virtual void setupNodeActions(QMenu *rootMenu) override;
};

#endif // vsOneWrapObjectNode_H
