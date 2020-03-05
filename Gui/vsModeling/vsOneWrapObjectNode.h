#ifndef vsOneWrapObjectNode_H
#define vsOneWrapObjectNode_H

#include "vsNavigatorNode.h"



class vsOneWrapObjectNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneWrapObjectNode(OpenSim::WrapObject *wrapObj,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsOneWrapObjectNode_H
