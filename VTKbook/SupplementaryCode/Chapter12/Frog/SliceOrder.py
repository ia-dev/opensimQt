#!/usr/bin/env python

import vtk

'''
These transformations permute medical image data to maintain proper orientation
regardless of the acquisition order. After applying these transforms with
vtkTransformFilter, a view up of 0,-1,0 will result in the body part
facing the viewer.
NOTE: some transformations have a -1 scale factor for one of the components.
      To ensure proper polygon orientation and normal direction, you must
      apply the vtkPolyDataNormals filter.

Naming:
si - superior to inferior (top to bottom)
is - inferior to superior (bottom to top)
ap - anterior to posterior (front to back)
pa - posterior to anterior (back to front)
lr - left to right
rl - right to left
'''

transforms = dict()

msi = vtk.vtkMatrix4x4()
msi.Zero()
msi.SetElement(0, 0, 1)
msi.SetElement(1, 2, 1)
msi.SetElement(2, 1, -1)
msi.SetElement(3, 3, 1)
si = vtk.vtkTransform()
si.SetMatrix(msi)
transforms['si'] = si

mis = vtk.vtkMatrix4x4()
mis.Zero()
mis.SetElement(0, 0, 1)
mis.SetElement(1, 2, -1)
mis.SetElement(2, 1, -1)
mis.SetElement(3, 3, 1)
tis = vtk.vtkTransform()  # is is a python reserved name
tis.SetMatrix(mis)
transforms['is'] = tis

ap = vtk.vtkTransform()
ap.Scale(1, -1, 1)
transforms['ap'] = ap

pa = vtk.vtkTransform()
pa.Scale(1, -1, -1)
transforms['pa'] = pa

mlr = vtk.vtkMatrix4x4()
mlr.Zero()
mlr.SetElement(0, 2, -1)
mlr.SetElement(1, 1, -1)
mlr.SetElement(2, 0, 1)
mlr.SetElement(3, 3, 1)
lr = vtk.vtkTransform()
lr.SetMatrix(mlr)
transforms['lr'] = lr

mrl = vtk.vtkMatrix4x4()
mrl.Zero()
mrl.SetElement(0, 2, 1)
mrl.SetElement(1, 1, -1)
mrl.SetElement(2, 0, 1)
mrl.SetElement(3, 3, 1)
rl = vtk.vtkTransform()
rl.SetMatrix(mrl)
transforms['rl'] = rl

"""
The previous transforms assume radiological views of the slices (viewed from the feet). Other
modalities such as physical sectioning may view from the head. These transforms modify the original
with a 180 rotation about y.
"""

mhf = vtk.vtkMatrix4x4()
mhf.Zero()
mhf.SetElement(0, 0, -1)
mhf.SetElement(1, 1, 1)
mhf.SetElement(2, 2, -1)
mhf.SetElement(3, 3, 1)
hf = vtk.vtkTransform()
hf.SetMatrix(mhf)
transforms['hf'] = hf

hfsi = vtk.vtkTransform()
hfsi.Concatenate(hf.GetMatrix())
hfsi.Concatenate(si.GetMatrix())
transforms['hfsi'] = hfsi

hfis = vtk.vtkTransform()
hfis.Concatenate(hf.GetMatrix())
hfis.Concatenate(tis.GetMatrix())
transforms['hfis'] = hfis

hfap = vtk.vtkTransform()
hfap.Concatenate(hf.GetMatrix())
hfap.Concatenate(ap.GetMatrix())
transforms['hfap'] = hfap

hfpa = vtk.vtkTransform()
hfpa.Concatenate(hf.GetMatrix())
hfpa.Concatenate(pa.GetMatrix())
transforms['hfpa'] = hfpa

hflr = vtk.vtkTransform()
hflr.Concatenate(hf.GetMatrix())
hflr.Concatenate(lr.GetMatrix())
transforms['hflr'] = hflr

hfrl = vtk.vtkTransform()
hfrl.Concatenate(hf.GetMatrix())
hfrl.Concatenate(rl.GetMatrix())
transforms['hfrl'] = hfrl

# mat = transforms['hfrl'].GetMatrix()
# m = [0] * 16
# mat.DeepCopy(m, mat)
# m1 = [m[i:i+4] for i in range(0, len(m), 4)]
# print(m1)
