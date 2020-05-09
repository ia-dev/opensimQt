#include "vsAllForcesNode.h"
#include "vsBodiesNode.h"
#include "vsConstraintsNode.h"
#include "vsContactGeometriesNode.h"
#include "vsControlersNode.h"
#include "vsGroundNode.h"
#include "vsJointsNode.h"
#include "vsMarkersNode.h"
#include "vsModelNode.h"
#include "vsProbesNode.h"
#include "vsNavigatorModel.h"
#include "vsPropertyItem.h"
#include "vsPropertyModel.h"
#include <QDebug>
#include <QInputDialog>
#include <QRegularExpression>
#include <qdialog.h>
#include <vsTools/vsXmlUtils.h>

vsModelNode::vsModelNode(OpenSim::Model *model,vsNavigatorNode *parentNode,QObject *parent):vsNavigatorNode(model,"",parentNode,parent)
{

    iconPath = ":/Data/Images/Nodes/osimNode.png";

    //loading ground
    vsGroundNode *groundNode = new vsGroundNode(&model->updGround(),this,this);

    //loading bodySet
    vsBodiesNode *bodiesNode = new vsBodiesNode(&model->upd_BodySet(),this,this);

    //loading jointsSet
    vsJointsNode *jointsNode = new vsJointsNode(&model->updJointSet(),this,this);

    //loading constraint
    vsConstraintsNode *constraintNode = new vsConstraintsNode(&model->updConstraintSet(),this,this);

    //loading contact geometry
    vsContactGeometriesNode *contactGeometriesNode = new vsContactGeometriesNode(&model->updContactGeometrySet(),this,this);

    //loading forces
    vsAllForcesNode *allForcesNode = new vsAllForcesNode(model,this,this);

    //loading markers
    vsMarkersNode *markersNode = new vsMarkersNode(&model->updMarkerSet(),this,this);

    //loading controlers
    vsControlersNode *controlesNode =new vsControlersNode(&model->updControllerSet(),this,this);

    //loading probs
    vsProbesNode *probesNode = new vsProbesNode(&model->updProbeSet(),this,this);


    visualizerVTK->addOpenSimModel(model);

}

void vsModelNode::onCloseModelClicked()
{
    qDebug() << "model  Closing";
    connectedModel()->closeModel(static_cast<OpenSim::Model*>(this->openSimObject));

}

void vsModelNode::onRenameModelTriggered()
{
    qDebug() << "renaming model";
    auto modelObj = OpenSim::Model::safeDownCast(openSimObject);
    if(!modelObj) return;
    QString newName = QInputDialog::getText(nullptr,"Rename","New Model Name: ");
    vsXmlUtils::changeModelName(modelObj->getInputFileName(),newName.toStdString());
    //modelObj->readObjectFromXMLNodeOrFile(newModelNode);
    //reload the model afterword
}

void vsModelNode::setupNodeActions(QMenu *rootMenu)
{
    QAction *makeCurrentAction = new QAction("Make Current",rootMenu);
    QAction *renameAction =  new QAction("Rename...",rootMenu);
    rootMenu->addActions(QList<QAction*>() << makeCurrentAction << renameAction);
    vsNavigatorNode::setupNodeActions(rootMenu);
    QAction *infoAction = new QAction("Info",rootMenu);
    rootMenu->addAction(infoAction);
    rootMenu->addSeparator();
    QAction *saveAction = new QAction("Save...",rootMenu);
    QAction *saveAsAction = new QAction("Save As...",rootMenu);
    rootMenu->addActions(QList<QAction*>() << saveAction << saveAsAction);
    rootMenu->addSeparator();
    QAction *closeAction = new QAction("Close",rootMenu);
    rootMenu->addAction(closeAction);
    rootMenu->addSeparator();
    QAction* addMotionAction = new QAction("Add Motion...",rootMenu);
    rootMenu->addAction(addMotionAction);

    connect(closeAction,&QAction::triggered,this,&vsModelNode::onCloseModelClicked);
    connect(renameAction,&QAction::triggered,this,&vsModelNode::onRenameModelTriggered);
}

void vsModelNode::setupPropertiesModel(vsPropertyModel *model)
{
    QStringList textObjProperties;
    textObjProperties << "credits" <<  "publications" << "length_units" << "force_units";
    vsNavigatorNode::setupPropertiesModel(model);
//    for (int i = 0; i < openSimObject->getNumProperties(); ++i) {

//        bool isModelProperty = false;

//        auto ap = &openSimObject->getPropertyByIndex(i);
//        ap->
//        QString apName = QString::fromStdString(ap->getName());
//        vsPropertyItem *apItem = new vsPropertyItem();
//        QStandardItem *apNameItem = new QStandardItem(apName);
//        qDebug() << apName << QString::fromStdString(ap->getTypeName());
//        apItem->m_name = apName;
//        QRegExp setRE(".*Set");

//        if(setRE.exactMatch(apName) || apName == "ground" || apName== "ModelVisualPreferences"){
//            apItem->m_isEditable = false;
//            apItem->m_type = vsPropertyItem::Object;
//            apItem->m_object = const_cast<OpenSim::Object*>(&ap->getValueAsObject());
//            apItem->setText(QString::fromStdString(apItem->m_object->getName()));
//            isModelProperty =  true;
//        }

//        else if( textObjProperties.contains(apName)){
//            apItem->m_isEditable = true;
//            apItem->m_type = vsPropertyItem::TextObj;
//            apItem->m_value = QString::fromStdString(ap->getValue<std::string>());
//            apItem->setText(apItem->m_value);
//            isModelProperty =  true;
//        }
//        else if(apName == "assembly_accuracy"){
//            apItem->m_isEditable = true;
//            apItem->m_type = vsPropertyItem::Text;
//            apItem->m_value = QString::number(ap->getValue<double>());
//            apItem->setText(apItem->m_value);
//            isModelProperty =  true;
//        }
//        else if(apName == "gravity"){
//            apItem->m_isEditable = true;
//            apItem->m_type = vsPropertyItem::List;
//            apItem->m_value = QString::number(ap->size());
//            apItem->setText(apItem->m_value);
//            isModelProperty =  true;
//        }
//        if(!isModelProperty) continue;
//        model->m_propertiesItem->appendRow(QList<QStandardItem*>()<< apNameItem << apItem);
//    }
}
