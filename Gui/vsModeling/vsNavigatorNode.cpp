#include "vsNavigatorModel.h"
#include "vsNavigatorNode.h"
#include "vsPropertyItem.h"
#include "vsPropertyModel.h"

#include <QAction>
#include <QString>
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

void vsNavigatorNode::setupPropertiesModel(vsPropertyModel *model){
	bool isEmptyName = true;
	if (openSimObject != nullptr) {
		//name property
		//qDebug() << QString::fromStdString(openSimObject->getName());
		//qDebug() << QString::fromStdString(openSimObject->getName());
		std::cout << "bug here ================================ start" << std::endl;
		std::cout << openSimObject->getClassName() << std::endl;
		std::cout << openSimObject->getNumProperties() << std::endl;
		//std::cout << openSimObject->getName() << std::endl;
		//isEmptyName = (openSimObject->getName() == "");
		isEmptyName = (openSimObject->getNumProperties()<=0) | (openSimObject->getNumProperties() > 1000);
		std::cout << isEmptyName << std::endl;
		std::cout << "bug here ================================ end" << std::endl;
		/* windows test
           bodyset ---->base ----> Trilongglen  name ="" ok
		   jointset ---->r_elbow ----> rotation1, can not get the name, "Segmentation Fault"  

		*/
		vsPropertyItem *nameItem = new vsPropertyItem();
		//qDebug() << QString::fromLatin1(openSimObject->getName(),(int)openSimObject->getName().size());

		if (! isEmptyName) {
				std::cout << "VisSim: Getting object prperties ........................" << std::endl;
				nameItem->m_name = "name";
				std::string nameTmp = openSimObject->getName();
				nameItem->m_value = QString::fromStdString(nameTmp);
				qDebug() << nameItem->m_value;
				nameItem->m_type = vsPropertyItem::Text;
				nameItem->setText(nameItem->m_value);
				model->m_propertiesItem->appendRow(QList<QStandardItem*>() << new QStandardItem("name") << nameItem);

				//type property
				vsPropertyItem *typeItem = new vsPropertyItem();
				typeItem->m_name = "type";
				typeItem->m_isEditable = false;
				std::string typeTmp = openSimObject->getConcreteClassName();
				typeItem->m_value = QString::fromStdString(typeTmp);
				typeItem->m_type = vsPropertyItem::Text;
				typeItem->setText(typeItem->m_value);
				model->m_propertiesItem->appendRow(QList<QStandardItem*>() << new QStandardItem("type") << typeItem);

				for (int i = 0; i < openSimObject->getNumProperties(); ++i) {

					auto ap = &openSimObject->getPropertyByIndex(i);
					QString apName = QString::fromStdString(ap->getName());
					vsPropertyItem *apItem = new vsPropertyItem();
					QStandardItem *apNameItem = new QStandardItem(apName);
					qDebug() << apName << QString::fromStdString(ap->getTypeName());

					if (ap->isListProperty()) {

						apItem->m_type = vsPropertyItem::List;
						QString listRepresentation = "[ ";
						for (int j = 0; j < ap->size(); ++j) {
							listRepresentation += QString::fromStdString(ap->getValueAsObject(j).getConcreteClassName());
							listRepresentation += " ";
						}
						if (ap->size() == 0) listRepresentation += " empty ";
						listRepresentation += "]";
						apItem->m_value = listRepresentation;
						apItem->setText(apItem->m_value);
					}
					//            else if(ap->isOptionalProperty()){

					//            }
					//            else if(ap->isUnnamedProperty()){\
					//            }
					else if (ap->isObjectProperty()) {
						if (OpenSim::Function::safeDownCast(const_cast<OpenSim::Object*>(&ap->getValueAsObject()))) {
							qDebug() << "it is a function indeed";
						}
						else if (apName == "Appearance") {
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
							model->m_appearancexItem->appendRow(QList<QStandardItem*>() << visibleNameItem << visibleItem);

							//opacity property
							auto opacityProp = &appr->getProperty_opacity();
							vsPropertyItem *opacityItem = new vsPropertyItem();
							QStandardItem *opacityNameItem = new QStandardItem("opacity");
							opacityItem->m_type = vsPropertyItem::Text;
							opacityItem->m_value = QString::fromStdString(opacityProp->toString());
							opacityItem->setText(opacityItem->m_value);
							opacityItem->setEditable(true);
							model->m_appearancexItem->appendRow(QList<QStandardItem*>() << opacityNameItem << opacityItem);

							//color property
							auto colorProp = &appr->getProperty_color();
							vsPropertyItem *colorItem = new vsPropertyItem();
							QStandardItem *colorNameItem = new QStandardItem("color");
							colorItem->m_type = vsPropertyItem::Color;
							colorItem->m_value = QString::fromStdString(colorProp->toString());
							colorItem->setText(colorItem->m_value);
							colorItem->setEditable(true);
							model->m_appearancexItem->appendRow(QList<QStandardItem*>() << colorNameItem << colorItem);

							//color property
							auto dpProp = &appr->getProperty_SurfaceProperties();
							vsPropertyItem *dpItem = new vsPropertyItem();
							QStandardItem *dpNameItem = new QStandardItem("DisplayPreferences");
							dpItem->m_type = vsPropertyItem::Text;
							dpItem->m_value = QString::fromStdString(dpProp->toString());
							dpItem->setText(dpItem->m_value);
							dpItem->setEditable(true);
							model->m_appearancexItem->appendRow(QList<QStandardItem*>() << dpNameItem << dpItem);

							continue;
						}
						else {
							apItem->m_type = vsPropertyItem::Object;
							apItem->m_object = const_cast<OpenSim::Object*>(&ap->getValueAsObject());
							apItem->setText(QString::fromStdString(apItem->m_object->getName()));
							apItem->setEditable(false);
						}
					}
					else {
						apItem->m_type = vsPropertyItem::Text;
						apItem->m_value = QString::fromStdString(ap->toStringForDisplay(1));
						apItem->setText(apItem->m_value);
					}

					QRegExp socketRegEx("^socket.*");
					if (socketRegEx.exactMatch(apName)) {
						model->m_socketsItem->appendRow(QList<QStandardItem*>() << apNameItem << apItem);
					}
					else {
						model->m_propertiesItem->appendRow(QList<QStandardItem*>() << apNameItem << apItem);
					}

				}

		}
    }
	std::cout << "VisSim: Getting object prperties is done ........................" << std::endl;

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

vsNavigatorNode *vsNavigatorNode::getNodeForObj(OpenSim::Object *object)
{
    //return the node for the given object
    if(object == openSimObject) return this;
    if(childNodes.size() == 0) return nullptr;
    foreach (auto childNode, childNodes) {
        vsNavigatorNode *selectedNode = childNode->getNodeForObj(object);
        if(selectedNode != nullptr) return selectedNode;
    }
    return nullptr;
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
