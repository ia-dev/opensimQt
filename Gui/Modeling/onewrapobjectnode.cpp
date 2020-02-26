#include "onewrapobjectnode.h"

OneWrapObjectNode::OneWrapObjectNode(OpenSim::WrapObject *wrapObj,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(wrapObj,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/wrapNode.png";
}
