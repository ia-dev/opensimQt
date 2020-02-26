#include "modelnode.h"
#include "navigatormodel.h"
#include <QDebug>
#include <QIcon>
NavigatorModel::NavigatorModel()
{
    m_rootNNode = new NavigatorNode(nullptr,"the root",nullptr,this);
}

void NavigatorModel::loadOpenSimModel(OpenSim::Model *openSimModel)
{
    m_activeModel = openSimModel;
    SimTK::State *stat = &m_activeModel->initSystem();
    //loading the model it self
    m_openModels.append(openSimModel);
    //m_rootOpenSimModel->initSystem();
    //m_rootOpenSimModel.getn

    ModelNode *modelNNode = new ModelNode(m_activeModel,m_rootNNode,this);


    emit layoutChanged();
//    m_activeModel->setUseVisualizer(true);
//    m_activeModel->updMatterSubsystem().setShowDefaultGeometry(true);
//    m_visualiser = &openSimModel->updVisualizer().updSimbodyVisualizer();
//    m_visualiser->setBackgroundType(m_visualiser->SolidColor);
//    m_visualiser->setBackgroundColor(SimTK::White);

    //OpenSim::simulate(*m_activeModel,*stat,10.0);

}

QModelIndex NavigatorModel::index(int row, int column, const QModelIndex &parent) const
{
    if(hasIndex(row,column,parent))
    {
        NavigatorNode *parentNode = nodeForIndex(parent);
        NavigatorNode *childNode = parentNode->childNodes.at(row);
        return createIndex(row,column,childNode);
    }
    return QModelIndex();
}

QModelIndex NavigatorModel::parent(const QModelIndex &child) const
{
    NavigatorNode *childNode = nodeForIndex(child);
    NavigatorNode *parentNode = childNode->parentNode;
    if(parentNode ==  m_rootNNode)
        return QModelIndex();
    int row = rowForNode(parentNode);
    int column = 0;
    return createIndex(row,column,parentNode);

}

int NavigatorModel::rowCount(const QModelIndex &parent) const
{
    NavigatorNode *parentNode = nodeForIndex(parent);
    return parentNode->childNodes.count();
}

int NavigatorModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant NavigatorModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && role==Qt::DisplayRole)
    {
        NavigatorNode *nNode = nodeForIndex(index);
        return  nNode->displayName;
    }
    if (index.isValid() && role==Qt::DecorationRole) {
        NavigatorNode *nNode = nodeForIndex(index);
        return QVariant::fromValue(QPixmap(nNode->iconPath));
    }
    return QVariant();

}

QModelIndex NavigatorModel::indexForNNode(NavigatorNode *nNode)
{
    if(nNode == m_rootNNode)
        return QModelIndex();
    int row = rowForNode(nNode);
    int column = 0;
    return createIndex(row,column,nNode);
}

NavigatorNode* NavigatorModel::nodeForIndex(const QModelIndex &index) const
{
    if(index.isValid())
        return static_cast<NavigatorNode*>(index.internalPointer());
    return m_rootNNode;

}

int NavigatorModel::rowForNode(NavigatorNode *node) const
{
    return node->parentNode->childNodes.indexOf(node);
}
