#include "actuatorsnode.h"
#include "oneactuatornode.h"

ActuatorsNode::ActuatorsNode(OpenSim::ForceSet *forceSet,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(nullptr,"Actuators",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/actuatorsNode.png";
    //loading actuators
    for (int i = 0; i < forceSet->updActuators().getSize(); ++i) {
        OpenSim::Actuator *actuator = &forceSet->updActuators().get(i);
        OneActuatorNode *actuatorNode =  new OneActuatorNode(actuator,this,this);
    }
}
