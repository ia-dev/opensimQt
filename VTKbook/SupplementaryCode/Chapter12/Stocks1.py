#!/usr/bin/env python
import os

import vtk


def main():
    colors = vtk.vtkNamedColors()

    fileNames = ['GE.vtk', 'GM.vtk', 'IBM.vtk', 'DEC.vtk']

    # Set up the stocks
    renderers = list()
    topRenderer = vtk.vtkRenderer()
    bottomRenderer = vtk.vtkRenderer()
    renderers.append(topRenderer)
    renderers.append(bottomRenderer)

    # create the outline
    apf = vtk.vtkAppendPolyData()
    olf = vtk.vtkOutlineFilter()
    olf.SetInputConnection(apf.GetOutputPort())
    outlineMapper = vtk.vtkPolyDataMapper()
    outlineMapper.SetInputConnection(olf.GetOutputPort())
    outlineActor = vtk.vtkActor()
    outlineActor.SetMapper(outlineMapper)

    zPosition = 0.0
    for fn in fileNames:
        zPosition = AddStock(renderers, apf, fn,
                             os.path.basename((os.path.splitext(fn)[0])), zPosition)

    # Setup the render window and interactor.
    renderWindow = vtk.vtkRenderWindow()
    renderWindow.AddRenderer(renderers[0])
    renderWindow.AddRenderer(renderers[1])

    renderWindowInteractor = vtk.vtkRenderWindowInteractor()
    renderWindowInteractor.SetRenderWindow(renderWindow)

    renderers[0].SetViewport(0.0, 0.4, 1.0, 1.0)
    renderers[1].SetViewport(0.0, 0.0, 1.0, 0.4)

    renderers[0].GetActiveCamera().SetViewAngle(5.0)
    renderers[0].ResetCamera()
    renderers[0].GetActiveCamera().Zoom(1.4)
    renderers[0].ResetCameraClippingRange()
    renderers[0].SetBackground(colors.GetColor3d("SteelBlue"))

    renderers[1].GetActiveCamera().SetViewUp(0, 0, -1)
    renderers[1].GetActiveCamera().SetPosition(0, 1, 0)
    renderers[1].GetActiveCamera().SetViewAngle(5.0)
    renderers[1].ResetCamera()
    renderers[1].GetActiveCamera().Zoom(2.2)
    renderers[1].ResetCameraClippingRange()
    renderers[1].SetBackground(colors.GetColor3d("LightSteelBlue"))

    renderers[0].AddActor(outlineActor)
    renderers[1].AddActor(outlineActor)

    renderWindow.SetSize(500, 800)
    # renderWindow.SetSize(1000, 1600)
    renderWindow.Render()

    renderWindowInteractor.Start()
    WriteImage('Figure12-10.png', renderWindow, rgba=False)


def AddStock(renderers, apf, filename, name, zPosition):
    print("Adding", name)

    # Read the data
    PolyDataRead = vtk.vtkPolyDataReader()
    PolyDataRead.SetFileName(filename)
    PolyDataRead.Update()

    TubeFilter = vtk.vtkTubeFilter()
    TubeFilter.SetInputConnection(PolyDataRead.GetOutputPort())
    TubeFilter.SetNumberOfSides(8)
    TubeFilter.SetRadius(0.5)
    TubeFilter.SetRadiusFactor(10000)

    Transform = vtk.vtkTransform()
    Transform.Translate(0, 0, zPosition)
    Transform.Scale(0.15, 1, 1)

    TransformFilter = vtk.vtkTransformPolyDataFilter()
    TransformFilter.SetInputConnection(TubeFilter.GetOutputPort())
    TransformFilter.SetTransform(Transform)

    apf.AddInputData(TransformFilter.GetOutput())

    # Create the labels.
    TextSrc = vtk.vtkVectorText()
    TextSrc.SetText(name)
    numberOfPoints = PolyDataRead.GetOutput().GetNumberOfPoints()

    nameIndex = int((numberOfPoints - 1) * 0.8)

    nameLocation = PolyDataRead.GetOutput().GetPoint(nameIndex)

    x = nameLocation[0] * 0.15
    y = nameLocation[1] + 5.0
    z = zPosition

    for r in range(0, len(renderers)):
        LabelMapper = vtk.vtkPolyDataMapper()
        LabelMapper.SetInputConnection(TextSrc.GetOutputPort())

        LabelActor = vtk.vtkFollower()
        LabelActor.SetMapper(LabelMapper)
        LabelActor.SetPosition(x, y, z)
        LabelActor.SetScale(2, 2, 2)
        LabelActor.SetOrigin(TextSrc.GetOutput().GetCenter())

        # Increment zPosition.
        zPosition += 8.0

        StockMapper = vtk.vtkPolyDataMapper()
        StockMapper.SetInputConnection(TransformFilter.GetOutputPort())
        StockMapper.SetScalarRange(0, 8000)
        StockActor = vtk.vtkActor()
        StockActor.SetMapper(StockMapper)

        renderers[r].AddActor(StockActor)
        renderers[r].AddActor(LabelActor)
        LabelActor.SetCamera(renderers[r].GetActiveCamera())
    return zPosition


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
