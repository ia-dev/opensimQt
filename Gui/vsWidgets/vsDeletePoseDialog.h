/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSDELETEPOSEDIALOG_H
#define VSDELETEPOSEDIALOG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class vsDeletePoseDialog;
}

class vsDeletePoseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit vsDeletePoseDialog(QStringList kyes,QWidget *parent = nullptr);
    ~vsDeletePoseDialog();
    QStringList getSelected();

private:
    QStringList m_keys;
    Ui::vsDeletePoseDialog *ui;
    QStringListModel kyesModel;
};

#endif // VSDELETEPOSEDIALOG_H
