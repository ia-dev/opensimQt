#ifndef vsGroundNode_H
#define vsGroundNode_H

#include "vsOneFrameNode.h"



class vsGroundNode : public vsOneFrameNode
{
    Q_OBJECT
public:
    vsGroundNode(OpenSim::Ground *gnd,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsGroundNode_H
