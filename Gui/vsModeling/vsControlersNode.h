#ifndef vsControlersNode_H
#define vsControlersNode_H

#include "vsNavigatorNode.h"



class vsControlersNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsControlersNode(OpenSim::ControllerSet *controlSet ,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsControlersNode_H
