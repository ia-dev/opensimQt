#ifndef vsControlNode_H
#define vsControlNode_H

#include "vsNavigatorNode.h"



class vsControlNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsControlNode(OpenSim::Controller *control ,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsControlNode_H
