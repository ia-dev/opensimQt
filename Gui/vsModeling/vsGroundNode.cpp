#include "vsGroundNode.h"

vsGroundNode::vsGroundNode(OpenSim::Ground *gnd,vsNavigatorNode *parentNode,QObject *parent):
    vsOneFrameNode(gnd,parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/bodyNode.png";
}
