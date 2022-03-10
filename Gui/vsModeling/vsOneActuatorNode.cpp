#include "vsOneActuatorNode.h"

vsOneActuatorNode::vsOneActuatorNode(OpenSim::Actuator *actuator,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(actuator,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/muscleNode.png";
}
