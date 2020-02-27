#ifndef vsJointsNode_H
#define vsJointsNode_H

#include "vsNavigatorNode.h"



class vsJointsNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsJointsNode(OpenSim::JointSet *jointSet,vsNavigatorNode* parentNode,QObject *parent);
};

#endif // vsJointsNode_H
