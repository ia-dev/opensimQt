#include "markersnode.h"
#include "onemarkernode.h"

MarkersNode::MarkersNode(OpenSim::MarkerSet *markerSet,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(markerSet,"markers",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/markersNode.png";

    //adding markers
    for (int i = 0; i < markerSet->getSize(); ++i) {
        OpenSim::Marker *marker = &markerSet->get(i);
        OneMarkerNode *oneMarkerNode = new OneMarkerNode(marker,this,this);
    }
}
