#include "vsOneOtherForceNode.h"

vsOneOtherForceNode::vsOneOtherForceNode(OpenSim::Force *otherForce,vsNavigatorNode *parentNode, QObject *parent):
    vsNavigatorNode(otherForce,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/forceNode.png";

}
