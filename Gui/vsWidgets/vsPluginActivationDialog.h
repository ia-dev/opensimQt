/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSPLUGINACTIVATIONDIALOG_H
#define VSPLUGINACTIVATIONDIALOG_H

#include <QDialog>

namespace Ui {
class vsPluginActivationDialog;
}

class vsPluginActivationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit vsPluginActivationDialog(QString fileName,QWidget *parent = nullptr);
    ~vsPluginActivationDialog();

private slots:
    void on_checkBox_toggled(bool checked);

private:
    Ui::vsPluginActivationDialog *ui;
};

#endif // VSPLUGINACTIVATIONDIALOG_H
