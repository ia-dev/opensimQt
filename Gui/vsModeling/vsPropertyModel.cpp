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
    QStandardItem  *emptyItem = new QStandardItem("");
    emptyItem->setBackground(Qt::lightGray);
   setColumnCount(2);
   //setting the properties
   m_propertiesItem = new QStandardItem();
   m_propertiesItem->setText("Properties");
   m_propertiesItem->setBackground(Qt::lightGray);
   appendRow(QList<QStandardItem*>()<< m_propertiesItem << emptyItem);
   //setting the sockets

   m_socketsItem = new QStandardItem();
   m_socketsItem->setText("Sockets");
   m_socketsItem->setBackground(Qt::lightGray);
   appendRow(QList<QStandardItem*>()<< m_socketsItem << emptyItem->clone());

   //setting the appearance
   m_appearancexItem = new QStandardItem();
   m_appearancexItem->setText("Appearance");
   m_appearancexItem->setBackground(Qt::lightGray);
   appendRow(QList<QStandardItem*>()<< m_appearancexItem << emptyItem->clone());

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
