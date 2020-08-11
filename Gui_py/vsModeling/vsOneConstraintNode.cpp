#include "vsOneConstraintNode.h"

vsOneConstraintNode::vsOneConstraintNode(OpenSim::Constraint *constraint,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(constraint,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/constraintNode.png";
}
