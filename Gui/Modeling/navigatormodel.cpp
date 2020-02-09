#include "navigatormodel.h"

NavigatorModel::NavigatorModel(NavigatorNode *rootNode):m_rootNNode(rootNode)
{

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
