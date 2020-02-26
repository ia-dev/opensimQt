#!/usr/bin/env python

"""
Based on clipPlane.tcl
"""

import vtk


def main():
    colors = vtk.vtkNamedColors()

    # Create the RenderWindows, Renderers and Render Window Interactors
    # We need two render windows as we are writing out a before and after image.
    renL = vtk.vtkRenderer()
    renR = vtk.vtkRenderer()
    renWinL = vtk.vtkRenderWindow()
    renWinL.AddRenderer(renL)
    renL.SetBackground(colors.GetColor3d('AntiqueWhite'))
    renWinR = vtk.vtkRenderWindow()
    renWinR.AddRenderer(renR)
    renR.SetBackground(colors.GetColor3d('AntiqueWhite'))
    irenL = vtk.vtkRenderWindowInteractor()
    irenL.SetRenderWindow(renWinL)
    irenR = vtk.vtkRenderWindowInteractor()
    irenR.SetRenderWindow(renWinR)

    # Create a plane.
    plane = vtk.vtkPlaneSource()
    plane.SetResolution(7, 6)

    planeTris = vtk.vtkTriangleFilter()
    planeTris.SetInputConnection(plane.GetOutputPort())

    halfPlane = vtk.vtkPlane()
    halfPlane.SetOrigin(-0.03, -0.03, 0)
    halfPlane.SetNormal(1, 0.4, 0)

    planeClipper = vtk.vtkClipPolyData()
    planeClipper.GenerateClipScalarsOn()
    planeClipper.SetClipFunction(halfPlane)
    planeClipper.SetInputConnection(planeTris.GetOutputPort())

    planeClipperMapper = vtk.vtkPolyDataMapper()
    planeClipperMapper.SetInputConnection(planeClipper.GetOutputPort())
    planeClipperMapper.ScalarVisibilityOff()

    planeClipperActor = vtk.vtkActor()
    planeClipperActor.SetMapper(planeClipperMapper)
    planeClipperActor.GetProperty().SetDiffuseColor(colors.GetColor3d('Black'))
    planeClipperActor.GetProperty().SetRepresentationToWireframe()
    planeClipperActor.GetProperty().SetLineWidth(3)

    # Get the data that has been clipped away.
    planeClipper1 = vtk.vtkClipPolyData()
    planeClipper1.GenerateClipScalarsOn()
    planeClipper1.SetClipFunction(halfPlane)
    planeClipper1.SetInputConnection(planeTris.GetOutputPort())
    planeClipper1.InsideOutOn()

    planeClipperMapper1 = vtk.vtkPolyDataMapper()
    planeClipperMapper1.SetInputConnection(planeClipper1.GetOutputPort())
    planeClipperMapper1.ScalarVisibilityOff()

    planeClipperActor1 = vtk.vtkActor()
    planeClipperActor1.SetMapper(planeClipperMapper1)
    planeClipperActor1.GetProperty().SetDiffuseColor(colors.GetColor3d('LightSlateGray'))
    planeClipperActor1.GetProperty().SetRepresentationToWireframe()
    planeClipperActor1.GetProperty().SetLineWidth(3)

    planeCutter = vtk.vtkCutter()
    planeCutter.SetCutFunction(halfPlane)
    planeCutter.SetInputConnection(planeTris.GetOutputPort())
    planeCutter.SetValue(0, 0)

    # Add in a plane cutter to highlight the boundary where the plane is clipped.
    planeTube = vtk.vtkTubeFilter()
    planeTube.SetInputConnection(planeTris.GetOutputPort())
    planeTube.SetNumberOfSides(8)
    planeTube.SetRadius(.005)

    planeCutterTube = vtk.vtkTubeFilter()
    planeCutterTube.SetInputConnection(planeCutter.GetOutputPort())
    planeCutterTube.SetNumberOfSides(8)
    planeCutterTube.SetRadius(.005)

    planeCutterMapper = vtk.vtkPolyDataMapper()
    planeCutterMapper.SetInputConnection(planeCutterTube.GetOutputPort())
    planeCutterMapper.ScalarVisibilityOff()

    planeCutterActor = vtk.vtkActor()
    planeCutterActor.SetMapper(planeCutterMapper)
    planeCutterActor.GetProperty().SetDiffuseColor(colors.GetColor3d('Maroon'))

    # Add the actors to the renderer, set the background and size
    renL.AddActor(planeClipperActor)
    renL.AddActor(planeClipperActor1)
    renL.AddActor(planeCutterActor)
    renL.ResetCamera()
    renL.GetActiveCamera().Zoom(1.35)
    renR.AddActor(planeClipperActor)
    renR.AddActor(planeCutterActor)
    renR.ResetCamera()
    renR.GetActiveCamera().Zoom(1.2)
    renWinL.SetSize(1002, 1002)
    renWinL.Render()
    renWinR.SetSize(1002, 1002)
    renWinR.Render()

    WriteImage('Figure9-9a.png', renWinL, rgba=False)
    WriteImage('Figure9-9b.png', renWinR, rgba=False)

    # irenL.Initialize()
    # irenR.Initialize()
    # irenL.Start()
    # irenR.Start()

def WriteImage(fileName, renWin, rgba=True):
    """
    Write the render window view to an image file.

    Image types supported are:
     BMP, JPEG, PNM, PNG, PostScript, TIFF.
    The default parameters are used for all writers, change as needed.

    :param fileName: The file name, if no extension then PNG is assumed.
    :param renWin: The render window.
    :param rgba: Used to set the buffer type.
    :return:
    """

    import os

    if fileName:
        # Select the writer to use.
        path, ext = os.path.splitext(fileName)
        ext = ext.lower()
        if not ext:
            ext = '.png'
            fileName = fileName + ext
        if ext == '.bmp':
            writer = vtk.vtkBMPWriter()
        elif ext == '.jpg':
            writer = vtk.vtkJPEGWriter()
        elif ext == '.pnm':
            writer = vtk.vtkPNMWriter()
        elif ext == '.ps':
            if rgba:
                rgba = False
            writer = vtk.vtkPostScriptWriter()
        elif ext == '.tiff':
            writer = vtk.vtkTIFFWriter()
        else:
            writer = vtk.vtkPNGWriter()

        windowto_image_filter = vtk.vtkWindowToImageFilter()
        windowto_image_filter.SetInput(renWin)
        windowto_image_filter.SetScale(1)  # image quality
        if rgba:
            windowto_image_filter.SetInputBufferTypeToRGBA()
        else:
            windowto_image_filter.SetInputBufferTypeToRGB()
            # Read from the front buffer.
            windowto_image_filter.ReadFrontBufferOff()
            windowto_image_filter.Update()

        writer.SetFileName(fileName)
        writer.SetInputConnection(windowto_image_filter.GetOutputPort())
        writer.Write()
    else:
        raise RuntimeError('Need a filename.')

if __name__ == '__main__':
    main()
