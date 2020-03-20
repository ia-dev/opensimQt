#include "vsOneWrapObjectNode.h"

vsOneWrapObjectNode::vsOneWrapObjectNode(OpenSim::WrapObject *wrapObj,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(wrapObj,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/wrapNode.png";
}

void vsOneWrapObjectNode::setupNodeActions(QMenu *rootMenu)
{
    vsNavigatorNode::setupNodeActions(rootMenu);
    displayMenu->addSeparator();
    QAction *smoothShadedAction = new QAction("Smooth Shaded",displayMenu);
    QAction *wireFramAction = new QAction("Wireframe");
    displayMenu->addAction(smoothShadedAction);
    displayMenu->addAction(wireFramAction);
}
