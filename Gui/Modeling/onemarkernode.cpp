#include "onemarkernode.h"

OneMarkerNode::OneMarkerNode(OpenSim::Marker *marker,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(marker,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/markerNode.png";
}
