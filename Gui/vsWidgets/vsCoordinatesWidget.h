/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020                                                  *
 *                                                                         *
 *   Authors:  Yasser Grimes                                               *
 *   Develeped as a Graduation Project                                     *
 *   School: National School of Computer Science Sidi-Bel-Abbes Algeria    *
 *   Supervisor: Bendaoud Faysal                                           *
 ***************************************************************************/
#ifndef VSCOORDINATESWIDGET_H
#define VSCOORDINATESWIDGET_H

#include "vsCoordinateDelegate.h"

#include <QWidget>

namespace Ui {
class vsCoordinatesWidget;
}

class vsCoordinatesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit vsCoordinatesWidget(QWidget *parent = nullptr);
    ~vsCoordinatesWidget();
    void addCoordinateDelegate(vsCoordinateDelegate *delegate);
    void removeCoordinateDelegate(vsCoordinateDelegate *delegate);

private:
    Ui::vsCoordinatesWidget *ui;
};

#endif // VSCOORDINATESWIDGET_H
