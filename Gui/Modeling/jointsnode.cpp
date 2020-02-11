#include "jointsnode.h"

JointsNode::JointsNode(OpenSim::JointSet *jointSet,NavigatorNode* parentNode,QObject *parent):
    NavigatorNode(jointSet,"Joints",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/jointsNode.png";
}
