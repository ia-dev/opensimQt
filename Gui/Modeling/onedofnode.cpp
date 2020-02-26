#include "onedofnode.h"

OneDofNode::OneDofNode(OpenSim::TransformAxis *tAxis,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(tAxis,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/dofNode.png";
}
