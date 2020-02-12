#include "onecontactgeometrynode.h"

OneContactGeometryNode::OneContactGeometryNode(OpenSim::ContactGeometry *contactGeometry,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(contactGeometry,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/contactGeometryNode.png";
}
