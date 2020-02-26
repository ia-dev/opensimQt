#include "onedofnode.h"
#include "oneframenode.h"
#include "onejointnode.h"

OneJointNode::OneJointNode(OpenSim::Joint *joint,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(joint,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/jointNode.png";

    //loading transforms
    OpenSim::CustomJoint *cj = OpenSim::CustomJoint::safeDownCast(joint);
    if(cj != nullptr)
    {
        OpenSim::SpatialTransform jointTransform = cj->updSpatialTransform();
        for (int j = 0; j < 6; ++j) {
            OpenSim::TransformAxis *transformAxis = &jointTransform.updTransformAxis(j);
            OneDofNode *axisNode = new OneDofNode(transformAxis,this,this);
        }
    }

    //loading frames
    for (int j = 0; j < joint->getProperty_frames().size(); ++j) {
        OpenSim::PhysicalOffsetFrame *pyOfstFrame =  &joint->upd_frames(j);
        OneFrameNode *frameNode = new OneFrameNode(pyOfstFrame,this,this);
    }

}
