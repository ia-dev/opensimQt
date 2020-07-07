#include "vsGeometryImplementationQt.h"
#include "vsInteractorStyle.h"
#include "vsVisualizerVTK.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QTime>

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
#include <vtkTextSource.h>
#include <vtkLineSource.h>
#include <vtkPointSource.h>
#include <vtkCylinderSource.h>
#include <vtkConeSource.h>
#include <vtkRegularPolygonSource.h>
#include <vtkParametricEllipsoid.h>
#include <vtkArrowSource.h>
#include <vtkParametricTorus.h>
#include <vtkParametricFunctionSource.h>
#include <vtkTubeFilter.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkTexture.h>
#include <vtkOpenGLSkybox.h>
#include <vtkPNGReader.h>
#include <vtkImageFlip.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOpenGLTexture.h>
#include<vtkTexturedButtonRepresentation2D.h>
#include <vtkButtonWidget.h>
#include <vtkImageData.h>
#include <vtkCoordinate.h>
#include <vtkWidgetEvent.h>
#include <vtkCamera.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkTransform.h>
#include <vtkAxesActor.h>
#include <vsModeling/vsNavigatorNode.h>
#include <vtkTextureMapToPlane.h>
#include <vsTools/vsOpenSimTools.h>
#include <vtkPropPicker.h>
#include <vtkOBJReader.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataReader.h>

vsVisualizerVTK::vsVisualizerVTK(QWidget *parent):
    QVTKOpenGLNativeWidget(parent),currentModel(nullptr)
{

    connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();

    vtkNew<vtkNamedColors> namedColors;
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    this->setRenderWindow(renderWindow);
    this->renderWindow()->AddRenderer(renderer);
    setBackgroundType(BackgroundType::GroundAndSky);

    //renderingTest();
    //addBox();
    //renderVtpMesh("F:\\FL\\3\\opensim-gui\\opensim-models\\Geometry\\bofoot.vtp");
    ground  = addGround();

    //skyBox = addSkyBox();
    //this->update();
    globalFrame = addGlobalFrame();
    renderer->GetActiveCamera()->SetPosition(2,2,2);
    renderer->GetActiveCamera()->SetFocalPoint(0,0,0);
    //renderer->GetActiveCamera()->SetClippingRange(1000,2000);
    //renderer->GetActiveCamera()->SetParallelProjection(true);
    //renderer->GetActiveCamera()->GetClippingRange(ground->GetBounds());
    //renderer->LightFollowCameraOn();

    //setting the renderer for the navigator elements
    vsNavigatorNode::visualizerVTK = this;

    //Setting Up the TOP Left Buttons
    //createButton(0,0,"");

    //interaction to select actors in the sceen
    propPicker = vtkSmartPointer<vtkPropPicker>::New();
    //vtkSmartPointer<vsInteractorStyle> interactor = vtkSmartPointer<vsInteractorStyle>::New();
    //interactor->setVisualizer(this);
    connections->Connect(interactor(),vtkCommand::LeftButtonPressEvent,this,SLOT(onVtkDoubleClicked(vtkObject *)));
    connections->Connect(interactor(),vtkCommand::StartPickEvent,this,SLOT(onInteractorPick(vtkObject *)));

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
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
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


    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
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
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(boxActor);
    return boxActor;
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::addGround()
{

    //TODO make the plane invisible from the -y
    auto planeSource = vtkSmartPointer<vtkPlaneSource>::New();
    planeSource->Update();
    planeSource->SetNormal(0,1,0);
    planeSource->SetOrigin(-500,0,-500);
    planeSource->SetPoint1(500,0,-500);
    planeSource->SetPoint2(-500,0,500);
    //planeSource->SetResolution(1000,1000);
    auto imgReader = vtkSmartPointer<vtkPNGReader>::New();
    imgReader->SetFileName("./vtk_images/Floor05-rsr-a.png");
    imgReader->Update();

    vtkSmartPointer<vtkImageData> groundData = vtkSmartPointer<vtkImageData>::New();
    createGroundImage(groundData,1000,1000);
    vtkSmartPointer<vtkTexture> texture =
      vtkSmartPointer<vtkTexture>::New();
    //texture->SetInputData(groundData);
    texture->SetInputData(imgReader->GetOutput());
    texture->SetRepeat(true);
    //texture->SetQuality(32);
    //texture->InterpolateOn();
    texture->Update();

    vtkSmartPointer<vtkTextureMapToPlane> texturePlane = vtkSmartPointer<vtkTextureMapToPlane>::New();
    texturePlane->SetInputConnection(planeSource->GetOutputPort());
    //texturePlane->SetAutomaticPlaneGeneration(true);
    texturePlane->SetSRange(0,1200);
    texturePlane->SetTRange(0,1200);
    auto planeMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    planeMapper->SetInputConnection(texturePlane->GetOutputPort());

    auto planeActor = vtkSmartPointer<vtkActor>::New();
    planeActor->SetMapper(planeMapper);
    planeActor->GetProperty()->SetColor(1,1,1);
    planeActor->SetTexture(texture);
    //planeActor->SetUseBounds(false);
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(planeActor);

    return planeActor;
}

void vsVisualizerVTK::createGroundImage(vtkSmartPointer<vtkImageData> groundData, int w, int h)
{
    groundData->SetDimensions(w, h, 1);
    groundData->AllocateScalars(VTK_UNSIGNED_CHAR, 3);

    int* dims = groundData->GetDimensions();

    // Fill the image with
    for (int y = 0; y < dims[1]; y++)
    {
      for (int x = 0; x < dims[0]; x++)
      {
        unsigned char* pixel =
          static_cast<unsigned char*>(groundData->GetScalarPointer(x, y, 0));
        if ((x+y)%2 == 0 )
        {
          pixel[0] = 150;
          pixel[1] = 150;
          pixel[2] = 150;
        }
        else
        {
          pixel[0] = 255;
          pixel[1] = 255;
          pixel[2] = 255;
        }
      }
    }
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::addSkyBox()
{
    const char *texturesPaths[] ={
        "./vtk_images/skyboxFRONT.png",
        "./vtk_images/skyboxBACK.png",
        "./vtk_images/skyboxRIGHT.png",
        "./vtk_images/skyboxLEFT.png",
        "./vtk_images/skyboxUP.png",
        "./vtk_images/skyboxDOWN.png"
    };


    auto skyTexture  = vtkSmartPointer<vtkTexture>::New();
    //skyTexture->Update();
    skyTexture->CubeMapOn();
    //skyTexture->MipmapOn();
    skyTexture->InterpolateOn();
    skyTexture->RepeatOff();
    skyTexture->EdgeClampOn();
    skyTexture->Update();
    for (int i = 0; i < 6; ++i) {
        auto imgReader = vtkSmartPointer<vtkPNGReader>::New();
        imgReader->SetFileName(texturesPaths[i]);
        imgReader->Update();
        auto flip = vtkSmartPointer<vtkImageFlip>::New();
        flip->SetInputConnection(imgReader->GetOutputPort(0));
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

    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(actor);
    return actor;
}

vtkSmartPointer<vtkAxesActor> vsVisualizerVTK::addGlobalFrame()
{
    auto renderer = renderWindow()->GetRenderers()->GetFirstRenderer();

    vtkSmartPointer<vtkTransform> transform =
    vtkSmartPointer<vtkTransform>::New();

    vtkSmartPointer<vtkAxesActor> axes =
    vtkSmartPointer<vtkAxesActor>::New();

    // The axes are positioned with a user transform
    axes->SetUserTransform(transform);
    axes->AxisLabelsOff();
    axes->SetCylinderRadius(0.01);
    axes->SetShaftTypeToCylinder();
    axes->SetTotalLength(1,1,1);
    axes->SetConeRadius(0);
    axes->SetUseBounds(false);

    renderer->AddActor(axes);
    return axes;
}

vtkSmartPointer<vtkPolyDataMapper> vsVisualizerVTK::getMeshDataMapper(std::string fileName)
{
    //Here you can add support for mesh format

    QString fileExtention = QString::fromStdString(fileName).right(3);
    const char *fileNameChar = fileName.data();
    //qDebug() << "the "<< fileExtention << " file path" << fileNameChar;

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

    if(fileExtention.toLower() == "obj"){
        vtkSmartPointer<vtkOBJReader> objReader =
          vtkSmartPointer<vtkOBJReader>::New();
        objReader->SetFileName(fileNameChar);
        objReader->Update();
        mapper->SetInputConnection(objReader->GetOutputPort());
    }
    else if (fileExtention.toLower() == "stl"){
        vtkSmartPointer<vtkSTLReader> stlReader =
                vtkSmartPointer<vtkSTLReader>::New();
        stlReader->SetFileName(fileNameChar);
        stlReader->Update();
        mapper->SetInputConnection(stlReader->GetOutputPort());
    }
    else if (fileExtention.toLower() == "vtk"){
        vtkSmartPointer<vtkPolyDataReader> vtkReader =
                vtkSmartPointer<vtkPolyDataReader>::New();
        vtkReader->SetFileName(fileNameChar);
        vtkReader->Update();
        mapper->SetInputConnection(vtkReader->GetOutputPort());
    }
    else {
        // vtp
        auto vtpFileReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
        vtpFileReader->SetFileName(fileNameChar);
        vtpFileReader->Update();
        mapper->SetInputConnection(vtpFileReader->GetOutputPort());
    }
    return mapper;
}

void vsVisualizerVTK::updateDecorativeGeometry(OpenSim::Object *obj, int actorIndex, const SimTK::DecorativeGeometry &geo, SimTK::Transform geometryTransform, double *scaleFactors)
{
    auto probList = componentActorsMap.value(obj,nullptr);
    if(!probList){
        qDebug() << "no prob detected.";
        return;
    };
    vtkSmartPointer<vtkProp> prop = nullptr;
    vtkSmartPointer<vtkActor> actor = nullptr;
    try {
      if(probList->size()<actorIndex+1){
          qDebug() << "actor index not found";
          return;
      }
      prop = probList->at(actorIndex);
      actor = vtkActor::SafeDownCast(prop);
      updateIndecies.value(currentModel)->insert(actor,true);
    } catch (...) {
        //qDebug() << "no prob found at index> " << actorIndex << "\n" << "or the prob cant be converted to actor";
        return;
    }
    actor->SetScale(scaleFactors);
    actor->SetUserMatrix(openSimToVtkTransform(geometryTransform));


}



vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativeMeshFile(const SimTK::DecorativeMeshFile &mesh
                                                                    ,SimTK::Transform mesh_transform ,double *scaleFactors)
{

    vtkSmartPointer<vtkPolyDataMapper> meshMapper = getMeshDataMapper(mesh.getMeshFile());
    auto vtpActor = vtkSmartPointer<vtkActor>::New();
    vtpActor->SetMapper(meshMapper);

//    auto geometryScale = mesh_transform.p();
//    double geometryScaleDouble[] = {geometryScale.get(0),geometryScale.get(1),geometryScale.get(2)};
//    auto geometryPosition = mesh_transform.p();
//    double geometryPositionDouble[] = {geometryPosition.get(0),geometryPosition.get(1),geometryPosition.get(2)};
//    vtpActor->SetPosition(geometryPositionDouble);
    vtpActor->SetScale(scaleFactors);

    vtpActor->SetUserMatrix(openSimToVtkTransform(mesh_transform));
    //geometry->getFrame().generateDecorations()


    updateIndecies.value(currentModel)->insert(vtpActor,false);
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
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
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(sphereActor);
    updateIndecies.value(currentModel)->insert(sphereActor,false);
    return sphereActor;
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativeEllipsoid(const SimTK::DecorativeEllipsoid &ellipsoid, SimTK::Transform ellipsoidTransform, double *scaleFactors)
{
    auto ellipsoidParameter = vtkSmartPointer<vtkParametricEllipsoid>::New();
    ellipsoidParameter->SetXRadius(ellipsoid.getRadii()[0]);
    ellipsoidParameter->SetYRadius(ellipsoid.getRadii()[1]);
    ellipsoidParameter->SetZRadius(ellipsoid.getRadii()[2]);



    auto ellipsoidSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
    ellipsoidSource->SetParametricFunction(ellipsoidParameter);
    ellipsoidSource->Update();

    auto ellipsoidMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    ellipsoidMapper->SetInputConnection(ellipsoidSource->GetOutputPort());

    auto ellipsoidActor = vtkSmartPointer<vtkActor>::New();
    ellipsoidActor->SetMapper(ellipsoidMapper);
    double colorTable[3];
    getDGColor(ellipsoid,colorTable);
    ellipsoidActor->GetProperty()->SetColor(colorTable);
    ellipsoidActor->GetProperty()->SetOpacity(ellipsoid.getOpacity()<0?1:ellipsoid.getOpacity());

    ellipsoidActor->SetScale(scaleFactors);
    ellipsoidActor->SetUserMatrix(openSimToVtkTransform(ellipsoidTransform));
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(ellipsoidActor);
    return ellipsoidActor;
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativeLine(const SimTK::DecorativeLine &line, SimTK::Transform lineTransform, double *scaleFactors,OpenSim::Object *obj,int actorIndex)
{

    //updating tube actors;
    vtkSmartPointer<vtkActor> tubeActor = nullptr;
    double colorTable[3];
    getDGColor(line,colorTable);

    if(!updating){
        tubeActor = vtkSmartPointer<vtkActor>::New();

        auto lineSource = vtkSmartPointer<vtkLineSource>::New();
        lineSource->SetPoint1(line.getPoint1().get(0),line.getPoint1().get(1),line.getPoint1().get(2));
        lineSource->SetPoint2(line.getPoint2().get(0),line.getPoint2().get(1),line.getPoint2().get(2));
        lineSource->Update();

    //    auto lineSource = vtkSmartPointer<vtkCylinderSource>::New();
    //    lineSource->SetCenter(line.getPoint1().get(0),line.getPoint1().get(1),line.getPoint1().get(2));
    //    //calculating hight
    //    lineSource->SetRadius(line.getLineThickness());
    //    auto pointDiff = line.getPoint2() - line.getPoint1();

        //lineSource->Update();

        auto lineMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
        lineMapper->SetInputConnection(lineSource->GetOutputPort());

    //    auto lineActor = vtkSmartPointer<vtkActor>::New();
    //    lineActor->SetMapper(lineMapper);
    //    lineActor->GetProperty()->SetColor(colorTable);
    //    lineActor->GetProperty()->SetOpacity(line.getOpacity()<0?1:line.getOpacity());
    //    //lineActor->GetProperty()->SetLineWidth(line.getLineThickness()<0?1:line.getLineThickness());
    //    lineActor->SetScale(scaleFactors);
    //    lineActor->SetUserMatrix(openSimToVtkTransform(lineTransform));

        vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
        tubeFilter->SetInputConnection(lineSource->GetOutputPort());
        tubeFilter->SetRadius(0.004);
        tubeFilter->SetVaryRadiusToVaryRadiusOff();
        tubeFilter->SetNumberOfSides(20);
        tubeFilter->Update();

        //qDebug() << "the tickness of the line" << line.getLineThickness();

        vtkSmartPointer<vtkPolyDataMapper> tubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        tubeMapper->SetInputConnection(tubeFilter->GetOutputPort());
        tubeMapper->GetInput();

        tubeActor->SetMapper(tubeMapper);
        vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
        //tubeActor->getInp
        //renderer->AddActor(lineActor);
        renderer->AddActor(tubeActor);
        muscleActorLineSourceMap.insert(tubeActor,lineSource);
        updateIndecies.value(currentModel)->insert(tubeActor,false);

    }else{
        auto probList = componentActorsMap.value(obj,nullptr);
        if(!probList){
            qDebug() << "no prob list detected for line";
            return nullptr;
        };
        vtkSmartPointer<vtkProp> prop = nullptr;
        try {
          if(probList->size()<actorIndex+1){
              return nullptr;
          }
          prop = probList->at(actorIndex);
          tubeActor = vtkActor::SafeDownCast(prop);
          auto lineSource = muscleActorLineSourceMap.value(tubeActor);
          lineSource->SetPoint1(line.getPoint1().get(0),line.getPoint1().get(1),line.getPoint1().get(2));
          lineSource->SetPoint2(line.getPoint2().get(0),line.getPoint2().get(1),line.getPoint2().get(2));
          lineSource->Update();
          //lineSource->UpdateWholeExtent();

          updateIndecies.value(currentModel)->insert(tubeActor,true);

        } catch (...) {
            //qDebug() << "no prob found at index> " << actorIndex << "\n" << "or the prob cant be converted to actor";
            return nullptr;
        }
    }
    tubeActor->SetUserMatrix(openSimToVtkTransform(lineTransform));
    tubeActor->GetProperty()->SetColor(colorTable);
    tubeActor->GetProperty()->SetOpacity(line.getOpacity()<0?1:line.getOpacity());
    tubeActor->SetScale(scaleFactors);

    return tubeActor;
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativeCylender(const SimTK::DecorativeCylinder &cylender, SimTK::Transform cylanderTransform, double *scaleFactors)
{
    auto cylenderSource = vtkSmartPointer<vtkCylinderSource>::New();
    cylenderSource->SetHeight(cylender.getHalfHeight()*2);
    cylenderSource->SetRadius(cylender.getRadius());
    cylenderSource->Update();

    auto cylenderMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    cylenderMapper->SetInputConnection(cylenderSource->GetOutputPort());

    auto cylenderActor = vtkSmartPointer<vtkActor>::New();
    cylenderActor->SetMapper(cylenderMapper);
    double colorTable[3];
    getDGColor(cylender,colorTable);
    cylenderActor->GetProperty()->SetColor(colorTable);
    cylenderActor->GetProperty()->SetOpacity(cylender.getOpacity()<0?1:cylender.getOpacity());

    cylenderActor->SetScale(scaleFactors);
    cylenderActor->SetUserMatrix(openSimToVtkTransform(cylanderTransform));
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(cylenderActor);
    return cylenderActor;
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativeBrick(const SimTK::DecorativeBrick &brick, SimTK::Transform brickTransform, double *scaleFactors)
{
    auto brickSource = vtkSmartPointer<vtkCubeSource>::New();
    brickSource->SetXLength(brick.getHalfLengths()[0]);
    brickSource->SetYLength(brick.getHalfLengths()[1]);
    brickSource->SetZLength(brick.getHalfLengths()[2]);
    brickSource->Update();

    auto brickMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    brickMapper->SetInputConnection(brickSource->GetOutputPort());

    auto brickActor = vtkSmartPointer<vtkActor>::New();
    brickActor->SetMapper(brickMapper);
    double colorTable[3];
    getDGColor(brick,colorTable);
    brickActor->GetProperty()->SetColor(colorTable);
    brickActor->GetProperty()->SetOpacity(brick.getOpacity()<0?1:brick.getOpacity());

    brickActor->SetScale(scaleFactors);
    brickActor->SetUserMatrix(openSimToVtkTransform(brickTransform));
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(brickActor);
    return brickActor;
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativeCircle(const SimTK::DecorativeCircle &circle, SimTK::Transform brickTransform, double *scaleFactors)
{
    auto circleSource = vtkSmartPointer<vtkRegularPolygonSource>::New();

    circleSource->SetRadius(circle.getRadius());

    circleSource->Update();

    auto circleMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    circleMapper->SetInputConnection(circleSource->GetOutputPort());

    auto circleActor = vtkSmartPointer<vtkActor>::New();
    circleActor->SetMapper(circleMapper);
    double colorTable[3];
    getDGColor(circle,colorTable);
    circleActor->GetProperty()->SetColor(colorTable);
    circleActor->GetProperty()->SetOpacity(circle.getOpacity()<0?1:circle.getOpacity());

    circleActor->SetScale(scaleFactors);
    circleActor->SetUserMatrix(openSimToVtkTransform(brickTransform));
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(circleActor);
    return circleActor;
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativeTorus(const SimTK::DecorativeTorus &torus, SimTK::Transform torusTransform, double *scaleFactors)
{
    auto torusParameter = vtkSmartPointer<vtkParametricTorus>::New();
    torusParameter->SetRingRadius(torus.getTubeRadius());
    torusParameter->SetCrossSectionRadius(torus.getTorusRadius());



    auto torusSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
    torusSource->SetParametricFunction(torusParameter);
    torusSource->Update();

    auto torusMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    torusMapper->SetInputConnection(torusSource->GetOutputPort());

    auto torusActor = vtkSmartPointer<vtkActor>::New();
    torusActor->SetMapper(torusMapper);
    double colorTable[3];
    getDGColor(torus,colorTable);
    torusActor->GetProperty()->SetColor(colorTable);
    torusActor->GetProperty()->SetOpacity(torus.getOpacity()<0?1:torus.getOpacity());

    torusActor->SetScale(scaleFactors);
    torusActor->SetUserMatrix(openSimToVtkTransform(torusTransform));
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(torusActor);
    return torusActor;

}

vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativeText(const SimTK::DecorativeText &text, SimTK::Transform textTransform, double *scaleFactors)
{
    auto textSource = vtkSmartPointer<vtkTextSource>::New();
    textSource->SetText(text.getText().data());
    textSource->Update();

    auto textMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    textMapper->SetInputConnection(textSource->GetOutputPort());

    auto textActor = vtkSmartPointer<vtkActor>::New();
    textActor->SetMapper(textMapper);
    double colorTable[3];
    getDGColor(text,colorTable);
    textActor->GetProperty()->SetColor(colorTable);
    textActor->GetProperty()->SetOpacity(text.getOpacity()<0?1:text.getOpacity());

    textActor->SetScale(scaleFactors);
    textActor->SetUserMatrix(openSimToVtkTransform(textTransform));
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(textActor);
    return textActor;
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativeArrow(const SimTK::DecorativeArrow &arrow, SimTK::Transform arrowTransform, double *scaleFactors)
{
    auto arrowSource = vtkSmartPointer<vtkArrowSource>::New();
    arrowSource->SetTipLength(arrow.getTipLength());
    arrowSource->Update();

    auto arrowMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    arrowMapper->SetInputConnection(arrowSource->GetOutputPort());

    auto arrowActor = vtkSmartPointer<vtkActor>::New();
    arrowActor->SetMapper(arrowMapper);
    double colorTable[3];
    getDGColor(arrow,colorTable);
    arrowActor->GetProperty()->SetColor(colorTable);
    arrowActor->GetProperty()->SetOpacity(arrow.getOpacity()<0?1:arrow.getOpacity());

    arrowActor->SetScale(scaleFactors);
    arrowActor->SetUserMatrix(openSimToVtkTransform(arrowTransform));
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(arrowActor);
    return arrowActor;
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativeCone(const SimTK::DecorativeCone &cone, SimTK::Transform coneTransform, double *scaleFactors)
{
    auto coneSource = vtkSmartPointer<vtkConeSource>::New();
    coneSource->SetRadius(cone.getBaseRadius());
    coneSource->SetHeight(cone.getHeight());
    coneSource->Update();

    auto coneMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper->SetInputConnection(coneSource->GetOutputPort());

    auto coneActor = vtkSmartPointer<vtkActor>::New();
    coneActor->SetMapper(coneMapper);
    double colorTable[3];
    getDGColor(cone,colorTable);
    coneActor->GetProperty()->SetColor(colorTable);
    coneActor->GetProperty()->SetOpacity(cone.getOpacity()<0?1:cone.getOpacity());

    coneActor->SetScale(scaleFactors);
    coneActor->SetUserMatrix(openSimToVtkTransform(coneTransform));
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(coneActor);
    return coneActor;
}

vtkSmartPointer<vtkActor> vsVisualizerVTK::renderDecorativePoint(const SimTK::DecorativePoint &point, SimTK::Transform coneTransform, double *scaleFactors)
{
    auto pointSource = vtkSmartPointer<vtkPointSource>::New();
    pointSource->SetRadius(point.getLineThickness());
    pointSource->Update();

    auto pointMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    pointMapper->SetInputConnection(pointSource->GetOutputPort());

    auto pointActor = vtkSmartPointer<vtkActor>::New();
    pointActor->SetMapper(pointMapper);
    double colorTable[3];
    getDGColor(point,colorTable);
    pointActor->GetProperty()->SetColor(colorTable);
    pointActor->GetProperty()->SetOpacity(point.getOpacity()<0?1:point.getOpacity());

    pointActor->SetScale(scaleFactors);
    pointActor->SetUserMatrix(openSimToVtkTransform(coneTransform));
    vtkRenderer *renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    renderer->AddActor(pointActor);
    return pointActor;
}

vtkSmartPointer<vtkProp> vsVisualizerVTK::renderDecorativeFrame(const SimTK::DecorativeFrame &frame, SimTK::Transform frameTransform, double *scaleFactors)
{

    auto renderer = renderWindow()->GetRenderers()->GetFirstRenderer();

    vtkSmartPointer<vtkTransform> transform =
    vtkSmartPointer<vtkTransform>::New();

    vtkSmartPointer<vtkAxesActor> frameActor =
    vtkSmartPointer<vtkAxesActor>::New();

    // The axes are positioned with a user transform
    frameActor->SetUserTransform(transform);
    frameActor->AxisLabelsOff();
    //frameActor->SetCylinderRadius(0.02);
    //frameActor->SetShaftTypeToCylinder();
    frameActor->SetTotalLength(0.2,0.2,0.2);
    //frameActor->SetConeRadius(0.01);
    frameActor->SetUseBounds(false);


    double colorTable[3];
    getDGColor(frame,colorTable);
//    frameActor->GetProperty()->SetColor(colorTable);
//    frameActor->GetProperty()->SetOpacity(frame.getOpacity()<0?1:frame.getOpacity());

    frameActor->SetScale(scaleFactors);
    frameActor->SetUserMatrix(openSimToVtkTransform(frameTransform));
    renderer->AddActor(frameActor);
    return frameActor;
}

void vsVisualizerVTK::updateVtkButtons()
{
    //redisplay the buttons inside the vtk visualizer widget
    //return;
    mXButton = createButton(0,0,"./vtk_images/frontView_axes.png");
    pXButton = createButton(0,1,"./vtk_images/backView_axes.png");
    mYButton = createButton(1,1,"./vtk_images/bottomView_axes.png");
    pYButton = createButton(1,0,"./vtk_images/topView_axes.png");
    mZButton = createButton(2,0,"./vtk_images/leftView_axes.png");
    pZButton = createButton(2,1,"./vtk_images/rightView_axes.png");
    zoomInButton = createButton(0,2,"./vtk_images/zoom-in.png");
    zoomOutButton = createButton(1,2,"./vtk_images/zoom-out.png");
    fitButton = createButton(2,2,"./vtk_images/refit.png");
    snapShotButton = createButton(0,3,"./vtk_images/stillCamera.png");
    recordButton = createButton(1,3,"./vtk_images/movieCamera.png");
    globalFramButton = createButton(2,3,"./vtk_images/axes.png");
    toggleGroundButton = createButton(0,4,"./vtk_images/toggle_ground.png");

}

vtkSmartPointer<vtkButtonWidget> vsVisualizerVTK::createButton(int posx,int posy, QString imagePath)
{

    auto imgReader = vtkSmartPointer<vtkPNGReader>::New();
    imgReader->SetFileName(imagePath.toStdString().data());
    imgReader->Update();


      auto renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();

      // An interactor
      vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      renderWindow()->GetInteractor();

      // Create the widget and its representation
      vtkSmartPointer<vtkTexturedButtonRepresentation2D> buttonRepresentation =
        vtkSmartPointer<vtkTexturedButtonRepresentation2D>::New();
      buttonRepresentation->SetNumberOfStates(1);
      buttonRepresentation->SetButtonTexture(0,imgReader->GetOutput());
      //buttonRepresentation->SetButtonTexture(1, image2);

      auto buttonWidget =
        vtkSmartPointer<vtkButtonWidget>::New();
      buttonWidget->SetInteractor(renderWindowInteractor);
      buttonWidget->SetRepresentation(buttonRepresentation);


      renderWindow()->Render();

      vtkSmartPointer<vtkCoordinate> upperLeft =
        vtkSmartPointer<vtkCoordinate>::New();
      upperLeft->SetCoordinateSystemToNormalizedViewport();
      //upperLeft.set
      upperLeft->SetValue(0,1);

      double bds[6];
      double sz = 20.0;
      bds[0] = sz + upperLeft->GetComputedDisplayValue(renderer)[0] + (sz+10) * (posx);
      bds[1] = bds[0] + sz;
      bds[2] = upperLeft->GetComputedDisplayValue(renderer)[1] - (sz+5) * (posy+1);
      bds[3] = bds[2] + sz;
      bds[4] = bds[5] = 0.0;

      buttonRepresentation->SetPlaceFactor(1);
      buttonRepresentation->PlaceWidget(bds);

      buttonWidget->On();
      //renderWindowInteractor->Start();

      connections->Connect(buttonWidget,vtkCommand::StateChangedEvent,this,SLOT(vtkButtonClicked(vtkObject *)));
      return buttonWidget;
}

void vsVisualizerVTK::takeSnapShot()
{
    //getting the file name
    QString snapshotPath ="./Snapshots/"+QDateTime::currentDateTime().toString(Qt::ISODate).replace(":","_")+".png";
    QDir hDir;
    if(!hDir.exists(QApplication::applicationDirPath()+"/Snapshots/"))
        hDir.mkdir(QApplication::applicationDirPath()+"/Snapshots/");

    //saving
    auto renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    vtkSmartPointer<vtkWindowToImageFilter> snapshotFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    snapshotFilter->SetInput(renderWindow());
    snapshotFilter->SetInputBufferTypeToRGBA();
    snapshotFilter->ReadFrontBufferOff();
    snapshotFilter->Update();

    vtkSmartPointer<vtkPNGWriter> writer =
    vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName(snapshotPath.toStdString().data());
    writer->SetInputConnection(snapshotFilter->GetOutputPort());
    writer->Write();
    vsOpenSimTools::tools->log("snapshot taken : "+snapshotPath,"VisualizerVTK",vsOpenSimTools::Success);

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
    currentModel = model;
    auto renderer  = renderWindow()->GetRenderers()->GetFirstRenderer();
    auto viewAngle = renderer->GetActiveCamera()->GetDistance();
    //visualizer solution
    //loading the Components
    if(!updating)model->getSystem().realize(model->updWorkingState(),SimTK::Stage::Position);
    SimTK::Array_<SimTK::DecorativeGeometry> compDecorations;

    //TODO try to implement frame geometries as Axes
    model->updDisplayHints().set_show_frames(false);
    //model->updDisplayHints().set_show_path_geometry(true);
//    model->updDisplayHints().set_show_contact_geometry(true);
//    model->updDisplayHints().set_show_wrap_geometry(true);
//    model->updDisplayHints().set_show_markers(true);
//    model->updDisplayHints().set_show_path_points(true);
//    model->updDisplayHints().set_show_debug_geometry(true);
    const OpenSim::ModelDisplayHints displayHints = model->getDisplayHints();

    vsGeometryImplementationQt geoImp(this,model->getSystem().getMatterSubsystem(),model->getWorkingState());
    geoImp.setRenderedModel(model);
    geoImp.setIsUpdate(false);
    modelActorsMap.insert(model,new QList<vtkSmartPointer<vtkProp>>());
    actorObjectsMap.insert(model,new QList<OpenSim::Object*>());
    updateIndecies.insert(model,new QMap<vtkSmartPointer<vtkProp>,bool>());


    OpenSim::ComponentList<const OpenSim::Component> componentList = model->getComponentList();
    OpenSim::ComponentListIterator<const OpenSim::Component> itr = componentList.begin();
    while (!itr.equals(componentList.end())) {
        const OpenSim::Component *comp = &itr.deref();
        componentActorsMap.insert(const_cast<OpenSim::Component*>(comp),new QList<vtkSmartPointer<vtkProp>>());
        actorObjectsMap.value(model)->append(const_cast<OpenSim::Component*>(comp));
        SimTK::Array_<SimTK::DecorativeGeometry> compDecorations;
        geoImp.setRenderedComponent(const_cast<OpenSim::Component*>(comp));
        comp->generateDecorations(false,displayHints,model->getWorkingState(),compDecorations);
        comp->generateDecorations(true,displayHints,model->getWorkingState(),compDecorations);
        //qDebug() << "the model subcomponents List Count >"<< compDecorations.size()<< " "
        //<<QString::fromStdString(itr.deref().getName())
        //<< QString::fromStdString(itr.deref().getConcreteClassName());
        for (int i = 0; i < compDecorations.size(); ++i) {
            geoImp.setComponentActorIndex(i);
            compDecorations.at(i).implementGeometry(geoImp);
        }

        itr.next();
    }

    //TODO add the custom Window as in this discussion https://discourse.vtk.org/t/camera-clipping-with-2-renderers/2483/9
    //renderer->ResetCamera();
    //renderer->GetActiveCamera()->SetDistance(viewAngle);
    //renderer->GetActiveCamera()->SetFocalPoint(0,0,0);
    //renderer->SetNearClippingPlaneTolerance(0.5);
    //renderer->Render();
    double bounds[] = {0,0,0,0,0,0};
    getModelBounds(model,bounds);
    renderer->ResetCamera(bounds);
    renderWindow()->Render();
    renderWindow()->Render();
    //renderer->perspe;
    //renderWindow()->Finalize();
    //renderer->GetActiveCamera()->set
//    for (SimTK::Stage stage = SimTK::Stage::Empty; stage <= model->getWorkingState().getSystemStage(); stage++) {
//        model->getSystem().calcDecorativeGeometryAndAppend(model->getWorkingState(),stage,compDecorations);
//    }
//    //qDebug() << "the size of the geometry array > " << compDecorations.size();
//    for (unsigned i = 0; i < compDecorations.size(); ++i) {
//        compDecorations.at(i).implementGeometry(geoImp);
//    }


}

void vsVisualizerVTK::updateModelDecorations(OpenSim::Model *model)
{
    auto renderer  = renderWindow()->GetRenderers()->GetFirstRenderer();

    //model->getSystem().realize(model->updWorkingState(),SimTK::Stage::Position);
    SimTK::Array_<SimTK::DecorativeGeometry> compDecorations;
    const OpenSim::ModelDisplayHints displayHints = model->getDisplayHints();

    vsGeometryImplementationQt geoImp(this,model->getSystem().getMatterSubsystem(),model->getWorkingState());
    geoImp.setRenderedModel(model);
    geoImp.setIsUpdate(true);
    //model->updWorkingState().
    //checking that the model geometries already exist
    if(!modelActorsMap.contains(model)){
        qDebug() << "cant update model as No geometries are found";
        return;
    }
    //auto modelActors = modelActorsMap.value(model);
    //modelActorsMap.insert(model,new QList<vtkSmartPointer<vtkProp>>());
    //actorObjectsMap.insert(model,new QList<OpenSim::Object*>());

    OpenSim::ComponentList<OpenSim::Component> componentList =model->updComponentList();
    OpenSim::ComponentListIterator<OpenSim::Component> itr = componentList.begin();
    while (!itr.equals(componentList.end())) {
        const OpenSim::Component *comp = &itr.deref();
        SimTK::Array_<SimTK::DecorativeGeometry> compDecorations;
        geoImp.setRenderedComponent(const_cast<OpenSim::Component*>(comp));
        comp->generateDecorations(false,displayHints,model->getWorkingState(),compDecorations);
        comp->generateDecorations(true,displayHints,model->getWorkingState(),compDecorations);

        for (int i = 0; i < compDecorations.size(); ++i) {
            geoImp.setComponentActorIndex(i);
            compDecorations.at(i).implementGeometry(geoImp);
            //compDecorations.at(i).
        }

        itr.next();
    }

    //TODO add the custom Window as in this discussion https://discourse.vtk.org/t/camera-clipping-with-2-renderers/2483/9
    //renderer->ResetCamera();
    //renderer->GetActiveCamera()->SetDistance(viewAngle);
    //renderer->GetActiveCamera()->SetFocalPoint(0,0,0);
    //renderer->SetNearClippingPlaneTolerance(0.5);
    clearModelUpdateIndices(model);

    renderer->Render();
    //currentModel = model;
    //double bounds[] = {0,0,0,0,0,0};
    //getModelBounds(model,bounds);
    //renderer->ResetCamera(bounds);
    //renderWindow()->Render();
    renderWindow()->Render();
    //renderer->perspe;
    //renderWindow()->Finalize();
    //renderer->GetActiveCamera()->set
//    for (SimTK::Stage stage = SimTK::Stage::Empty; stage <= model->getWorkingState().getSystemStage(); stage++) {
//        model->getSystem().calcDecorativeGeometryAndAppend(model->getWorkingState(),stage,compDecorations);
//    }
//    //qDebug() << "the size of the geometry array > " << compDecorations.size();
//    for (unsigned i = 0; i < compDecorations.size(); ++i) {
//        compDecorations.at(i).implementGeometry(geoImp);
//    }

}

void vsVisualizerVTK::addVtkActorToMap(OpenSim::Model *model,vtkSmartPointer<vtkProp> actor)
{
    *modelActorsMap.value(model) << actor;
}

void vsVisualizerVTK::addVtkActorToComponentMap(OpenSim::Component *component, vtkSmartPointer<vtkProp> actor)
{
    *componentActorsMap.value(component) << actor;
}

OpenSim::Model *vsVisualizerVTK::getModelForActor(vtkSmartPointer<vtkActor> actor)
{
    foreach (auto model, modelActorsMap.keys()) {
        auto actorList = *modelActorsMap.value(model);
        if(actorList.contains(actor)){
            //qDebug() << "model Name " << QString::fromStdString(model->getName());
            return model;
        }

    }
    return nullptr;
}

QList<vtkSmartPointer<vtkProp> >* vsVisualizerVTK::getActorForComponent(OpenSim::Object *component)
{
    if(componentActorsMap.contains(component))
        return componentActorsMap.value(component);
    return nullptr;
}

void vsVisualizerVTK::clearModelUpdateIndices(OpenSim::Model *model)
{
    auto map_ = updateIndecies.value(currentModel);
    foreach (auto k, map_->keys()) {
        //disabling when not updated
        if(!map_->value(k)){
            k->SetVisibility(false);
        }else{
            k->SetVisibility(true);
        }
        map_->insert(k,false);
    }
}


BackgroundType vsVisualizerVTK::backgroundType() const
{
    return m_backgroundType;
}

void vsVisualizerVTK::setBackgroundType(const BackgroundType &backgroundType)
{
    vtkNew<vtkNamedColors> namedColors;
    auto renderer =this->renderWindow()->GetRenderers()->GetFirstRenderer();
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

void vsVisualizerVTK::clearTheScene()
{
    auto renderer =this->renderWindow()->GetRenderers()->GetFirstRenderer();
    //TODO remove only the models actors
    //removing the component map
    foreach(auto component , componentActorsMap.keys()){
        auto actorLists = componentActorsMap.value(component);
        componentActorsMap.remove(component);
        delete actorLists;
    }
    componentActorsMap.clear();
    foreach (auto modelObj, modelActorsMap.keys()) {
        foreach (auto actorObj, *modelActorsMap.value(modelObj)) {
            actorObj->SetVisibility(false);
            modelActorsMap.value(modelObj)->removeOne(actorObj);
            renderer->RemoveActor(actorObj);
            //actorObj->Delete();
        }
    }
    modelActorsMap.clear();
    //renderer->RemoveAllViewProps();

    renderer->ResetCamera(globalFrame->GetBounds());
    //renderer->Render();
    renderWindow()->Render();
    renderWindow()->Finalize();


}

void vsVisualizerVTK::removeModelActors(OpenSim::Model *model)
{
    auto renderer =this->renderWindow()->GetRenderers()->GetFirstRenderer();
    auto modelActors = modelActorsMap.value(model);
    foreach (auto obj, *actorObjectsMap.value(model)) {

        //TODO create a map for actor--component to faster the search
        componentActorsMap.value(obj)->clear();
        componentActorsMap.remove(obj);
    }
    foreach (auto actor, *modelActors) {
        renderer->RemoveActor(actor);
        actor->Delete();
    }
    modelActorsMap.remove(model);
    //renderer->ResetCamera();
    //renderer->Render();
    renderWindow()->Render();
    //renderWindow()->Finalize();

}

void vsVisualizerVTK::getModelBounds(OpenSim::Model *model, double *bounds)
{
    if(currentModel == nullptr) return;
    auto  modelActors = *modelActorsMap.value(model);
    foreach (vtkSmartPointer<vtkProp> actor, modelActors ) {
        double *actorBounds = actor->GetBounds();
        if(actorBounds[0]<bounds[0]) bounds[0] = actorBounds[0];
        if(actorBounds[1]>bounds[1]) bounds[1] = actorBounds[1];
        if(actorBounds[2]<bounds[2]) bounds[2] = actorBounds[2];
        if(actorBounds[3]>bounds[3]) bounds[3] = actorBounds[3];
        if(actorBounds[4]<bounds[4]) bounds[4] = actorBounds[4];
        if(actorBounds[5]>bounds[4]) bounds[5] = actorBounds[5];
    }
}

void vsVisualizerVTK::focusOnCurrentModel()
{
    double modelBounds[6] = {-1,1,-1,1,-1,1};
    if(currentModel != nullptr){
        getModelBounds(currentModel,modelBounds);
    }
    renderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera(modelBounds);

}

void vsVisualizerVTK::selectActorInNavigator(vtkSmartPointer<vtkActor> actor)
{
    OpenSim::Object *selectedObject = getOpenSimObjectForActor(actor);
    if(selectedObject == nullptr) return;
    emit this->objectSelectedInNavigator(selectedObject);
    qDebug() << "selected object " << QString::fromStdString(selectedObject->getName());
}

OpenSim::Object *vsVisualizerVTK::getOpenSimObjectForActor(vtkSmartPointer<vtkActor> actor)
{
    foreach (OpenSim::Object *obj, componentActorsMap.keys()) {
        if(componentActorsMap.value(obj)->contains(actor.Get())){
            return  obj;
        }
    }
    return nullptr;
}

void vsVisualizerVTK::setComponetVisibility(OpenSim::Object *obj, bool visible)
{
    auto componentProbs = componentActorsMap.value(obj);
    if(componentProbs){
        foreach (auto prop, *componentProbs) {
            prop->SetVisibility(visible);
        }
    }
    renderWindow()->Render();
}

void vsVisualizerVTK::highlightComponentsProps(OpenSim::Object *obj)
{
    //TODO save components colors and apply the selection color
}

void vsVisualizerVTK::vtkButtonClicked(vtkObject *clickedObject)
{
    auto renderer = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    //TODO setup the camera to look at the Actor instead of the origin
    auto currentCamera = this->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
    if(clickedObject == pXButton.Get()){
       //auto fp = currentCamera->GetFocalPoint();
       double newPos[3] = {2,0.5,0};
       currentCamera->SetPosition(newPos);
       //currentCamera->SetFocalPoint(0,0.5,0.5);
       currentCamera->SetViewUp(0,1,0);
       //qDebug() << "the signal is working";
        focusOnCurrentModel();
    }
    else if(clickedObject == mXButton.Get()){
       //auto fp = currentCamera->GetFocalPoint();
       double newPos[3] = {-2,0.5,0};
       currentCamera->SetPosition(newPos);
       //currentCamera->SetFocalPoint(0,0.5,0.5);
       currentCamera->SetViewUp(0,1,0);
       //qDebug() << "the signal is working";
        //renderer->ResetCamera();
       focusOnCurrentModel();
    }
    else if(clickedObject == pYButton.Get()){
        //auto fp = currentCamera->GetFocalPoint();
        double newPos[3] = {0,2,0};
        currentCamera->SetPosition(newPos);
        //currentCamera->SetFocalPoint(0.5,0,0.5);
        currentCamera->SetViewUp(0,0,-1);
        //qDebug() << "the signal is working";
        //renderer->ResetCamera();
        focusOnCurrentModel();
    }
    else if(clickedObject == mYButton.Get()){
        //auto fp = currentCamera->GetFocalPoint();
        double newPos[3] = {0,-2,0};
        currentCamera->SetPosition(newPos);
        //currentCamera->SetFocalPoint(0.5,0.0,0.5);
        currentCamera->SetViewUp(0,0,1);
        //qDebug() << "the signal is working";
        //renderer->ResetCamera();
        focusOnCurrentModel();
    }
    else if(clickedObject == pZButton.Get()){
        //auto fp = currentCamera->GetFocalPoint();
        double newPos[3] = {0,0.5,2};
        currentCamera->SetPosition(newPos);
        //currentCamera->SetFocalPoint(0.5,0.5,0);
        currentCamera->SetViewUp(0,1,0);
        //qDebug() << "the signal is working";
        //renderer->ResetCamera();
        focusOnCurrentModel();
    }
    else if(clickedObject == mZButton.Get()){
        //auto fp = currentCamera->GetFocalPoint();
        double newPos[3] = {0,0.5,-2};
        currentCamera->SetPosition(newPos);
        //currentCamera->SetFocalPoint(0.5,0.5,0);
        currentCamera->SetViewUp(0,1,0);
        //qDebug() << "the signal is working";
        //renderer->ResetCamera(globalFrame->GetBounds());
        focusOnCurrentModel();
    }
    else if(clickedObject == zoomOutButton.Get()){
        //auto fp = currentCamera->GetFocalPoint();
//        double *orientation = currentCamera->GetO
//        double *position = currentCamera->GetPosition();
//        double newPos[3] ={ orientation[0]+position[0],orientation[1]+position[1],orientation[2]+position[2]};
//        currentCamera->SetPosition(newPos);

        currentCamera->Zoom(0.9);
    }
    else if(clickedObject == zoomInButton.Get()){
        //auto fp = currentCamera->GetFocalPoint();
//        double *orientation = currentCamera->GetO
//        double *position = currentCamera->GetPosition();
//        double newPos[3] ={ orientation[0]+position[0],orientation[1]+position[1],orientation[2]+position[2]};
//        currentCamera->SetPosition(newPos);
        currentCamera->Zoom(1.1);
    }
    else if(clickedObject == fitButton.Get()){

        //renderer->ResetCamera();
        focusOnCurrentModel();
    }
    else if(clickedObject == snapShotButton.Get()){
        takeSnapShot();
    }
    else if (clickedObject == globalFramButton.Get()) {
        globalFrame->SetVisibility(!globalFrame->GetVisibility());
    }
    else if(clickedObject == toggleGroundButton.Get()){
        ground->SetVisibility(!ground->GetVisibility());
    }


    //this->renderWindow()->GetRenderers()->GetFirstRenderer()->Render();
    renderWindow()->Render();
    renderWindow()->Finalize();
    //    //qDebug() << "the signal is working" << (clickedObject == pXButton.Get());
}

void vsVisualizerVTK::onVtkDoubleClicked(vtkObject *obj)
{
    int x = 0;
    int y = 0;
    interactor()->GetEventPosition(x,y);
    //interactor()->GetMousePosition(&x,&y);
    int picked = propPicker->PickProp(x,y,renderWindow()->GetRenderers()->GetFirstRenderer());
    if(picked == 1){
        if(propPicker->GetProp3D() == ground.Get() || propPicker->GetProp3D() == globalFrame.Get()){
            return;
        }
        selectActorInNavigator(propPicker->GetActor());

        //updating the selected actor pointer and color
        if(propPicker->GetActor()){
            if(selectedActor){
                selectedActor->GetProperty()->SetColor(selectedActorColorBackup);
            }
            selectedActor =  propPicker->GetActor();
            selectedActor->GetProperty()->GetColor(selectedActorColorBackup);
            selectedActor->GetProperty()->SetColor(1,.6,0);
        }

    }

}

void vsVisualizerVTK::onInteractorPick(vtkObject *obj)
{
    //qDebug() << "double clicked ";
}

void vsVisualizerVTK::resizeEvent(QResizeEvent *event)
{
    QVTKOpenGLNativeWidget::resizeEvent(event);
    updateVtkButtons();
}

void vsVisualizerVTK::paintEvent(QPaintEvent *event)
{
    QVTKOpenGLNativeWidget::paintEvent(event);
    //auto renderer  = this->renderWindow()->GetRenderers()->GetFirstRenderer();
    //renderer->ResetCameraClippingRange(0.001,10000,0.0001,10000,0.0001,10000);
}

void vsVisualizerVTK::showEvent(QShowEvent *event)
{
    QVTKOpenGLNativeWidget::showEvent(event);
    updateVtkButtons();
}
