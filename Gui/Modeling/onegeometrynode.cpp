#include "onegeometrynode.h"

OneGeometryNode::OneGeometryNode(OpenSim::Geometry *geometry,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(geometry,"",parentNode,parent)

{
    iconPath = ":/Data/Images/Nodes/displayGeometry.png";
}
