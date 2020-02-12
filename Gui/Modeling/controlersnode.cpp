#include "controlersnode.h"
#include "controlnode.h"

ControlersNode::ControlersNode(OpenSim::ControllerSet *controlSet ,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(controlSet,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/constraintsNode.png";

    for (int i = 0; i < controlSet->getSize(); ++i) {
        OpenSim::Controller *control = &controlSet->get(i);
        ControlNode *controleNode = new ControlNode(control,this,this);
    }

}
