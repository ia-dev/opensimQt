#ifndef vsOneOtherForceNode_H
#define vsOneOtherForceNode_H

#include "vsNavigatorNode.h"



class vsOneOtherForceNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneOtherForceNode(OpenSim::Force *otherForce,vsNavigatorNode *parentNode, QObject *parent);
};

#endif // vsOneOtherForceNode_H
