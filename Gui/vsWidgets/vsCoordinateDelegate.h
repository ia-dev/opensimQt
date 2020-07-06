/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
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
    explicit vsCoordinateDelegate(int coordinate_index/*OpenSim::Coordinate& coordinate */,OpenSim::Model *model,QWidget *parent = nullptr);
    ~vsCoordinateDelegate();
    void updateModelGeometries();

private slots:
    void on_valueSpinBox_valueChanged(double arg1);

    void on_lockButton_toggled(bool checked);

    void on_clampButton_toggled(bool checked);

    void on_seekSlider_valueChanged(int value);

private:
    Ui::vsCoordinateDelegate *ui;
    OpenSim::Coordinate *m_connectedCoordinate;
    OpenSim::Model *m_model;
    //using the index instead of the Coordinate untill the OpenSim bug is fixed.
    int m_coordinateIndex;
};

#endif // VSCOORDINATEDELEGATE_H
