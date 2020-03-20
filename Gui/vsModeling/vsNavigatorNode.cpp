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

void vsNavigatorNode::setupNodeActions(QMenu *rootMenu)
{
    displayMenu = new QMenu("Display",rootMenu);

    QAction *showAction= new QAction("Show",displayMenu);
    QAction *hideAction= new QAction("Hide",displayMenu);
    QAction *colorAction= new QAction("Color...",displayMenu);
    QAction *opacityAction= new QAction("Opacity...",displayMenu);

    displayMenu->addActions(QList<QAction*>() << showAction << hideAction << colorAction << opacityAction);

    rootMenu->addMenu(displayMenu);
    if(!editColorAndOpacity)
        disableActionsForSets();
}

void vsNavigatorNode::disableActionsForSets()
{
    QStringList actionsToDisable;
    actionsToDisable << "Color..." <<"Opacity...";
    foreach (auto action, displayMenu->actions()) {
        if(actionsToDisable.contains(action->text())) action->setEnabled(false);
    }
}


vsVisualizerVTK* vsNavigatorNode::visualizerVTK = nullptr;
