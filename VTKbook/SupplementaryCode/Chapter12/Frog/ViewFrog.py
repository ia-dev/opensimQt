import vtk

colors = vtk.vtkNamedColors()

ren1 = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren1)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

# The tissue name and color.
tissues = [
    ['lung', 'powder_blue'], ['heart', 'tomato'],
    ['liver', 'pink]'], ['duodenum', 'orange'],
    ['blood', 'salmon'], ['brain', 'beige'],
    ['eye_retna', 'misty_rose'], ['eye_white', 'white'],
    ['ileum', 'raspberry'], ['kidney', 'banana'],
    ['l_intestine', 'peru'], ['nerve', 'carrot'],
    ['spleen', 'violet'], ['stomach', 'plum'],
    ['skeleton', 'wheat']
]
reader = dict()
mapper = dict()
actor = dict()

for t in tissues:
    reader[t[0]] = vtk.vtkPolyDataReader()
    reader[t[0]].SetFileName(t[0] + '.vtk')
    mapper[t[0]] = vtk.vtkPolyDataMapper()
    mapper[t[0]].SetInputConnection(reader[t[0]].GetOutputPort())
    mapper[t[0]].ScalarVisibilityOff()
    actor[t[0]] = vtk.vtkLODActor()
    actor[t[0]].SetMapper(mapper[t[0]])
    actor[t[0]].GetProperty().SetDiffuseColor(colors.GetColor3d(t[1]))
    actor[t[0]].GetProperty().SetSpecular(0.5)
    actor[t[0]].GetProperty().SetDiffuse(0.8)
    ren1.AddActor(actor[t[0]])

ren1.SetBackground(colors.GetColor3d("SlateGray"))
renWin.SetSize(450, 450)
ren1.GetActiveCamera().SetViewUp(0, -1, 0)
ren1.GetActiveCamera().Azimuth(30)
ren1.GetActiveCamera().Elevation(30)
ren1.GetActiveCamera().Dolly(1.75)
iren.Initialize()
renWin.Render()
iren.Start()
