#include "navigatormodel.h"
#include <QDebug>
NavigatorModel::NavigatorModel(OpenSim::Model *rootModel)
{
    loadOpenSimModel(rootModel);
}

void NavigatorModel::loadOpenSimModel(OpenSim::Model *openSimModel)
{
    //loading the model it self
    m_rootOpenSimModel = openSimModel;
    m_rootNNode = new NavigatorNode(openSimModel,"the root ",nullptr,this);

    //loading bodies
    NavigatorNode *bodySetNode = new NavigatorNode(nullptr,"Bodies",m_rootNNode,this);
    OpenSim::BodySet bodySet = m_rootOpenSimModel->updBodySet();
    for (int i = 0; i < bodySet.getSize(); i++) {
        NavigatorNode *bodyNode = new NavigatorNode(&(bodySet.get(i)),"items",bodySetNode,this);
        qDebug() << "body display name : " <<bodyNode->displayName;
        qDebug() << "body display name from item: " <<QString::fromStdString(bodyNode->openSimObject->getName());
    }
//    foreach (OpenSim::Body *body, m_rootOpenSimModel->getBodySet()) {
//        NavigatorNode *bodyNode = new NavigatorNode(body,"",bodySetNode,this);
//    }

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
