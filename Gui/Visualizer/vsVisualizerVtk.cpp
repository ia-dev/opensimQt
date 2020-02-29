#include "vsGeometryImplementationQt.h"
#include "vsVisualizerVTK.h"

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
#include <vtkSphereSource.h>
#include <vtkLineSource.h>
#include <vtkCylinderSource.h>
#include <vtkTubeFilter.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkTexture.h>
#include <vtkTextureMapToPlane.h>
#include <vtkOpenGLSkybox.h>
#include <vtkPNGReader.h>
#include <vtkImageFlip.h>
#include <vtkOpenGLTexture.h>
#include <Modeling/vsNavigatorNode.h>

vsVisualizerVTK::vsVisualizerVTK(QWidget *parent):
    QVTKOpenGLWidget(parent)
{
    vtkNew<vtkNamedColors> namedColors;
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    this->SetRenderWindow(renderWindow);
    this->GetRenderWindow()->AddRenderer(renderer);
    setBackgroundType(BackgroundType::GroundAndSky);

    //renderingTest();
    //addBox();
    //renderVtpMesh("F:\\FL\\3\\opensim-gui\\opensim-models\\Geometry\\bofoot.vtp");
    //addGround();
    //addSkyBox();
    this->update();

    //setting the renderer for the navigator elements
    vsNavigatorNode::visualizerVTK = this;


}

void vsVisualizerVTK::renderingTest()
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

void vsVisualizerVTK::getDGColor(const SimTK::DecorativeGeometry& geo,double *color_out)
{
    if(geo.getColor()[0] >= 0){
        for (int i = 0; i < 3; ++i) {
            color_out[i] = geo.getColor()[i];
        }
    }
    else {
        for (int i = 0; i < 3; ++i) {
            color_out[i] = 1;
        }
    }
}


vtkSmartPointer<vtkActor> vsVisualizerVTK::renderGeometry(OpenSim::Geometry *geometry)
{
    QString meshFile = "F:\\FL\\3\\opensim-gui\\opensim-models\\Geometry\\"+
            QString::fromStdString(geometry->getPropertyByName("mesh_file").getValue<std::string>());
    const char *fileNameChar = meshFile.toStdString().data();
    //qDebug() << "the vtp file path" << fileNameChar;
    auto vtpFileReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    vtpFileReader->SetFileName(fileNameChar);
    vtpFileReader->Update();

    auto vtpMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtpMapper->SetInputConnection(vtpFileReader->GetOutputPort());

    auto vtpActor = vtkSmartPointer<vtkActor>::New();
    vtpActor->SetMapper(vtpMapper);

    auto geometryScale = geometry->get_scale_factors();
    double geometryScaleDouble[] = {geometryScale.get(0),geometryScale.get(1),geometryScale.get(2)};
    vtpActor->SetScale(geometryScaleDouble);

    //geometry->getFrame().generateDecorations()


    vtkRenderer *renderer = this->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(vtpActor);
    renderer->ResetCamera(vtpActor->GetBounds());
    this->update();
    return  vtpActor;
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::addBox()
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

vtkSmartPointer<vtkActor> vsVisualizerVTK::addGround()
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

vtkSmartPointer<vtkActor> vsVisualizerVTK::addSkyBox()
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

vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativeMeshFile(const SimTK::DecorativeMeshFile &mesh
                                                                    ,SimTK::Transform mesh_transform ,double *scaleFactors)
{
    const char *fileNameChar = mesh.getMeshFile().data();
    qDebug() << "the vtp file path" << fileNameChar;
    auto vtpFileReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    vtpFileReader->SetFileName(fileNameChar);
    vtpFileReader->Update();

    auto vtpMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtpMapper->SetInputConnection(vtpFileReader->GetOutputPort());

    auto vtpActor = vtkSmartPointer<vtkActor>::New();
    vtpActor->SetMapper(vtpMapper);

//    auto geometryScale = mesh_transform.p();
//    double geometryScaleDouble[] = {geometryScale.get(0),geometryScale.get(1),geometryScale.get(2)};
//    auto geometryPosition = mesh_transform.p();
//    double geometryPositionDouble[] = {geometryPosition.get(0),geometryPosition.get(1),geometryPosition.get(2)};
//    vtpActor->SetPosition(geometryPositionDouble);
    vtpActor->SetScale(scaleFactors);

    vtpActor->SetUserMatrix(openSimToVtkTransform(mesh_transform));
    //geometry->getFrame().generateDecorations()


    vtkRenderer *renderer = this->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(vtpActor);
    renderer->ResetCamera(vtpActor->GetBounds());
    this->update();
    return  vtpActor;
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativeSphere(const SimTK::DecorativeSphere &sphere, SimTK::Transform sphereTransform, double *scaleFactors)
{
    auto sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(sphere.getRadius());
    sphereSource->Update();

    auto sphereMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

    auto sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);
    double colorTable[3];
    getDGColor(sphere,colorTable);
    sphereActor->GetProperty()->SetColor(colorTable);
    sphereActor->GetProperty()->SetOpacity(sphere.getOpacity()<0?1:sphere.getOpacity());

    sphereActor->SetScale(scaleFactors);
    sphereActor->SetUserMatrix(openSimToVtkTransform(sphereTransform));
    vtkRenderer *renderer = this->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(sphereActor);
    return sphereActor;
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativeLine(const SimTK::DecorativeLine &line, SimTK::Transform lineTransform, double *scaleFactors)
{
    auto lineSource = vtkSmartPointer<vtkLineSource>::New();
    lineSource->SetPoint1(line.getPoint1().get(0),line.getPoint1().get(1),line.getPoint1().get(2));
    lineSource->SetPoint2(line.getPoint2().get(0),line.getPoint2().get(1),line.getPoint2().get(2));
    lineSource->Update();

//    auto lineSource = vtkSmartPointer<vtkCylinderSource>::New();
//    lineSource->SetCenter(line.getPoint1().get(0),line.getPoint1().get(1),line.getPoint1().get(2));
//    //calculating hight
//    lineSource->SetRadius(line.getLineThickness());
//    auto pointDiff = line.getPoint2() - line.getPoint1();

    lineSource->Update();

    auto lineMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    lineMapper->SetInputConnection(lineSource->GetOutputPort());

    auto lineActor = vtkSmartPointer<vtkActor>::New();
    lineActor->SetMapper(lineMapper);
    double colorTable[3];
    getDGColor(line,colorTable);
    lineActor->GetProperty()->SetColor(colorTable);
    lineActor->GetProperty()->SetOpacity(line.getOpacity()<0?1:line.getOpacity());
    //lineActor->GetProperty()->SetLineWidth(line.getLineThickness()<0?1:line.getLineThickness());
    lineActor->SetScale(scaleFactors);
    lineActor->SetUserMatrix(openSimToVtkTransform(lineTransform));

    vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter->SetInputConnection(lineSource->GetOutputPort());
    tubeFilter->SetRadius(0.006);
    tubeFilter->SetVaryRadiusToVaryRadiusOff();
    tubeFilter->SetNumberOfSides(60);
    tubeFilter->Update();

    qDebug() << "the tickness of the line" << line.getLineThickness();

    vtkSmartPointer<vtkPolyDataMapper> tubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    tubeMapper->SetInputConnection(tubeFilter->GetOutputPort());

    vtkSmartPointer<vtkActor> tubeActor = vtkSmartPointer<vtkActor>::New();
    tubeActor->SetMapper(tubeMapper);
    tubeActor->GetProperty()->SetColor(colorTable);
    tubeActor->GetProperty()->SetOpacity(line.getOpacity()<0?1:line.getOpacity());
    tubeActor->SetScale(scaleFactors);

    vtkRenderer *renderer = this->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(lineActor);
    renderer->AddActor(tubeActor);
    return lineActor;
}

vtkSmartPointer<vtkMatrix4x4> vsVisualizerVTK::openSimToVtkTransform(SimTK::Transform stkTransform)
{
    vtkSmartPointer<vtkMatrix4x4> retMat = vtkSmartPointer<vtkMatrix4x4>::New();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            retMat->SetElement(i,j,stkTransform.toMat44()[i][j]);
        }
    }
    return  retMat;
}

void vsVisualizerVTK::addOpenSimModel(OpenSim::Model *model)
{
    //visualizer solution
    //loading the Components
    model->getSystem().realize(model->updWorkingState(),SimTK::Stage::Position);
    SimTK::Array_<SimTK::DecorativeGeometry> compDecorations;


//    model->updDisplayHints().set_show_frames(true);
//    model->updDisplayHints().set_show_path_geometry(true);
//    model->updDisplayHints().set_show_contact_geometry(true);
//    model->updDisplayHints().set_show_wrap_geometry(true);
//    model->updDisplayHints().set_show_markers(true);
//    model->updDisplayHints().set_show_path_points(true);
//    model->updDisplayHints().set_show_debug_geometry(true);
    const OpenSim::ModelDisplayHints displayHints = model->getDisplayHints();

    vsGeometryImplementationQt geoImp(this,model->getSystem().getMatterSubsystem(),model->getWorkingState());

    OpenSim::ComponentList<const OpenSim::Component> componentList = model->getComponentList();
    OpenSim::ComponentListIterator<const OpenSim::Component> itr = componentList.begin();
    while (!itr.equals(componentList.end())) {
        const OpenSim::Component *comp = &itr.deref();
        SimTK::Array_<SimTK::DecorativeGeometry> compDecorations;
        comp->generateDecorations(false,displayHints,model->getWorkingState(),compDecorations);
        comp->generateDecorations(true,displayHints,model->getWorkingState(),compDecorations);
        qDebug() << "the model subcomponents List Count >"<< compDecorations.size()<< " " << QString::fromStdString(itr.deref().getName())
                 << QString::fromStdString(itr.deref().getConcreteClassName());
        for (int i = 0; i < compDecorations.size(); ++i) {
            compDecorations.at(i).implementGeometry(geoImp);
        }

        itr.next();
    }


//    for (SimTK::Stage stage = SimTK::Stage::Empty; stage <= model->getWorkingState().getSystemStage(); stage++) {
//        model->getSystem().calcDecorativeGeometryAndAppend(model->getWorkingState(),stage,compDecorations);
//    }
//    qDebug() << "the size of the geometry array > " << compDecorations.size();
//    for (unsigned i = 0; i < compDecorations.size(); ++i) {
//        compDecorations.at(i).implementGeometry(geoImp);
//    }


}

BackgroundType vsVisualizerVTK::backgroundType() const
{
    return m_backgroundType;
}

void vsVisualizerVTK::setBackgroundType(const BackgroundType &backgroundType)
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
