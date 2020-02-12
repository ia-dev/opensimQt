#include "controlnode.h"

ControlNode::ControlNode(OpenSim::Controller *control ,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(control,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/constraintNode.png";
}
