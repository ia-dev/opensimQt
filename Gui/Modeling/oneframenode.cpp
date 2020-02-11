#include "oneframenode.h"

OneFrameNode::OneFrameNode(OpenSim::Frame *frame,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(frame,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/body.png";
}
