#ifndef GROUNDNODE_H
#define GROUNDNODE_H

#include "oneframenode.h"



class GroundNode : public OneFrameNode
{
    Q_OBJECT
public:
    GroundNode(OpenSim::Ground *gnd,NavigatorNode *parentNode,QObject *parent);
};

#endif // GROUNDNODE_H
