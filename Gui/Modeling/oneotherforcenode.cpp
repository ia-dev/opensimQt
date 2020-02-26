#include "oneotherforcenode.h"

OneOtherForceNode::OneOtherForceNode(OpenSim::Force *otherForce,NavigatorNode *parentNode, QObject *parent):
    NavigatorNode(otherForce,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/forceNode.png";

}
