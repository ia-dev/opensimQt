#include "vsModelNode.h"
#include "vsNavigatorModel.h"
#include <QDebug>
#include <QIcon>
#include <vsTools/vsOpenSimTools.h>
vsNavigatorModel::vsNavigatorModel()
{
    m_rootNNode = new vsNavigatorNode(nullptr,"the root",nullptr,this);
    m_rootNNode->setConnectedModel(this);
}

void vsNavigatorModel::clean()
{
    emit layoutAboutToBeChanged();
    beginResetModel();
    int rowsToBeRemoved = m_rootNNode->childNodes.size();
    m_activeModel = nullptr;
    foreach (auto modelNode, m_rootNNode->childNodes) {
        m_rootNNode->childNodes.removeOne(modelNode);
        modelNode->deleteLater();
    }
    foreach (auto model_, m_openModels) {
        m_openModels.removeOne(model_);
        delete model_;
    }
    endResetModel();
    emit layoutChanged();
}

void vsNavigatorModel::getActionsForIndex(QModelIndex selected_index,QMenu *rootMenu)
{
    vsNavigatorNode *selectedNode  = nodeForIndex(selected_index);
    selectedNode->setupNodeActions(rootMenu);
}

void vsNavigatorModel::loadOpenSimModel(OpenSim::Model *openSimModel)
{
    SimTK::State *stat = &openSimModel->initSystem();
    //openSimModel->realizePosition(openSimModel->updWorkingState());
    //openSimModel->realizeVelocity(openSimModel->updWorkingState());
    //openSimModel->realizeAcceleration(openSimModel->updWorkingState());
    //openSimModel->buildSystem();
    //SimTK::State *stat = &openSimModel->initializeState();
   // openSimModel->realizeDynamics(openSimModel->updWorkingState());
    vsOpenSimTools::tools->modelStateMap.insert(openSimModel,stat);
    m_openModels.append(openSimModel);
    setActiveModel(openSimModel);
    //loading the model it self
    //m_rootOpenSimModel->initSystem();
    //m_rootOpenSimModel.getn

    vsModelNode *modelNNode = new vsModelNode(m_activeModel,m_rootNNode,this);

    emit expendIndex(indexForNNode(modelNNode));

    emit layoutChanged();
//    m_activeModel->setUseVisualizer(true);
//    m_activeModel->updMatterSubsystem().setShowDefaultGeometry(true);
//    m_visualiser = &openSimModel->updVisualizer().updSimbodyVisualizer();
//    m_visualiser->setBackgroundType(m_visualiser->SolidColor);
//    m_visualiser->setBackgroundColor(SimTK::White);

    //OpenSim::simulate(*m_activeModel,*stat,10.0);

}

QModelIndex vsNavigatorModel::selectObject(OpenSim::Object *obj)
{
    vsNavigatorNode *selectedObjNode  = m_rootNNode->getNodeForObj(obj);
    if(selectedObjNode == nullptr) return QModelIndex() ;
    qDebug() << "selected Node " << selectedObjNode->displayName;
    auto index = indexForNNode(selectedObjNode);
    emit this->expendIndex(index);
    return index;
}

QModelIndex vsNavigatorModel::index(int row, int column, const QModelIndex &parent) const
{
    if(hasIndex(row,column,parent))
    {
        vsNavigatorNode *parentNode = nodeForIndex(parent);
        vsNavigatorNode *childNode = parentNode->childNodes.at(row);
        return createIndex(row,column,childNode);
    }
    return QModelIndex();
}

QModelIndex vsNavigatorModel::parent(const QModelIndex &child) const
{
    vsNavigatorNode *childNode = nodeForIndex(child);
    vsNavigatorNode *parentNode = childNode->parentNode;
    if(parentNode ==  m_rootNNode)
        return QModelIndex();
    int row = rowForNode(parentNode);
    int column = 0;
    return createIndex(row,column,parentNode);

}

int vsNavigatorModel::rowCount(const QModelIndex &parent) const
{
    vsNavigatorNode *parentNode = nodeForIndex(parent);
    return parentNode->childNodes.count();
}

int vsNavigatorModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant vsNavigatorModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && role==Qt::DisplayRole)
    {
        vsNavigatorNode *nNode = nodeForIndex(index);
        return nNode->displayName + (nNode->openSimObject == m_activeModel?"  [Current]":"") ;
    }
    if (index.isValid() && role==Qt::DecorationRole) {
        vsNavigatorNode *nNode = nodeForIndex(index);
        return QVariant::fromValue(QPixmap(nNode->iconPath));
    }
    return QVariant();

}

QModelIndex vsNavigatorModel::indexForNNode(vsNavigatorNode *nNode)
{
    if(nNode == m_rootNNode)
        return QModelIndex();
    int row = rowForNode(nNode);
    int column = 0;
    return createIndex(row,column,nNode);
}

vsNavigatorNode* vsNavigatorModel::nodeForIndex(const QModelIndex &index) const
{
    if(index.isValid())
        return static_cast<vsNavigatorNode*>(index.internalPointer());
    return m_rootNNode;

}

int vsNavigatorModel::rowForNode(vsNavigatorNode *node) const
{
    return node->parentNode->childNodes.indexOf(node);
}

OpenSim::Model *vsNavigatorModel::getActiveModel() const
{
    return m_activeModel;
}

void vsNavigatorModel::setActiveModel(OpenSim::Model *activeModel)
{
    m_activeModel = activeModel;
    emit activeModelUpdated();
}

QList<OpenSim::Model *> vsNavigatorModel::getOpenModels() const
{
    return m_openModels;
}

void vsNavigatorModel::closeCurrentModel()
{
    if(m_activeModel == nullptr){
        vsOpenSimTools::tools->log("No Current Model to be closed","NavigatorModel",vsOpenSimTools::Error);
        return;
    }
    vsModelNode *activeNode = getNodeForModel(m_activeModel);

    activeNode->visualizerVTK->removeModelActors(static_cast<OpenSim::Model*>(activeNode->openSimObject));

    activeNode->removeNode();
    m_openModels.removeOne(m_activeModel);


    if(m_openModels.size()>0)
        setActiveModel(m_openModels.first());
    else
        setActiveModel(nullptr);
    vsOpenSimTools::tools->log("Current Model closed","NavigatorModel",vsOpenSimTools::Success);
    emit layoutChanged();
    //TODO remove the model from the tools

}

void vsNavigatorModel::closeAllModels()
{
    foreach(auto modelNode,m_rootNNode->childNodes){
        OpenSim::Model *model = static_cast<OpenSim::Model*>(modelNode->openSimObject);
        modelNode->visualizerVTK->removeModelActors(model);
        modelNode->removeNode();
    }
    setActiveModel(nullptr);
    foreach(auto model , m_openModels){
        qDebug() << "model to remove :"<< QString::fromStdString(model->getName());
        m_openModels.removeOne(model);
        delete model;
    }
    vsOpenSimTools::tools->log("All Models are closed","NavigatorModel",vsOpenSimTools::Success);
    emit layoutChanged();
}

void vsNavigatorModel::closeModel(OpenSim::Model *model)
{

    vsModelNode *activeNode = getNodeForModel(model);

    activeNode->visualizerVTK->removeModelActors(static_cast<OpenSim::Model*>(activeNode->openSimObject));

    beginRemoveRows(indexForNNode(activeNode->parentNode),rowForNode(activeNode),rowForNode(activeNode));
    activeNode->removeNode();
    m_openModels.removeOne(model);
    endRemoveRows();

    if(model == m_activeModel  && m_openModels.size()>0)
        setActiveModel(m_openModels.first());
    else
        setActiveModel(nullptr);
    vsOpenSimTools::tools->log("Model "+QString::fromStdString(model->getName())+" closed","NavigatorModel",vsOpenSimTools::Success);
    emit layoutChanged();
    //TODO remove the model from the tools
}

vsModelNode *vsNavigatorModel::getNodeForModel(OpenSim::Model *model)
{
    foreach (auto *nNode, m_rootNNode->childNodes) {
        vsModelNode *modelNode = static_cast<vsModelNode*>(nNode);
        if(modelNode->openSimObject == model)
            return static_cast<vsModelNode*>(nNode);
    }
    return nullptr;
}

vsNavigatorNode *vsNavigatorModel::getNodeForObject(OpenSim::Object *obj)
{
    return  m_rootNNode->getNodeForObj(obj);
}

