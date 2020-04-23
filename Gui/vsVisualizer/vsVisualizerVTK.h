#ifndef VTKVISUALIZER_H
#define VTKVISUALIZER_H

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <QVector3D>
#include <QVector4D>
#include <OpenSim.h>
#include <QMap>
#include <vtkImageData.h>
#include <vtkButtonWidget.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkAxesActor.h>
#include <vtkTextureMapToPlane.h>
#include <QVTKOpenGLStereoWidget.h>
#include <QVTKOpenGLNativeWidget.h>

enum class BackgroundType{
    Solid,GroundAndSky
};

class vsVisualizerVTK : public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    vsVisualizerVTK(QWidget *parent = nullptr);
    void renderingTest();

    void getDGColor(const SimTK::DecorativeGeometry& geo,double *color_out);

    vtkSmartPointer<vtkActor> renderGeometry(OpenSim::Geometry *geometry);
    vtkSmartPointer<vtkActor> addBox();
    vtkSmartPointer<vtkActor> addGround();
    void createGroundImage(vtkSmartPointer<vtkImageData> groundData,int w, int h);
    vtkSmartPointer<vtkActor> addSkyBox();
    vtkSmartPointer<vtkAxesActor> addGlobalFrame();

    vtkSmartPointer<vtkActor> renderDecorativeMeshFile(const SimTK::DecorativeMeshFile& mesh,
                                                       SimTK::Transform mesh_transform, double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativeSphere(const SimTK::DecorativeSphere& sphere, SimTK::Transform sphereTransform,
                                                     double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativeEllipsoid(const SimTK::DecorativeEllipsoid& ellipsoid, SimTK::Transform ellipsoidTransform,
                                                     double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativeLine(const SimTK::DecorativeLine& line, SimTK::Transform lineTransform,
                                                   double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativeCylender(const SimTK::DecorativeCylinder& cylender, SimTK::Transform cylanderTransform,
                                                   double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativeBrick(const SimTK::DecorativeBrick& brick, SimTK::Transform brickTransform,
                                                       double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativeCircle(const SimTK::DecorativeCircle& circle, SimTK::Transform brickTransform,
                                                    double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativeTorus(const SimTK::DecorativeTorus& torus, SimTK::Transform torusTransform,
                                                     double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativeText(const SimTK::DecorativeText& text, SimTK::Transform textTransform,
                                                    double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativeArrow(const SimTK::DecorativeArrow& arrow, SimTK::Transform arrowTransform,
                                                    double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativeCone(const SimTK::DecorativeCone& cone, SimTK::Transform coneTransform,
                                                    double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativePoint(const SimTK::DecorativePoint& point, SimTK::Transform coneTransform,
                                                   double *scaleFactors);

    void updateVtkButtons();
    vtkSmartPointer<vtkButtonWidget> createButton(int posx,int posy,QString imagePath);
    void takeSnapShot();




    vtkSmartPointer<vtkMatrix4x4> openSimToVtkTransform(SimTK::Transform stkTransform);
    void addOpenSimModel(OpenSim::Model *model);
    void addVtkActorToMap(OpenSim::Model *model,vtkSmartPointer<vtkActor> actor);
    void addVtkActorToComponentMap(OpenSim::Component *compoenent,vtkSmartPointer<vtkActor> actor);
    OpenSim::Model* getModelForActor(vtkSmartPointer<vtkActor> actor);
    QList<vtkSmartPointer<vtkActor>>* getActorForComponent(OpenSim::Object *component);

    BackgroundType backgroundType() const;
    void setBackgroundType(const BackgroundType &backgroundType);

    //operations on the scene
    void clearTheScene();
    void removeModelActors(OpenSim::Model *model);
    void getModelBounds(OpenSim::Model *model,double *bounds);
    void focusOnCurrentModel();

public slots:
    void vtkButtonClicked(vtkObject *clickedObject);
    void onVtkDoubleClicked(vtkObject *obj);
    void onInteractorPick(vtkObject *obj);

private:
    BackgroundType m_backgroundType;
    QMap<OpenSim::Model*, QList<vtkSmartPointer<vtkActor>>*> modelActorsMap;
    QMap<OpenSim::Object*, QList<vtkSmartPointer<vtkActor>>*> componentActorsMap;

    OpenSim::Model *currentModel;

    //scene actors
    vtkSmartPointer<vtkRenderer> backgroundRenderer;
    vtkSmartPointer<vtkActor> skyBox;
    vtkSmartPointer<vtkActor> ground;
    vtkSmartPointer<vtkAxesActor> globalFrame;

    //vtk to qt slots connection

    vtkSmartPointer<vtkEventQtSlotConnect> connections;

    //buttons
    vtkSmartPointer<vtkButtonWidget> mXButton;
    vtkSmartPointer<vtkButtonWidget> pXButton;
    vtkSmartPointer<vtkButtonWidget> mYButton;
    vtkSmartPointer<vtkButtonWidget> pYButton;
    vtkSmartPointer<vtkButtonWidget> mZButton;
    vtkSmartPointer<vtkButtonWidget> pZButton;
    vtkSmartPointer<vtkButtonWidget> zoomOutButton;
    vtkSmartPointer<vtkButtonWidget> zoomInButton;
    vtkSmartPointer<vtkButtonWidget> fitButton;
    vtkSmartPointer<vtkButtonWidget> snapShotButton;
    vtkSmartPointer<vtkButtonWidget> recordButton;
    vtkSmartPointer<vtkButtonWidget> globalFramButton;



    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // VTKVISUALIZER_H
