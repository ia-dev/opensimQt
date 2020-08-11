#include "vsConstraintsNode.h"
#include "vsOneConstraintNode.h"

vsConstraintsNode::vsConstraintsNode(OpenSim::ConstraintSet *constraintSet,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(constraintSet,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/constraintsNode.png";
    editColorAndOpacity = false;
    //loading constraints
    for (int i = 0; i < constraintSet->getSize(); ++i) {
        OpenSim::Constraint *constraint  = &constraintSet->get(i);
        vsOneConstraintNode *constraintNode = new vsOneConstraintNode(constraint,this,this);
    }
}
