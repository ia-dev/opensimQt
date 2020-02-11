#include "onejointnode.h"

OneJointNode::OneJointNode(OpenSim::Joint *joint,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(joint,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/jointNode.png";
}
