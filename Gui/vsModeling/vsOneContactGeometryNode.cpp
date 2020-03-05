#include "vsOneContactGeometryNode.h"

vsOneContactGeometryNode::vsOneContactGeometryNode(OpenSim::ContactGeometry *contactGeometry,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(contactGeometry,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/contactGeometryNode.png";
}
