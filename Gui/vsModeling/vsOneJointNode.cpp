#include "vsOneDofNode.h"
#include "vsOneFrameNode.h"
#include "vsOneJointNode.h"

vsOneJointNode::vsOneJointNode(OpenSim::Joint *joint,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(joint,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/jointNode.png";

    //loading transforms
    OpenSim::CustomJoint *cj = OpenSim::CustomJoint::safeDownCast(joint);
    if(cj != nullptr)
    {
        OpenSim::SpatialTransform jointTransform = cj->updSpatialTransform();
        for (int j = 0; j < 6; ++j) {
            OpenSim::TransformAxis *transformAxis = &jointTransform.updTransformAxis(j);
            vsOneDofNode *axisNode = new vsOneDofNode(transformAxis,this,this);
        }
    }

    //loading frames
    for (int j = 0; j < joint->getProperty_frames().size(); ++j) {
        OpenSim::PhysicalOffsetFrame *pyOfstFrame =  &joint->upd_frames(j);
        vsOneFrameNode *frameNode = new vsOneFrameNode(pyOfstFrame,this,this);
    }

}

void vsOneJointNode::setupNodeActions(QMenu *rootMenu)
{
    QAction *toggleParentFramAction = new QAction("Toggle Parent Fram",rootMenu);
    QAction *toggleChildFramAction = new QAction("Toggle Child Fram",rootMenu);
    toggleChildFramAction->setCheckable(true);
    toggleChildFramAction->setChecked(true);
    toggleParentFramAction->setCheckable(true);
    rootMenu->addAction(toggleParentFramAction);
    rootMenu->addAction(toggleChildFramAction);
}
