#include "modelnode.h"

ModelNode::ModelNode(OpenSim::Model *model,NavigatorNode *parentNode,QObject *parent):NavigatorNode(model,"",parentNode,parent)
{

    iconPath = ":/Data/Images/Nodes/osimNode.png";

}
