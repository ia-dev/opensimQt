#!/usr/bin/env python

import os

import colors
import sliceorder
import vtk
from WindowLevelInterface import WindowLevelInterface

VTK_TEXTBOOK_DATA = os.environ.get("VTK_TEXTBOOK_DATA")
if VTK_TEXTBOOK_DATA is None:
    s = "Could not find the VTK Textbook data folder.\n"
    s += "Please set the environment variable:\n"
    s += " VTK_TEXTBOOK_DATA\n to the VTK Textbook data folder.\n"
    raise Exception(s)

# Now create the RenderWindow, Renderer and Interactor
#
ren = vtk.vtkRenderer()
ren2 = vtk.vtkRenderer()
ren3 = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
renWin.AddRenderer(ren2)
renWin.AddRenderer(ren3)

iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

sliceNumber = 40
ROWS = 470
COLUMNS = 500
GREYSTUDY = VTK_TEXTBOOK_DATA + "/frog/frog"

SEGMENTSTUDY = VTK_TEXTBOOK_DATA + "/frog/frogTissue"

START_SLICE = 1
END_SLICE = 138
PIXEL_SIZE = 1
SPACING = 1.5
centerX = (COLUMNS / 2)
centerY = (ROWS / 2)
centerZ = (END_SLICE - START_SLICE) / 2
endX = (COLUMNS - 1)
endY = (ROWS - 1)
endZ = (END_SLICE - 1)
originX = (COLUMNS / 2.0) * PIXEL_SIZE * -1.0
originY = (ROWS / 2.0) * PIXEL_SIZE * -1.0
SLICE_ORDER = "hfsi"

greyReader = vtk.vtkPNMReader()
greyReader.SetFilePrefix(GREYSTUDY)
greyReader.SetDataSpacing(PIXEL_SIZE, PIXEL_SIZE, SPACING)
greyReader.SetDataVOI(0, endX, 0, endY, sliceNumber, sliceNumber)
greyReader.SetDataExtent(0, endX, 0, endY, sliceNumber, sliceNumber)
greyReader.DebugOn()

greyPadder = vtk.vtkImageConstantPad()
greyPadder.SetInputConnection(greyReader.GetOutputPort())
greyPadder.SetOutputWholeExtent(0, 511, 0, 511, 0, 0)
greyPadder.SetConstant(0)

greyPlane = vtk.vtkPlaneSource()

greyTransform = vtk.vtkTransformPolyDataFilter()
if SLICE_ORDER == "si":
    greyTransform.SetTransform(sliceorder.si)
elif SLICE_ORDER == "tis":
    greyTransform.SetTransform(sliceorder.tis)
elif SLICE_ORDER == "ap":
    greyTransform.SetTransform(sliceorder.ap)
elif SLICE_ORDER == "pa":
    greyTransform.SetTransform(sliceorder.pa)
elif SLICE_ORDER == "lr":
    greyTransform.SetTransform(sliceorder.lr)
elif SLICE_ORDER == "rl":
    greyTransform.SetTransform(sliceorder.rl)
elif SLICE_ORDER == "hf":
    greyTransform.SetTransform(sliceorder.hf)
elif SLICE_ORDER == "hfsi":
    greyTransform.SetTransform(sliceorder.hfsi)
elif SLICE_ORDER == "hfis":
    greyTransform.SetTransform(sliceorder.hfis)
elif SLICE_ORDER == "hfap":
    greyTransform.SetTransform(sliceorder.hfap)
elif SLICE_ORDER == "hfpa":
    greyTransform.SetTransform(sliceorder.hfpa)
elif SLICE_ORDER == "hflr":
    greyTransform.SetTransform(sliceorder.hflr)
elif SLICE_ORDER == "hfrl":
    greyTransform.SetTransform(sliceorder.hfrl)
else:
    s = "No such transform exists."
    raise Exception(s)
greyTransform.SetInputConnection(greyPlane.GetOutputPort())

greyNormals = vtk.vtkPolyDataNormals()
greyNormals.SetInputConnection(greyTransform.GetOutputPort())
greyNormals.FlipNormalsOff()

wllut = vtk.vtkWindowLevelLookupTable()
wllut.SetWindow(255)
wllut.SetLevel(128)
wllut.SetTableRange(0, 255)
wllut.Build()

greyMapper = vtk.vtkPolyDataMapper()
greyMapper.SetInputConnection(greyPlane.GetOutputPort())
greyMapper.ImmediateModeRenderingOn()

greyTexture = vtk.vtkTexture()
greyTexture.SetInputConnection(greyPadder.GetOutputPort())
greyTexture.SetLookupTable(wllut)
greyTexture.MapColorScalarsThroughLookupTableOn()
greyTexture.InterpolateOn()

greyActor = vtk.vtkActor()
greyActor.SetMapper(greyMapper)
greyActor.SetTexture(greyTexture)

segmentReader = vtk.vtkPNMReader()
segmentReader.SetFilePrefix(SEGMENTSTUDY)
segmentReader.SetDataSpacing(PIXEL_SIZE, PIXEL_SIZE, SPACING)
segmentReader.SetDataVOI(0, endX, 0, endY, sliceNumber, sliceNumber)
segmentReader.SetDataExtent(0, endX, 0, endY, sliceNumber, sliceNumber)

segmentPadder = vtk.vtkImageConstantPad()
segmentPadder.SetInputConnection(segmentReader.GetOutputPort())
segmentPadder.SetOutputWholeExtent(0, 511, 0, 511, 0, 0)
segmentPadder.SetConstant(0)

segmentPlane = vtk.vtkPlaneSource()

segmentTransform = vtk.vtkTransformPolyDataFilter()
if SLICE_ORDER == "si":
    segmentTransform.SetTransform(sliceorder.si)
elif SLICE_ORDER == "tis":
    segmentTransform.SetTransform(sliceorder.tis)
elif SLICE_ORDER == "ap":
    segmentTransform.SetTransform(sliceorder.ap)
elif SLICE_ORDER == "pa":
    segmentTransform.SetTransform(sliceorder.pa)
elif SLICE_ORDER == "lr":
    segmentTransform.SetTransform(sliceorder.lr)
elif SLICE_ORDER == "rl":
    segmentTransform.SetTransform(sliceorder.rl)
elif SLICE_ORDER == "hf":
    segmentTransform.SetTransform(sliceorder.hf)
elif SLICE_ORDER == "hfsi":
    segmentTransform.SetTransform(sliceorder.hfsi)
elif SLICE_ORDER == "hfis":
    segmentTransform.SetTransform(sliceorder.hfis)
elif SLICE_ORDER == "hfap":
    segmentTransform.SetTransform(sliceorder.hfap)
elif SLICE_ORDER == "hfpa":
    segmentTransform.SetTransform(sliceorder.hfpa)
elif SLICE_ORDER == "hflr":
    segmentTransform.SetTransform(sliceorder.hflr)
elif SLICE_ORDER == "hfrl":
    segmentTransform.SetTransform(sliceorder.hfrl)
else:
    s = "No such transform exists."
    raise Exception(s)
segmentTransform.SetInputConnection(segmentPlane.GetOutputPort())

segmentNormals = vtk.vtkPolyDataNormals()
segmentNormals.SetInputConnection(segmentTransform.GetOutputPort())
segmentNormals.FlipNormalsOn()

colorlut = vtk.vtkLookupTable()
colorlut.SetNumberOfColors(17)
colorlut.SetTableRange(0, 16)
colorlut.Build()

colorlut.SetTableValue(0, 0, 0, 0, 0)
colorlut.SetTableValue(1, colors.salmon + (0,))  # blood
colorlut.SetTableValue(2, colors.beige + (0,))  # brain
colorlut.SetTableValue(3, colors.orange + (0,))  # duodenum
colorlut.SetTableValue(4, colors.misty_rose + (0,))  # eye_retna
colorlut.SetTableValue(5, colors.white + (0,))  # eye_white
colorlut.SetTableValue(6, colors.tomato + (0,))  # heart
colorlut.SetTableValue(7, colors.raspberry + (0,))  # ileum
colorlut.SetTableValue(8, colors.banana + (0,))  # kidney
colorlut.SetTableValue(9, colors.peru + (0,))  # l_intestine
colorlut.SetTableValue(10, colors.pink + (0,))  # liver
colorlut.SetTableValue(11, colors.powder_blue + (0,))  # lung
colorlut.SetTableValue(12, colors.carrot + (0,))  # nerve
colorlut.SetTableValue(13, colors.wheat + (0,))  # skeleton
colorlut.SetTableValue(14, colors.violet + (0,))  # spleen
colorlut.SetTableValue(15, colors.plum + (0,))  # stomach

segmentMapper = vtk.vtkPolyDataMapper()
segmentMapper.SetInputConnection(segmentPlane.GetOutputPort())
segmentMapper.ImmediateModeRenderingOn()

segmentTexture = vtk.vtkTexture()
segmentTexture.SetInputConnection(segmentPadder.GetOutputPort())
segmentTexture.SetLookupTable(colorlut)
segmentTexture.MapColorScalarsThroughLookupTableOn()
segmentTexture.InterpolateOff()

segmentActor = vtk.vtkActor()
segmentActor.SetMapper(segmentMapper)
segmentActor.SetTexture(segmentTexture)

segmentOverlayActor = vtk.vtkActor()
segmentOverlayActor.SetMapper(segmentMapper)
segmentOverlayActor.SetTexture(segmentTexture)

segmentOverlayActor.GetProperty().SetOpacity(.5)
ren.SetBackground(0, 0, 0)
ren.SetViewport(0, .5, .5, 1)
renWin.SetSize(640, 480)
ren.AddActor(greyActor)

ren2.SetBackground(0, 0, 0)
ren2.SetViewport(.5, .5, 1, 1)
ren2.AddActor(segmentActor)

cam1 = vtk.vtkCamera()
ren.SetActiveCamera(cam1)
ren.ResetCamera()

ren3.AddActor(greyActor)
ren3.AddActor(segmentOverlayActor)
segmentOverlayActor.SetPosition(0, 0, .01)

ren3.SetBackground(0, 0, 0)
ren3.SetViewport(0, 0, 1, .5)

ren2.SetActiveCamera(ren.GetActiveCamera())
ren3.SetActiveCamera(ren.GetActiveCamera())

# source WindowLevel.tcl
# WindowLevelOn.wllut()
# A helper class to set the window level, etc
WindowLevelInterface(wllut)

renWin.Render()
iren.Initialize()


# render the image
#
# iren AddObserver UserEvent {wm deiconify .vtkInteract}

# prevent the tk window from showing up then start the event loop
# wm withdraw .

def slice(number):
    global endX, endY
    global maxX, maxY
    greyReader.SetDataVOI(0, endX, 0, endY, number, number)
    greyReader.SetDataExtent(0, endX, 0, endY, number, number)
    greyReader.GetOutput().SetOrigin(0, 0, 0)
    segmentReader.SetDataVOI(0, endX, 0, endY, number, number)
    segmentReader.SetDataExtent(0, endX, 0, endY, number, number)
    segmentReader.GetOutput().SetOrigin(0, 0, 0)


renWin.Render()


def slices():
    global START_SLICE, END_SLICE
    for i in range(START_SLICE, END_SLICE + 1):
        slice(i)
        update()


iren.Initialize()
renWin.Render()
iren.Start()
