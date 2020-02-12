#include "jointsnode.h"
#include "onejointnode.h"

JointsNode::JointsNode(OpenSim::JointSet *jointSet,NavigatorNode* parentNode,QObject *parent):
    NavigatorNode(jointSet,"Joints",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/jointsNode.png";

    //loading joints
    for (int i = 0; i < jointSet->getSize(); ++i) {
        OpenSim::Joint *joint = &jointSet->get(i);
        OneJointNode *jointNode = new OneJointNode(joint,this,this);
    }
}
