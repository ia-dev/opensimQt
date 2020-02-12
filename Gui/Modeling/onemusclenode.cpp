#include "onemusclenode.h"

OneMuscleNode::OneMuscleNode(OpenSim::Muscle *muscle,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(muscle,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/muscleNode.png";
}
