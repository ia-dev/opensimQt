/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari                                           *
 *                                                                         *
 ***************************************************************************/
#ifndef VSVTKWINDOW_H
#define VSVTKWINDOW_H

#include <QObject>
#include <vtkRenderWindow.h>
#include <QVTKOpenGLWindow.h>

class vsVTKWindow
{
    Q_OBJECT
public:
    vsVTKWindow();
};

#endif // VSVTKWINDOW_H
