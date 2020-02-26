#include "navigatornode.h"

#include <qdebug.h>

NavigatorNode::NavigatorNode(OpenSim::Object *_openSimObj,QString _displayName,NavigatorNode *_parentNode,QObject *parent) :QObject(parent),
  openSimObject(_openSimObj),displayName(_displayName),parentNode(_parentNode)
{
    if(openSimObject != nullptr)
        displayName = QString::fromStdString(openSimObject->getName());
    if(parentNode != nullptr)
        parentNode->childNodes.append(this);
    iconPath = ":/Data/Images/Nodes/bodyNode.png";
    qDebug() << "visuazer vtk " <<  visualizerVTK->objectName();
}

VisualizerVTK* NavigatorNode::visualizerVTK = nullptr;
