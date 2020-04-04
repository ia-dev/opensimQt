#include "vsNavigatorModel.h"
#include "vsNavigatorNode.h"
#include "vsPropertyItem.h"
#include "vsPropertyModel.h"

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

QVariantMap vsNavigatorNode::getNodeProperties()
{
    QVariantMap retMap;
    //adding the properties
    QVariantList properties;

    if(openSimObject != nullptr){
        QVariantMap nameProperty;
        nameProperty.insert("name",QString::fromStdString(openSimObject->getName()));
    }
    return retMap;
}

void vsNavigatorNode::setupPropertiesModel(vsPropertyModel *model)
{
    if(openSimObject != nullptr){
        //name property
        vsPropertyItem *nameItem = new vsPropertyItem();
        nameItem->m_name = "name";
        nameItem->m_value = QString::fromStdString(openSimObject->getName());
        nameItem->m_type = vsPropertyItem::Text;
        nameItem->setText(nameItem->m_value);
        model->m_propertiesItem->appendRow(QList<QStandardItem*>()<< new QStandardItem("name") << nameItem);


        //type property
        vsPropertyItem *typeItem = new vsPropertyItem();
        typeItem->m_name = "type";
        typeItem->m_isEditable = false;
        typeItem->m_value = QString::fromStdString(openSimObject->getConcreteClassName());
        typeItem->m_type = vsPropertyItem::Text;
        typeItem->setText(typeItem->m_value);
        model->m_propertiesItem->appendRow(QList<QStandardItem*>()<< new QStandardItem("type") << typeItem);

        //components property
        auto componentsProperty = &openSimObject->getPropertyByName("components");
        if(componentsProperty != nullptr){
            QString propertyRepresentation = "[ ";
            for (int i = 0; i < componentsProperty->size(); ++i) {
                auto comp = &componentsProperty->getValueAsObject(i);
                propertyRepresentation += (" "+QString::fromStdString(comp->getName()));
            }
            propertyRepresentation += " ]";
            if(componentsProperty->size()==0) propertyRepresentation = "[ no component ]";
            vsPropertyItem *componetsItem = new vsPropertyItem();
            componetsItem->m_name = "components";
            componetsItem->m_isEditable = false;
            componetsItem->m_value = propertyRepresentation;
            componetsItem->m_type = vsPropertyItem::List;
            componetsItem->setText(propertyRepresentation);
            model->m_propertiesItem->appendRow(QList<QStandardItem*>()<< new QStandardItem("components") << componetsItem);
        }


    }
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
