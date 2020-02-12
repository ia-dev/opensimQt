#ifndef ONEACTUATORNODE_H
#define ONEACTUATORNODE_H

#include "navigatornode.h"



class OneActuatorNode : public NavigatorNode
{
    Q_OBJECT
public:
    OneActuatorNode(OpenSim::Actuator *actuator,NavigatorNode *parentNode,QObject *parent);
};

#endif // ONEACTUATORNODE_H
