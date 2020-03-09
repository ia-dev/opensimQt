#ifndef vsContactForcesNode_H
#define vsContactForcesNode_H

#include "vsNavigatorNode.h"



class vsContactForcesNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsContactForcesNode(OpenSim::ForceSet *forceSet,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsContactForcesNode_H
