#include "onecontactforcenode.h"

OneContactForceNode::OneContactForceNode(OpenSim::Force *contactForce,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(contactForce,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/contactNode.png";
}
