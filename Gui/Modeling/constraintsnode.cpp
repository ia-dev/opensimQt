#include "constraintsnode.h"
#include "oneconstraintnode.h"

ConstraintsNode::ConstraintsNode(OpenSim::ConstraintSet *constraintSet,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(constraintSet,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/constraintsNode.png";

    //loading constraints
    for (int i = 0; i < constraintSet->getSize(); ++i) {
        OpenSim::Constraint *constraint  = &constraintSet->get(i);
        OneConstraintNode *constraintNode = new OneConstraintNode(constraint,this,this);
    }
}
