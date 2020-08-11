#include "vsOneMarkerNode.h"

vsOneMarkerNode::vsOneMarkerNode(OpenSim::Marker *marker,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(marker,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/markerNode.png";
}

void vsOneMarkerNode::setupNodeActions(QMenu *rootMenu)
{
    vsNavigatorNode::setupNodeActions(rootMenu);
    QAction *deleteAction = new QAction("Delete",rootMenu);
    rootMenu->addAction(deleteAction);
    foreach (QAction *action, displayMenu->actions()) {
        if(action->text()=="Color..." || action->text()== "Opacity...")
            displayMenu->removeAction(action);
    }
}
