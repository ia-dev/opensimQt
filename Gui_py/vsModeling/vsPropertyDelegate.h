/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSPROPERTYDELEGATE_H
#define VSPROPERTYDELEGATE_H

#include <QItemDelegate>
#include <QObject>

class vsPropertyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    vsPropertyDelegate(QObject *parent = nullptr);

    // QAbstractItemDelegate interface
public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

};

#endif // VSPROPERTYDELEGATE_H
