#include "vsNavigatorModel.h"
#include "vsNavigatorNode.h"

#include <QAction>
#include <qdebug.h>

vsNavigatorNode::vsNavigatorNode(OpenSim::Object *_openSimObj,QString _displayName,vsNavigatorNode *_parentNode,QObject *parent) :QObject(parent),
  openSimObject(_openSimObj),displayName(_displayName),parentNode(_parentNode),m_connectedModel(nullptr)
{
    if(openSimObject != nullptr)
        displayName = QString::fromStdString(openSimObject->getName());
    if(parentNode != nullptr){
        parentNode->childNodes.append(this);
        setConnectedModel(parentNode->connectedModel());
    }
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

void vsNavigatorNode::removeNode()
{
    parentNode->childNodes.removeOne(this);
    this->deleteLater();
}

vsNavigatorModel *vsNavigatorNode::connectedModel() const
{
    return m_connectedModel;
}

void vsNavigatorNode::setConnectedModel(vsNavigatorModel *connectedModel)
{
    if (m_connectedModel == connectedModel)
        return;

    m_connectedModel = connectedModel;
    emit connectedModelChanged(m_connectedModel);
}


vsVisualizerVTK* vsNavigatorNode::visualizerVTK = nullptr;
