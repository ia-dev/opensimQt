#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkConeSource.h>
#include <vtkDepthOfFieldPass.h>
#include <vtkGlyph3D.h>
#include <vtkImageData.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOpenGLRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRectilinearGrid.h>
#include <vtkRectilinearGridGeometryFilter.h>
#include <vtkRenderStepsPass.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSimpleMotionBlurPass.h>
#include <vtkSphereSource.h>

#include <iostream>

#include <vtkBMPWriter.h>
#include <vtkImageWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkPNGWriter.h>
#include <vtkPNMWriter.h>
#include <vtkPostScriptWriter.h>
#include <vtkTIFFWriter.h>
#include <vtkWindowToImageFilter.h>

#include <algorithm>
#include <locale>
#include <string>

namespace {
/**
 * Write the render window view to an image file.
 *
 * Image types supported are:
 *  BMP, JPEG, PNM, PNG, PostScript, TIFF.
 * The default parameters are used for all writers, change as needed.
 *
 * @param fileName The file name, if no extension then PNG is assumed.
 * @param renWin The render window.
 * @param rgba Used to set the buffer type.
 *
 */
void WriteImage(std::string const& fileName, vtkRenderWindow* renWin,
                bool rgba = true);

} // namespace
int main(int, char* [])
{
  vtkNew<vtkNamedColors> colors;

  // create the rendering objects
  vtkNew<vtkRenderer> ren1;
  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer(ren1);
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);

  // create the pipline, ball and spikes
  vtkNew<vtkSphereSource> sphere;
  sphere->SetThetaResolution(7);
  sphere->SetPhiResolution(7);
  vtkNew<vtkPolyDataMapper> sphereMapper;
  sphereMapper->SetInputConnection(sphere->GetOutputPort());
  vtkNew<vtkActor> sphereActor;
  sphereActor->SetMapper(sphereMapper);
  vtkNew<vtkActor> sphereActor2;
  sphereActor2->SetMapper(sphereMapper);
  vtkNew<vtkConeSource> cone;
  cone->SetResolution(5);

  vtkNew<vtkGlyph3D> glyph;
  glyph->SetInputConnection(sphere->GetOutputPort());
  glyph->SetSourceConnection(cone->GetOutputPort());
  glyph->SetVectorModeToUseNormal();
  glyph->SetScaleModeToScaleByVector();
  glyph->SetScaleFactor(0.25);
  vtkNew<vtkPolyDataMapper> spikeMapper;
  spikeMapper->SetInputConnection(glyph->GetOutputPort());
  vtkNew<vtkActor> spikeActor;
  spikeActor->SetMapper(spikeMapper);
  vtkNew<vtkActor> spikeActor2;
  spikeActor2->SetMapper(spikeMapper);

  spikeActor->SetPosition(0, 0.7, 7.0);
  sphereActor->SetPosition(0, 0.7, 7.0);
  spikeActor2->SetPosition(0, -1, -10);
  sphereActor2->SetPosition(0, -1, -10);
  //   spikeActor2->SetScale(1.5, 1.5, 1.5);
  //   sphereActor2->SetScale(1.5, 1.5, 1.5);

  ren1->AddActor(sphereActor);
  ren1->AddActor(spikeActor);
  ren1->AddActor(sphereActor2);
  ren1->AddActor(spikeActor2);
  ren1->SetBackground(colors->GetColor3d("MidnightBlue").GetData());
  renWin->SetSize(1002, 1002);

  // Zoom in a little
  ren1->ResetCamera();
  ren1->GetActiveCamera()->SetFocalPoint(0, 0, 0);
  ren1->GetActiveCamera()->Zoom(3.8);
  ren1->GetActiveCamera()->SetFocalDisk(10);

  // create the basic VTK render steps
  vtkNew<vtkRenderStepsPass> basicPasses;

  // finally add the DOF passs
  vtkNew<vtkDepthOfFieldPass> dofp;
  dofp->SetDelegatePass(basicPasses);
  dofp->AutomaticFocalDistanceOff();
  // tell the renderer to use our render pass pipeline
  vtkOpenGLRenderer* glrenderer = vtkOpenGLRenderer::SafeDownCast(ren1);
  glrenderer->SetPass(dofp);

  vtkNew<vtkSimpleMotionBlurPass> motion;
  motion->SetDelegatePass(basicPasses);

  renWin->Render();

  auto numRenders = 21;
  for (auto i = 0; i < numRenders; i += 1)
  {
    //     spikeActor2->RotateY(2);
    //     sphereActor2->RotateY(2);
    ren1->GetActiveCamera()->Azimuth(2);
    ren1->GetActiveCamera()->Elevation(2);
    renWin->Render();
  }
  iren->Initialize();
  iren->Start();

  WriteImage("Figure7-37", renWin, false);

  return EXIT_SUCCESS;
}

namespace {
void WriteImage(std::string const& fileName, vtkRenderWindow* renWin, bool rgba)
{
  if (!fileName.empty())
  {
    std::string fn = fileName;
    std::string path;
    std::string ext;
    std::size_t found = fn.find_last_of(".");
    if (found == std::string::npos)
    {
      path = fn;
      ext = ".png";
      fn += ext;
    }
    else
    {
      path = fileName.substr(0, found);
      ext = fileName.substr(found, fileName.size());
    }
    std::locale loc;
    std::transform(ext.begin(), ext.end(), ext.begin(),
                   [=](char const& c) { return std::tolower(c, loc); });
    vtkSmartPointer<vtkImageWriter> writer =
        vtkSmartPointer<vtkImageWriter>::New();
    if (ext == ".bmp")
    {
      writer = vtkSmartPointer<vtkBMPWriter>::New();
    }
    else if (ext == ".jpg")
    {
      writer = vtkSmartPointer<vtkJPEGWriter>::New();
    }
    else if (ext == ".pnm")
    {
      writer = vtkSmartPointer<vtkPNMWriter>::New();
    }
    else if (ext == ".ps")
    {
      if (rgba)
      {
        rgba = false;
      }
      writer = vtkSmartPointer<vtkPostScriptWriter>::New();
    }
    else if (ext == ".tiff")
    {
      writer = vtkSmartPointer<vtkTIFFWriter>::New();
    }
    else
    {
      writer = vtkSmartPointer<vtkPNGWriter>::New();
    }
    vtkSmartPointer<vtkWindowToImageFilter> window_to_image_filter =
        vtkSmartPointer<vtkWindowToImageFilter>::New();
    window_to_image_filter->SetInput(renWin);
    window_to_image_filter->SetScale(1); // image quality
    if (rgba)
    {
      window_to_image_filter->SetInputBufferTypeToRGBA();
    }
    else
    {
      window_to_image_filter->SetInputBufferTypeToRGB();
    }
    // Read from the front buffer.
    window_to_image_filter->ReadFrontBufferOff();
    window_to_image_filter->Update();

    writer->SetFileName(fn.c_str());
    writer->SetInputConnection(window_to_image_filter->GetOutputPort());
    writer->Write();
  }
  else
  {
    std::cerr << "No filename provided." << std::endl;
  }

  return;
}
} // namespace
