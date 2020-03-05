#ifndef vsOtherForcesNode_H
#define vsOtherForcesNode_H

#include "vsNavigatorNode.h"



class vsOtherForcesNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOtherForcesNode(OpenSim::ForceSet *forceSet,vsNavigatorNode *parentNode, QObject *parent);
};

#endif // vsOtherForcesNode_H
