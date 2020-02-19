#include "visualizervtk.h"

#include <vtkNew.h>
#include <vtkSphereSource.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkGenericOpenGLRenderWindow.h>

VisualizerVTK::VisualizerVTK(QWidget *parent):
    QVTKOpenGLWidget(parent)
{
    vtkNew<vtkNamedColors> namedColors;

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;

    auto sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetCenter(0,0,0);
    sphereSource->SetRadius(3);
    sphereSource->Update();
    auto sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    //sphereMapper->SetInputConnection(0,sphereMapper->GetOutputPort(0));
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

    auto sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);
    sphereActor->GetProperty()->SetColor(namedColors->GetColor4d("Tomato").GetData());

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(sphereActor);
    renderer->SetBackground(namedColors->GetColor4d("SteelBlue").GetData());
    this->SetRenderWindow(renderWindow);
    this->GetRenderWindow()->AddRenderer(renderer);
    this->update();
}
