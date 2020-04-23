/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari                                           *
 *                                                                         *
 ***************************************************************************/
#ifndef VSINTERACTORSTYLE_H
#define VSINTERACTORSTYLE_H

#include "vsVisualizerVTK.h"

#include <vtkInteractorStyleTrackballActor.h>
#include <vtkObjectFactory.h>
#include <QVTKInteractor.h>
class vsInteractorStyle : public QVTKInteractor
{
public:
    static vsInteractorStyle* New();
    vsInteractorStyle();
    vtkTypeMacro(vsInteractorStyle,QVTKInteractor)

    // vtkInteractorStyle interface
public:

    void setVisualizer(vsVisualizerVTK *visualizer);

private:
    vsVisualizerVTK *m_visualizer;


    // vtkRenderWindowInteractor interface
public:
    virtual void LeftButtonPressEvent() override;
};


#endif // VSINTERACTORSTYLE_H
