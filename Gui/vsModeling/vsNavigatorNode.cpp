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
        typeItem->m_value = QString::fromStdString(openSimObject->getConcreteClassName());
        typeItem->m_type = vsPropertyItem::Text;
        typeItem->setText(typeItem->m_value);
        model->m_propertiesItem->appendRow(QList<QStandardItem*>()<< new QStandardItem("type") << typeItem);
        qDebug() << "updated";


        QRegExp socketsRE("socket_(.*)");
        QRegExp inputsRE("input_(.*)");

        //the abstract properties
        for (int i = 0; i < openSimObject->getNumProperties(); ++i) {
             const OpenSim::AbstractProperty *absProperty = &openSimObject->getPropertyByIndex(i);
             QString propertyName = QString::fromStdString(absProperty->getName());
             if (socketsRE.exactMatch(propertyName) || inputsRE.exactMatch(propertyName)) {
                 continue;
             }
             vsPropertyItem *pItem = new vsPropertyItem();
             QStandardItem *pName = new QStandardItem();

             pItem->m_name = propertyName;
             pName->setText(propertyName);
             if(! absProperty->isListProperty()){
                 //Object property
                 if(absProperty->isObjectProperty() && absProperty->size() == 1){
                     const OpenSim::Object *objectFromProperty = &(absProperty->isOptionalProperty()
                             ?absProperty->getValueAsObject(0):absProperty->getValueAsObject());
                     auto gp =OpenSim::GeometryPath::safeDownCast(const_cast<OpenSim::Object*>(objectFromProperty));
                     auto fp =OpenSim::Function::safeDownCast(const_cast<OpenSim::Object*>(objectFromProperty));

                     if (gp != nullptr){
                        pItem->m_type = vsPropertyItem::GeometryPath;
                        pItem->m_object = gp;
                        pItem->m_value = QString::fromStdString(gp->getName());
                        pItem->setText(pItem->m_name);
                     }
                     else{

                         pItem->m_type = vsPropertyItem::Object;
                         pItem->m_object = const_cast<OpenSim::Object*>(objectFromProperty);
                         pItem->m_value = QString::fromStdString(pItem->m_object->getName());
                         pItem->setText(pItem->m_name);
                     }
                 }
                 //others
                 else{
                     if(absProperty->isOptionalProperty()){
                         QString apType = QString::fromStdString(absProperty->getTypeName());
                         pItem->m_name = propertyName;
                         if (apType.toLower() == "double"){

                             if(propertyName.contains("color",Qt::CaseInsensitive))
                             {
                                 pItem->m_type = vsPropertyItem::Color;
                                 pItem->m_value = "";
                                 pItem->setText(pItem->m_name);
                             }
                          }

                     }
                 }
             }
             //testing of the a property have been added
             model->m_propertiesItem->appendRow(QList<QStandardItem*>()<< pName << pItem);

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
