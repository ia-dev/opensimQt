#include "onebodynode.h"

OneBodyNode::OneBodyNode(OpenSim::Body *body,NavigatorNode *parentNode,QObject *parent):
    OneFrameNode(body,parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/bodyNode.png";
}
