#include "vsControlNode.h"

vsControlNode::vsControlNode(OpenSim::Controller *control ,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(control,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/constraintNode.png";
}
