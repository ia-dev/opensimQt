#ifndef vsProbesNode_H
#define vsProbesNode_H

#include "vsNavigatorNode.h"



class vsProbesNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsProbesNode(OpenSim::ProbeSet *probSet,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsProbesNode_H
