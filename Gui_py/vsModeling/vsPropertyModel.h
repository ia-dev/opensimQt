/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSPROPERTYMODEL_H
#define VSPROPERTYMODEL_H

#include "vsNavigatorNode.h"

#include <QObject>
#include <QStandardItemModel>

class vsPropertyModel : public QStandardItemModel
{
    Q_OBJECT
    Q_PROPERTY(vsNavigatorNode* selectedNavigarorNode READ selectedNavigarorNode WRITE setSelectedNavigarorNode NOTIFY selectedNavigarorNodeChanged)

    vsNavigatorNode* m_selectedNavigarorNode;

public:
    vsPropertyModel(QObject *parent);

    vsNavigatorNode* selectedNavigarorNode() const;


    QStandardItem *m_rootItem;
    QStandardItem *m_propertiesItem;
    QStandardItem *m_socketsItem;
    QStandardItem *m_appearancexItem;


public slots:

    void setSelectedNavigarorNode(vsNavigatorNode* selectedNavigarorNode);

signals:

    void selectedNavigarorNodeChanged(vsNavigatorNode* selectedNavigarorNode);
};

#endif // VSPROPERTYMODEL_H
