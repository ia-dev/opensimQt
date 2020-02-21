#include "visualizervtk.h"

#include <QDebug>

#include <vtkNew.h>
#include <vtkSphereSource.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkCubeSource.h>
#include <vtkPlaneSource.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkTexture.h>
#include <vtkTextureMapToPlane.h>
#include <vtkOpenGLSkybox.h>
#include <vtkPNGReader.h>
#include <vtkImageFlip.h>
#include <vtkOpenGLTexture.h>

VisualizerVTK::VisualizerVTK(QWidget *parent):
    QVTKOpenGLWidget(parent)
{
    vtkNew<vtkNamedColors> namedColors;
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    this->SetRenderWindow(renderWindow);
    this->GetRenderWindow()->AddRenderer(renderer);
    setBackgroundType(BackgroundType::GroundAndSky);

    //renderingTest();
    addBox();
    renderVtpMesh("F:\\FL\\3\\opensim-gui\\opensim-models\\Geometry\\bofoot.vtp");
    //addGround();
    //addSkyBox();
    this->update();
}

void VisualizerVTK::renderingTest()
{
    //this fuction will draw a sphere on the vtkWidget for testing

    vtkNew<vtkNamedColors> namedColors;
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
    vtkRenderer *renderer = this->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(sphereActor);

}

void VisualizerVTK::renderVtpMesh(QString fileName)
{
    const char *fileNameChar = fileName.toStdString().data();
    qDebug() << "the vtp file path" << fileNameChar;
    auto vtpFileReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    vtpFileReader->SetFileName(fileNameChar);
    vtpFileReader->Update();

    auto vtpMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtpMapper->SetInputConnection(vtpFileReader->GetOutputPort());

    auto vtpActor = vtkSmartPointer<vtkActor>::New();
    vtpActor->SetMapper(vtpMapper);

    vtkRenderer *renderer = this->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(vtpActor);
    renderer->ResetCamera(vtpActor->GetBounds());
    this->update();
}

vtkSmartPointer<vtkActor> VisualizerVTK::addBox()
{
    auto boxSource = vtkSmartPointer<vtkCubeSource>::New();
    boxSource->Update();

    auto boxMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    boxMapper->SetInputConnection(boxSource->GetOutputPort());

    auto boxActor = vtkSmartPointer<vtkActor>::New();
    boxActor->SetMapper(boxMapper);
    boxActor->GetProperty()->SetColor(0,1,0);
    boxActor->SetPosition(4,0,0);
    vtkRenderer *renderer = this->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(boxActor);
    return boxActor;
}

vtkSmartPointer<vtkActor> VisualizerVTK::addGround()
{
    auto planeSource = vtkSmartPointer<vtkPlaneSource>::New();
    planeSource->Update();
    planeSource->SetCenter(0,0,0);
    planeSource->SetNormal(0,0,1);
    planeSource->SetPoint1(0,10000,0);
    planeSource->SetPoint2(10000,0,0);

    auto planeMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    planeMapper->SetInputConnection(planeSource->GetOutputPort());

    auto planeActor = vtkSmartPointer<vtkActor>::New();
    planeActor->SetMapper(planeMapper);
    planeActor->GetProperty()->SetColor(0,1,1);

    vtkRenderer *renderer = this->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(planeActor);
    return planeActor;
}

vtkSmartPointer<vtkActor> VisualizerVTK::addSkyBox()
{
    const char *texturesPaths[] ={
        "./skyboxFRONT.png",
        "./skyboxBACK.png",
        "./skyboxRIGHT.png",
        "./skyboxLEFT.png",
        "./skyboxUP.png",
        "./skyboxDOWN.png"
    };


    auto skyTexture  = vtkSmartPointer<vtkTexture>::New();
    //skyTexture->Update();
    //skyTexture->MipmapOn();
    skyTexture->InterpolateOn();
    skyTexture->RepeatOff();
    skyTexture->EdgeClampOn();
    //skyTexture->CubeMapOn();
    for (int i = 0; i < 6; ++i) {
        auto imgReader = vtkSmartPointer<vtkPNGReader>::New();
        imgReader->SetFileName(texturesPaths[i]);
        auto flip = vtkSmartPointer<vtkImageFlip>::New();
        flip->SetInputConnection(imgReader->GetOutputPort());
        flip->SetFilteredAxes(1);
        skyTexture->SetInputConnection(i,flip->GetOutputPort());
    }



    auto skySource = vtkSmartPointer<vtkCubeSource>::New();
    skySource->Update();

    auto skyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    skyMapper->SetInputConnection(skySource->GetOutputPort());

//    auto skyBox  = vtkSmartPointer<vtkOpenGLSkybox>::New();
//    skyBox->SetTexture(skyTexture);
//    skyBox->SetMapper(skyMapper);

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(skyMapper);
    actor->SetTexture(skyTexture);

    vtkRenderer *renderer = this->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(actor);
    return actor;
}

BackgroundType VisualizerVTK::backgroundType() const
{
    return m_backgroundType;
}

void VisualizerVTK::setBackgroundType(const BackgroundType &backgroundType)
{
    vtkNew<vtkNamedColors> namedColors;
    auto renderer =this->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    m_backgroundType = backgroundType;
    if (m_backgroundType == BackgroundType::Solid) {
        renderer->SetGradientBackground(false);
        renderer->SetBackground(namedColors->GetColor4d("SteelBlue").GetData());
    }
    else {
        renderer->SetGradientBackground(true);
        renderer->SetBackground2(0.43,0.62,0.65);
        renderer->SetBackground(0.15,0.18,0.25);
    }
}
