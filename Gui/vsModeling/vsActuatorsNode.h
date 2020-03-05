#ifndef vsActuatorsNode_H
#define vsActuatorsNode_H

#include "vsNavigatorNode.h"



class vsActuatorsNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsActuatorsNode(OpenSim::ForceSet *forceSet,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsActuatorsNode_H
