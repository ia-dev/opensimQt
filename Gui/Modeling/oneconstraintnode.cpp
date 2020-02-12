#include "oneconstraintnode.h"

OneConstraintNode::OneConstraintNode(OpenSim::Constraint *constraint,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(constraint,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/constraintNode.png";
}
