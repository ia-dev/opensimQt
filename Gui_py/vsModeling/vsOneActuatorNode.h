#ifndef vsOneActuatorNode_H
#define vsOneActuatorNode_H

#include "vsNavigatorNode.h"



class vsOneActuatorNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneActuatorNode(OpenSim::Actuator *actuator,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsOneActuatorNode_H
