#include "vsNavigatorNode.h"

#include <QAction>
#include <qdebug.h>

vsNavigatorNode::vsNavigatorNode(OpenSim::Object *_openSimObj,QString _displayName,vsNavigatorNode *_parentNode,QObject *parent) :QObject(parent),
  openSimObject(_openSimObj),displayName(_displayName),parentNode(_parentNode)
{
    if(openSimObject != nullptr)
        displayName = QString::fromStdString(openSimObject->getName());
    if(parentNode != nullptr)
        parentNode->childNodes.append(this);
    iconPath = ":/Data/Images/Nodes/bodyNode.png";
    qDebug() << "visuazer vtk " <<  visualizerVTK->objectName();
}

vsNavigatorNode::~vsNavigatorNode()
{
    foreach (auto node, childNodes) {
        childNodes.removeOne(node);
        node->deleteLater();
    }
}

QList<QAction*> vsNavigatorNode::getNodeActions()
{
    QList<QAction*> returnList;
    returnList << new QAction("Display");
    return returnList;
}

vsVisualizerVTK* vsNavigatorNode::visualizerVTK = nullptr;
