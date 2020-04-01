/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsPropertyDelegate.h"

vsPropertyDelegate::vsPropertyDelegate(QObject *parent)
{

}

QWidget *vsPropertyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return nullptr;
}

void vsPropertyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

void vsPropertyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}
