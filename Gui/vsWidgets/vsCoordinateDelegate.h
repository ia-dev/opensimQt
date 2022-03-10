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
#include <QStateMachine>
#include <OpenSim.h>

namespace Ui {
class vsCoordinateDelegate;
}

struct CoordinateInfo;

class vsCoordinateDelegate : public QWidget
{
    Q_OBJECT

public:
    explicit vsCoordinateDelegate(int coordinate_index/*OpenSim::Coordinate& coordinate */,OpenSim::Model *model,QWidget *parent = nullptr);
    ~vsCoordinateDelegate();
    void updateModelGeometries();
    CoordinateInfo getCurrentCoordinateInfo();
    CoordinateInfo getDefaultCoordinateInfo();

    void setCoordinateInfo(CoordinateInfo info);

private slots:
    void on_valueSpinBox_valueChanged(double arg1);

    void on_lockButton_toggled(bool checked);

    void on_clampButton_toggled(bool checked);

    void on_seekSlider_valueChanged(int value);

    void on_speedSpinBox_valueChanged(double arg1);

private:
    Ui::vsCoordinateDelegate *ui;
    OpenSim::Coordinate *m_connectedCoordinate;
    OpenSim::Model *m_model;
    //QStateMachine coordinateState;
    bool m_isLoading = true;
    //using the index instead of the Coordinate untill the OpenSim bug is fixed.
    int m_coordinateIndex;
};

#endif // VSCOORDINATEDELEGATE_H
