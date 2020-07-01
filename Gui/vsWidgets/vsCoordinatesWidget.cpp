/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020                                                  *
 *                                                                         *
 *   Authors:  Yasser Grimes                                               *
 *   Develeped as a Graduation Project                                     *
 *   School: National School of Computer Science Sidi-Bel-Abbes Algeria    *
 *   Supervisor: Bendaoud Faysal                                           *
 ***************************************************************************/
#include "vsCoordinatesWidget.h"
#include "ui_vsCoordinatesWidget.h"

vsCoordinatesWidget::vsCoordinatesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsCoordinatesWidget)
{
    ui->setupUi(this);
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
