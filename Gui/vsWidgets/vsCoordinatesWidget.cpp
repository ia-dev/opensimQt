/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsCoordinatesWidget.h"
#include "ui_vsCoordinatesWidget.h"

vsCoordinatesWidget::vsCoordinatesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsCoordinatesWidget)
{
    ui->setupUi(this);

    QAction *newAction = new QAction("New...",this);
    QAction *setDefaultAction = new QAction("Set Default",this);
    QAction *deleteAction = new QAction("Delete...",this);
    m_posesActions << newAction << setDefaultAction << deleteAction;
}

vsCoordinatesWidget::~vsCoordinatesWidget()
{
    delete ui;
}

void vsCoordinatesWidget::addCoordinateDelegate(vsCoordinateDelegate *delegate)
{
    ui->listLayout->addWidget(delegate);
}

void vsCoordinatesWidget::removeCoordinateDelegate(vsCoordinateDelegate *delegate)
{
    ui->listLayout->removeWidget(delegate);
}

QList<QAction *> vsCoordinatesWidget::getPosesActions()
{
    return m_posesActions;
}

QMenu* vsCoordinatesWidget::getPosesMenu()
{
    retMenu = new QMenu(this);
    retMenu->addActions(m_posesActions);
    return retMenu;
}
