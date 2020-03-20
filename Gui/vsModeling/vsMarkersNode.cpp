#include "vsMarkersNode.h"
#include "vsOneMarkerNode.h"

vsMarkersNode::vsMarkersNode(OpenSim::MarkerSet *markerSet,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(markerSet,"markers",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/markersNode.png";
    editColorAndOpacity = false;
    //adding markers
    for (int i = 0; i < markerSet->getSize(); ++i) {
        OpenSim::Marker *marker = &markerSet->get(i);
        vsOneMarkerNode *oneMarkerNode = new vsOneMarkerNode(marker,this,this);
    }
}

void vsMarkersNode::setupNodeActions(QMenu *rootMenu)
{
    vsNavigatorNode::setupNodeActions(rootMenu);
    QAction *newAction = new QAction("Add New",rootMenu);
    QAction *saveAction = new QAction("Save to File...",rootMenu);
    QAction *loadAction = new QAction("Load From File... ",rootMenu);
    rootMenu->addActions(QList<QAction*>()<< newAction << saveAction << loadAction );
}
