#include "vsActuatorsNode.h"
#include "vsOneActuatorNode.h"

vsActuatorsNode::vsActuatorsNode(OpenSim::ForceSet *forceSet,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(nullptr,"Actuators",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/vsActuatorsNode.png";
    //loading actuators
    for (int i = 0; i < forceSet->updActuators().getSize(); ++i) {
        OpenSim::Actuator *actuator = &forceSet->updActuators().get(i);
        vsOneActuatorNode *actuatorNode =  new vsOneActuatorNode(actuator,this,this);
    }
}
