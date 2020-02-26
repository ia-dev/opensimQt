#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkConeSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkWeakPointer.h>

#include <iostream>

// Callback for the interaction
class vtkMyCallback : public vtkCallbackCommand
{
public:
  static vtkMyCallback* New()
  {
    return new vtkMyCallback;
  }
  void Execute(vtkObject* caller, unsigned long, void*) override
  {
    vtkWeakPointer<vtkRenderer> ren = reinterpret_cast<vtkRenderer*>(caller);
    std::cout << ren->GetActiveCamera()->GetPosition()[0] << ", "
              << ren->GetActiveCamera()->GetPosition()[1] << ", "
              << ren->GetActiveCamera()->GetPosition()[2] << std::endl;
  }
  ~vtkMyCallback() = default;
};

int main(int, char*[])
{
  vtkNew<vtkNamedColors> colors;

  vtkNew<vtkConeSource> cone;
  cone->SetHeight(3.0);
  cone->SetRadius(1.0);
  cone->SetResolution(10);

  vtkNew<vtkPolyDataMapper> coneMapper;
  coneMapper->SetInputConnection(cone->GetOutputPort());
  vtkNew<vtkActor> coneActor;
  coneActor->SetMapper(coneMapper);
  coneActor->GetProperty()->SetColor(colors->GetColor3d("Banana").GetData());

  vtkNew<vtkRenderer> ren1;
  ren1->AddActor(coneActor);
  ren1->SetBackground(colors->GetColor3d("MidnightBlue").GetData());

  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer(ren1);
  renWin->SetSize(300, 300);

  vtkNew<vtkMyCallback> mo1;
  ren1->AddObserver(vtkCommand::StartEvent, mo1);

  // Loop over 360 degrees and render the cone each time.
  for (auto i = 0; i < 360; ++i)
  {
    renWin->Render();
    ren1->GetActiveCamera()->Azimuth(1);
  }

  return EXIT_SUCCESS;
}
