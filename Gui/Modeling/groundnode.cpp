#include "groundnode.h"

GroundNode::GroundNode(OpenSim::Ground *gnd,NavigatorNode *parentNode,QObject *parent):
    OneFrameNode(gnd,parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/bodyNode.png";
}
