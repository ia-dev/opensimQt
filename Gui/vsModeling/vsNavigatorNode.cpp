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

        for (int i = 0; i < openSimObject->getNumProperties(); ++i) {

            auto ap  = &openSimObject->getPropertyByIndex(i);
            QString apName = QString::fromStdString(ap->getName());
            vsPropertyItem *apItem = new vsPropertyItem();
            QStandardItem *apNameItem = new QStandardItem(apName);
            qDebug() << apName << QString::fromStdString(ap->getTypeName());


            if(ap->isListProperty()){

                apItem->m_type = vsPropertyItem::List;
                QString listRepresentation = "[ ";
                for (int j = 0; j < ap->size(); ++j) {
                    listRepresentation += QString::fromStdString(ap->getValueAsObject(j).getConcreteClassName());
                    listRepresentation += " ";
                }
                if(ap->size() == 0) listRepresentation += " empty ";
                listRepresentation += "]";
                apItem->m_value  = listRepresentation;
                apItem->setText(apItem->m_value);
            }
            else if(ap->isObjectProperty()){
                if(apName == "Appearance"){
                    auto _appr = &ap->getValueAsObject();
                    auto appr = OpenSim::Appearance::safeDownCast(const_cast<OpenSim::Object*>(_appr));
                    //visible property
                    auto visibleProp = &appr->getProperty_visible();
                    vsPropertyItem *visibleItem = new vsPropertyItem();
                    QStandardItem *visibleNameItem = new QStandardItem("visible");
                    visibleItem->m_type = vsPropertyItem::Check;
                    visibleItem->m_value = QString::fromStdString(visibleProp->toString());
                    visibleItem->setText(visibleItem->m_value);
                    visibleItem->setEditable(true);
                    model->m_appearancexItem->appendRow(QList<QStandardItem*>()<< visibleNameItem << visibleItem);

                    //opacity property
                    auto opacityProp = &appr->getProperty_opacity();
                    vsPropertyItem *opacityItem = new vsPropertyItem();
                    QStandardItem *opacityNameItem = new QStandardItem("opacity");
                    opacityItem->m_type = vsPropertyItem::Text;
                    opacityItem->m_value = QString::fromStdString(opacityProp->toString());
                    opacityItem->setText(opacityItem->m_value);
                    opacityItem->setEditable(true);
                    model->m_appearancexItem->appendRow(QList<QStandardItem*>()<< opacityNameItem << opacityItem);

                    //color property
                    auto colorProp = &appr->getProperty_color();
                    vsPropertyItem *colorItem = new vsPropertyItem();
                    QStandardItem *colorNameItem = new QStandardItem("color");
                    colorItem->m_type = vsPropertyItem::Color;
                    colorItem->m_value = QString::fromStdString(colorProp->toString());
                    colorItem->setText(colorItem->m_value);
                    colorItem->setEditable(true);
                    model->m_appearancexItem->appendRow(QList<QStandardItem*>()<< colorNameItem << colorItem);

                    //color property
                    auto dpProp = &appr->getProperty_SurfaceProperties();
                    vsPropertyItem *dpItem = new vsPropertyItem();
                    QStandardItem *dpNameItem = new QStandardItem("DisplayPreferences");
                    dpItem->m_type = vsPropertyItem::Text;
                    dpItem->m_value = QString::fromStdString(dpProp->toString());
                    dpItem->setText(dpItem->m_value);
                    dpItem->setEditable(true);
                    model->m_appearancexItem->appendRow(QList<QStandardItem*>()<< dpNameItem << dpItem);

                    continue;
                }
                else{
                    apItem->m_type = vsPropertyItem::Object;
                    apItem->m_object = const_cast<OpenSim::Object*>(&ap->getValueAsObject());
                    apItem->setText(QString::fromStdString(apItem->m_object->getName()));
                    apItem->setEditable(false);
                }
            }
            else{
                apItem->m_type = vsPropertyItem::Text;
                apItem->m_value = QString::fromStdString(ap->toStringForDisplay(1));
                apItem->setText(apItem->m_value);
            }

            QRegExp socketRegEx("^socket.*");
            if(socketRegEx.exactMatch(apName)){
                model->m_socketsItem->appendRow(QList<QStandardItem*>()<< apNameItem << apItem);
            }else{
                model->m_propertiesItem->appendRow(QList<QStandardItem*>()<< apNameItem << apItem);
            }

        }


//        //components property
//        auto componentsProperty = &openSimObject->getPropertyByName("components");
//        if(componentsProperty != nullptr){
//            QString propertyRepresentation = "[ ";
//            for (int i = 0; i < componentsProperty->size(); ++i) {
//                auto comp = &componentsProperty->getValueAsObject(i);
//                propertyRepresentation += (" "+QString::fromStdString(comp->getName()));
//            }
//            propertyRepresentation += " ]";
//            if(componentsProperty->size()==0) propertyRepresentation = "[ no component ]";
//            vsPropertyItem *componetsItem = new vsPropertyItem();
//            componetsItem->m_name = "components";
//            componetsItem->m_isEditable = false;
//            componetsItem->m_value = propertyRepresentation;
//            componetsItem->m_type = vsPropertyItem::List;
//            componetsItem->setText(propertyRepresentation);
//            model->m_propertiesItem->appendRow(QList<QStandardItem*>()<< new QStandardItem("components") << componetsItem);
//        }


    }
}

void vsNavigatorNode::selectVisualizerActors()
{
    if(openSimObject == nullptr) return;
    auto componentActors = visualizerVTK->getActorForComponent(openSimObject);
    if(componentActors == nullptr) return;
    qDebug() << "actors size " << componentActors->size();

    foreach (auto actor, *componentActors) {
        actor->SetVisibility(false);
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
