/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari                                           *
 *                                                                         *
 ***************************************************************************/
#include "vsInteractorStyle.h"

#include <QDebug>
vtkStandardNewMacro(vsInteractorStyle);

vsInteractorStyle::vsInteractorStyle():QVTKInteractor(),m_visualizer(nullptr)
{

}


void vsInteractorStyle::setVisualizer(vsVisualizerVTK *visualizer)
{
    m_visualizer = visualizer;
}

void vsInteractorStyle::LeftButtonPressEvent()
{
    vtkRenderWindowInteractor::LeftButtonPressEvent();
    qDebug() << "actor clicked" ;
}
