#include "vsOneWrapObjectNode.h"

#include <QDebug>

vsOneWrapObjectNode::vsOneWrapObjectNode(OpenSim::WrapObject *wrapObj,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(wrapObj,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/wrapNode.png";
    qDebug()<< "wrap object Name" << QString::fromStdString(wrapObj->getName());
    qDebug()<< "wrap object Name object" << QString::fromStdString(openSimObject->getConcreteClassName());
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
