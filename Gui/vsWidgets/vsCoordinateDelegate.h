/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020                                                  *
 *                                                                         *
 *   Authors:  Yasser Grimes                                               *
 *   Develeped as a Graduation Project                                     *
 *   School: National School of Computer Science Sidi-Bel-Abbes Algeria    *
 *   Supervisor: Bendaoud Faysal                                           *
 ***************************************************************************/
#ifndef VSCOORDINATEDELEGATE_H
#define VSCOORDINATEDELEGATE_H

#include <QWidget>
#include <OpenSim.h>

namespace Ui {
class vsCoordinateDelegate;
}

class vsCoordinateDelegate : public QWidget
{
    Q_OBJECT

public:
    explicit vsCoordinateDelegate(OpenSim::Coordinate& connectedCoordinate,OpenSim::Model *model,QWidget *parent = nullptr);
    ~vsCoordinateDelegate();

private:
    Ui::vsCoordinateDelegate *ui;
    OpenSim::Coordinate m_connectedCoordinate;
    OpenSim::Model *m_model;
};

#endif // VSCOORDINATEDELEGATE_H
