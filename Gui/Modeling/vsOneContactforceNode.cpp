#include "vsOneContactForceNode.h"

vsOneContactForceNode::vsOneContactForceNode(OpenSim::Force *contactForce,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(contactForce,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/contactNode.png";
}
