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

class vsInteractorStyle : public vtkInteractorStyleTrackballActor
{
public:
    vsInteractorStyle(vsVisualizerVTK* visualizerVTK);

    // vtkInteractorStyle interface
public:
    virtual void OnLeftButtonDown() override;

private:
    vsVisualizerVTK *m_visualizer;

};

#endif // VSINTERACTORSTYLE_H
