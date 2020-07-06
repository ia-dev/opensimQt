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

#include <QAction>
#include <QWidget>
#include <qmenu.h>

namespace Ui {
class vsCoordinatesWidget;
}

typedef struct CoordinateInfo
{
    double m_value;
    double m_speed;
    bool m_isLocked;
    bool m_isClamped;
    CoordinateInfo(double value,double speed, double isLocked , double isClamped) :
    m_value(value),m_speed(speed),m_isLocked(isLocked),m_isClamped(isClamped)
    {
    }
};

class vsCoordinatesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit vsCoordinatesWidget(QWidget *parent = nullptr);
    ~vsCoordinatesWidget();
    void addCoordinateDelegate(vsCoordinateDelegate *delegate);
    void removeCoordinateDelegate(vsCoordinateDelegate *delegate);
    void initializeWidgetForNewModel();
    void loadPose(QString k);
    QList<QAction*> getPosesActions();
    QMenu* getPosesMenu();

private:
    Ui::vsCoordinatesWidget *ui;
    QMap<QString,QList<CoordinateInfo>*> poses;
    QList<vsCoordinateDelegate *> delegates;
    QList<QAction*> m_posesActions;
    QMenu *retMenu;
};

#endif // VSCOORDINATESWIDGET_H
