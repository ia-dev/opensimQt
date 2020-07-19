/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsPluginActivationDialog.h"
#include "ui_vsPluginActivationDialog.h"

vsPluginActivationDialog::vsPluginActivationDialog(QString fileName,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vsPluginActivationDialog)
{
    ui->setupUi(this);
    ui->loadedLabel->setText("library "+fileName+" has been loaded successfully !");
}

vsPluginActivationDialog::~vsPluginActivationDialog()
{
    delete ui;
}

void vsPluginActivationDialog::on_checkBox_toggled(bool checked)
{
    if(checked){
        //TODO register the plugin to load on entry
    }
}
