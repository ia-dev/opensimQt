/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari                                           *
 *                                                                         *
 ***************************************************************************/
#include "vsInteractorStyle.h"

vsInteractorStyle::vsInteractorStyle(vsVisualizerVTK *visulizerVTK):vtkInteractorStyleTrackballActor(),m_visualizer(visulizerVTK)
{

}

void vsInteractorStyle::OnLeftButtonDown()
{

}
