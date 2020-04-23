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
    QColor topLevelColor("#cccccc");
    QStandardItem  *emptyItem = new QStandardItem("");
    emptyItem->setBackground(topLevelColor);
   setColumnCount(2);
   setHorizontalHeaderLabels(QStringList() << "Property" << "Value");
   //setting the properties
   m_propertiesItem = new QStandardItem();
   m_propertiesItem->setText("Properties");
   m_propertiesItem->setBackground(topLevelColor);
   appendRow(QList<QStandardItem*>()<< m_propertiesItem << emptyItem);
   //setting the sockets

   m_socketsItem = new QStandardItem();
   m_socketsItem->setText("Sockets");
   m_socketsItem->setBackground(topLevelColor);
   appendRow(QList<QStandardItem*>()<< m_socketsItem << emptyItem->clone());

   //setting the appearance
   m_appearancexItem = new QStandardItem();
   m_appearancexItem->setText("Appearance");
   m_appearancexItem->setBackground(topLevelColor);
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
    if(m_propertiesItem->rowCount()>0)m_propertiesItem->removeRows(0,m_propertiesItem->rowCount());
    if(m_socketsItem->rowCount()>0)m_socketsItem->removeRows(0,m_socketsItem->rowCount());
    if(m_appearancexItem->rowCount()>0)m_appearancexItem->removeRows(0,m_appearancexItem->rowCount());
    m_selectedNavigarorNode->setupPropertiesModel(this);
    emit layoutChanged();
    emit selectedNavigarorNodeChanged(m_selectedNavigarorNode);
}
