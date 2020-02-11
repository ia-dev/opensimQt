#include "bodiesnode.h"

BodiesNode::BodiesNode(NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(nullptr,"Bodies",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/bodiesNode.png";
}
