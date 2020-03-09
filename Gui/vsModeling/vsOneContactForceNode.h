#ifndef vsOneContactForceNode_H
#define vsOneContactForceNode_H

#include "vsNavigatorNode.h"



class vsOneContactForceNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneContactForceNode(OpenSim::Force *contactForce,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsOneContactForceNode_H
