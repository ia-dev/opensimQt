#include "vsOneDofNode.h"

vsOneDofNode::vsOneDofNode(OpenSim::TransformAxis *tAxis,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(tAxis,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/dofNode.png";
}
