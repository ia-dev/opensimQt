#include "vsOneWrapObjectNode.h"

vsOneWrapObjectNode::vsOneWrapObjectNode(OpenSim::WrapObject *wrapObj,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(wrapObj,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/wrapNode.png";
}
