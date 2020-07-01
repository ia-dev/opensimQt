/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020                                                  *
 *                                                                         *
 *   Authors:  Yasser Grimes                                               *
 *   Develeped as a Graduation Project                                     *
 *   School: National School of Computer Science Sidi-Bel-Abbes Algeria    *
 *   Supervisor: Bendaoud Faysal                                           *
 ***************************************************************************/
#include "vsCoordinateDelegate.h"
#include "ui_vsCoordinateDelegate.h"

vsCoordinateDelegate::vsCoordinateDelegate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsCoordinateDelegate)
{
    ui->setupUi(this);
}

vsCoordinateDelegate::~vsCoordinateDelegate()
{
    delete ui;
}
