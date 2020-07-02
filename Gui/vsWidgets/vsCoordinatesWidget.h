/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
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
