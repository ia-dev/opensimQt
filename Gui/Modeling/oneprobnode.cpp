#include "oneprobnode.h"

OneProbNode::OneProbNode(OpenSim::Probe *probe,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(probe,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/probe_single.png";
}
