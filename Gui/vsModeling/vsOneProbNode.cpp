#include "vsOneProbNode.h"

vsOneProbNode::vsOneProbNode(OpenSim::Probe *probe,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(probe,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/probe_single.png";
}
