/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsPropertyModel.h"

vsPropertyModel::vsPropertyModel()
{

}

QModelIndex vsPropertyModel::index(int row, int column, const QModelIndex &parent) const
{
    if(hasIndex(row,column,parent))
    {
        vsPropertyNode *parentNode = nodeForIndex(parent);
        vsPropertyNode *childNode = parentNode->childNodes.at(row);
        return createIndex(row,column,childNode);
    }
    return QModelIndex();
}

QModelIndex vsPropertyModel::parent(const QModelIndex &child) const
{

}

int vsPropertyModel::rowCount(const QModelIndex &parent) const
{

}

int vsPropertyModel::columnCount(const QModelIndex &parent) const
{

}

QVariant vsPropertyModel::data(const QModelIndex &index, int role) const
{

}

vsPropertyNode *vsPropertyModel::rootNode() const
{
    return m_rootNode;
}

QModelIndex vsPropertyModel::indexForNode(vsPropertyNode *pNode)
{
    if(pNode == m_rootNode)
        return QModelIndex();
    int row = rowForNode(pNode);
    int column = 0;
    return createIndex(row,column,pNode);
}

vsPropertyNode *vsPropertyModel::nodeForIndex(const QModelIndex &index) const
{
    if(index.isValid())
        return static_cast<vsPropertyNode*>(index.internalPointer());
    return m_rootNode;
}

int vsPropertyModel::rowForNode(vsPropertyNode *node) const
{
    return node->m_parentNode->m_childNodes.indexOf(node);
}

void vsPropertyModel::setRootNode(vsPropertyNode *rootNode)
{
    if (m_rootNode == rootNode)
        return;

    m_rootNode = rootNode;
    emit rootNodeChanged(m_rootNode);
}
