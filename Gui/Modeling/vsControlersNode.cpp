#include "vsControlersNode.h"
#include "vsControlNode.h"

vsControlersNode::vsControlersNode(OpenSim::ControllerSet *controlSet ,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(controlSet,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/vsConstraintsNode.png";

    for (int i = 0; i < controlSet->getSize(); ++i) {
        OpenSim::Controller *control = &controlSet->get(i);
        vsControlNode *controleNode = new vsControlNode(control,this,this);
    }

}
