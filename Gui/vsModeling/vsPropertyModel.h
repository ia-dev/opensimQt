/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSPROPERTYMODEL_H
#define VSPROPERTYMODEL_H

#include "vsPropertyNode.h"

#include <QAbstractItemModel>
#include <QObject>

class vsPropertyModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(vsPropertyNode* rootNode READ rootNode WRITE setRootNode NOTIFY rootNodeChanged)
    vsPropertyNode* m_rootNode;

public:
    vsPropertyModel();



    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    vsPropertyNode* rootNode() const;
public slots:
    void setRootNode(vsPropertyNode* rootNode);
signals:
    void rootNodeChanged(vsPropertyNode* rootNode);
};

#endif // VSPROPERTYMODEL_H
