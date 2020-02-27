#include "vsJointsNode.h"
#include "vsOneJointNode.h"

vsJointsNode::vsJointsNode(OpenSim::JointSet *jointSet,vsNavigatorNode* parentNode,QObject *parent):
    vsNavigatorNode(jointSet,"Joints",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/vsJointsNode.png";

    //loading joints
    for (int i = 0; i < jointSet->getSize(); ++i) {
        OpenSim::Joint *joint = &jointSet->get(i);
        vsOneJointNode *jointNode = new vsOneJointNode(joint,this,this);
    }
}
