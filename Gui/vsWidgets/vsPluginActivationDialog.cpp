/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsPluginActivationDialog.h"
#include "ui_vsPluginActivationDialog.h"

#include <vsTools/vsOpenSimTools.h>

vsPluginActivationDialog::vsPluginActivationDialog(QString fileName,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vsPluginActivationDialog),
    m_fileName(fileName)
{
    ui->setupUi(this);
    setWindowTitle("Library Loaded");
    ui->loadedLabel->setText("library "+fileName+" has been loaded successfully !");
    QStringList onEntryPlugins = vsOpenSimTools::tools->openSimSettings
            ->value("plugins/loadOnEntry",QStringList()).toStringList();
    ui->checkBox->setCheckState(onEntryPlugins.contains(m_fileName)?Qt::CheckState::Checked:Qt::CheckState::Unchecked);
}

vsPluginActivationDialog::~vsPluginActivationDialog()
{
    delete ui;
}

void vsPluginActivationDialog::on_checkBox_toggled(bool checked)
{
    QStringList onEntryPlugins = vsOpenSimTools::tools->openSimSettings
            ->value("plugins/loadOnEntry",QStringList()).toStringList();

    if(checked){
        //TODO register the plugin to load on entry
        if(onEntryPlugins.contains(m_fileName)) return;
        onEntryPlugins << m_fileName;
        vsOpenSimTools::tools->openSimSettings
                    ->setValue("plugins/loadOnEntry",onEntryPlugins);
    }else{
        if(!onEntryPlugins.contains(m_fileName)) return;
        onEntryPlugins.removeOne(m_fileName);
        vsOpenSimTools::tools->openSimSettings
                    ->setValue("plugins/loadOnEntry",onEntryPlugins);
    }
}
