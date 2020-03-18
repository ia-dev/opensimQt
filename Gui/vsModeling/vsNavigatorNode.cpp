#include "vsNavigatorNode.h"

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

vsVisualizerVTK* vsNavigatorNode::visualizerVTK = nullptr;
