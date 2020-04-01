/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsPropertyModel.h"


vsPropertyModel::vsPropertyModel(QObject *parent):QStandardItemModel(parent)
{

   m_propertiesItem = new QStandardItem("Properties");
   m_propertiesItem->setBackground(Qt::lightGray);
   appendRow(m_propertiesItem);

}

vsNavigatorNode *vsPropertyModel::selectedNavigarorNode() const
{
    return m_selectedNavigarorNode;
}



void vsPropertyModel::setSelectedNavigarorNode(vsNavigatorNode *selectedNavigarorNode)
{
    if (m_selectedNavigarorNode == selectedNavigarorNode)
        return;

    m_selectedNavigarorNode = selectedNavigarorNode;
    emit selectedNavigarorNodeChanged(m_selectedNavigarorNode);
}
