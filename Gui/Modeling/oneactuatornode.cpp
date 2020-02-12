#include "oneactuatornode.h"

OneActuatorNode::OneActuatorNode(OpenSim::Actuator *actuator,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(actuator,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/muscleNode.png";
}
