#include "vsMarkersNode.h"
#include "vsOneMarkerNode.h"

vsMarkersNode::vsMarkersNode(OpenSim::MarkerSet *markerSet,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(markerSet,"markers",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/markersNode.png";

    //adding markers
    for (int i = 0; i < markerSet->getSize(); ++i) {
        OpenSim::Marker *marker = &markerSet->get(i);
        vsOneMarkerNode *oneMarkerNode = new vsOneMarkerNode(marker,this,this);
    }
}
