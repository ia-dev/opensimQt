#include "vsOneMarkerNode.h"

vsOneMarkerNode::vsOneMarkerNode(OpenSim::Marker *marker,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(marker,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/markerNode.png";
}
