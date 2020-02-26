#!/usr/bin/env python

import time

import vtk


class MyCallback(object):
    def __init__(self, ren):
        self.ren = ren

    def __call__(self, caller, ev):
        pos = self.ren.GetActiveCamera().GetPosition()
        print(', '.join('{0:0.6g}'.format(i) for i in pos))


def main():
    colors = vtk.vtkNamedColors()

    # Create the pipeline
    cone = vtk.vtkConeSource()
    cone.SetHeight(3.0)
    cone.SetRadius(1.0)
    cone.SetResolution(10)
    coneMapper = vtk.vtkPolyDataMapper()
    coneMapper.SetInputConnection(cone.GetOutputPort())
    coneActor = vtk.vtkActor()
    coneActor.SetMapper(coneMapper)
    coneActor.GetProperty().SetColor(colors.GetColor3d("Banana"))

    ren1 = vtk.vtkRenderer()
    ren1.AddActor(coneActor)
    ren1.SetBackground(colors.GetColor3d('MidnightBlue'))
    # Add the observer here
    ren1.AddObserver("StartEvent", MyCallback(ren1))
    renWin = vtk.vtkRenderWindow()
    renWin.AddRenderer(ren1)
    renWin.SetSize(300, 300)

    # Loop over 360 degrees and render the cone each time.
    for i in range(0, 360):
        time.sleep(0.03)
        renWin.Render()
        ren1.GetActiveCamera().Azimuth(1)


if __name__ == '__main__':
    main()
