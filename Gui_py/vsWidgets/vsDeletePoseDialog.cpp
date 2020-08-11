/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsDeletePoseDialog.h"
#include "ui_vsDeletePoseDialog.h"

vsDeletePoseDialog::vsDeletePoseDialog(QStringList keys,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vsDeletePoseDialog)
{
    ui->setupUi(this);
    m_keys.append(keys);
    m_keys.removeOne("default");
    kyesModel.setStringList(m_keys);
    ui->listView->setModel(&kyesModel);
    //ui->listView->selectionModel()->selectedRows()
}

vsDeletePoseDialog::~vsDeletePoseDialog()
{
    delete ui;
}

QStringList vsDeletePoseDialog::getSelected()
{
    QStringList retList;
    foreach (auto index, ui->listView->selectionModel()->selectedRows()) {
        retList << m_keys.value(index.row());
    }
    return QStringList(retList);
}
