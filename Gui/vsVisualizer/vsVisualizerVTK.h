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
#include <vtkPropPicker.h>
#include <vtkPolyDataMapper.h>

enum class BackgroundType{
    Solid,GroundAndSky
};

class vsVisualizerVTK : public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    vsVisualizerVTK(QWidget *parent = nullptr);
    void renderingTest();

    bool updating = false;

    void getDGColor(const SimTK::DecorativeGeometry& geo,double *color_out);

    vtkSmartPointer<vtkActor> renderGeometry(OpenSim::Geometry *geometry);
    vtkSmartPointer<vtkActor> addBox();
    vtkSmartPointer<vtkActor> addGround();
    void createGroundImage(vtkSmartPointer<vtkImageData> groundData,int w, int h);
    vtkSmartPointer<vtkActor> addSkyBox();
    vtkSmartPointer<vtkAxesActor> addGlobalFrame();
    vtkSmartPointer<vtkPolyDataMapper> getMeshDataMapper(std::string fileName);

    void updateDecorativeGeometry(OpenSim::Object *obj,int actorIndex,const SimTK::DecorativeGeometry& geo,SimTK::Transform geometryTransform,double *scaleFactors);

    vtkSmartPointer<vtkActor> renderDecorativeMeshFile(const SimTK::DecorativeMeshFile& mesh,
                                                       SimTK::Transform mesh_transform, double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativeSphere(const SimTK::DecorativeSphere& sphere, SimTK::Transform sphereTransform,
                                                     double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativeEllipsoid(const SimTK::DecorativeEllipsoid& ellipsoid, SimTK::Transform ellipsoidTransform,double *scaleFactors);
    vtkSmartPointer<vtkActor> renderDecorativeLine(const SimTK::DecorativeLine& line, SimTK::Transform lineTransform,
                                                   double *scaleFactors,OpenSim::Object *obj = nullptr,int actorIndex=0);
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

    vtkSmartPointer<vtkProp> renderDecorativeFrame(const SimTK::DecorativeFrame& frame, SimTK::Transform frameTransform,
                                                   double *scaleFactors);


    void updateVtkButtons();
    vtkSmartPointer<vtkButtonWidget> createButton(int posx,int posy,QString imagePath);
    void takeSnapShot();




    vtkSmartPointer<vtkMatrix4x4> openSimToVtkTransform(SimTK::Transform stkTransform);
    void addOpenSimModel(OpenSim::Model *model);
    void updateModelDecorations(OpenSim::Model *model);
    void addVtkActorToMap(OpenSim::Model *model,vtkSmartPointer<vtkProp> actor);
    void addVtkActorToComponentMap(OpenSim::Component *compoenent,vtkSmartPointer<vtkProp> actor);
    OpenSim::Model* getModelForActor(vtkSmartPointer<vtkActor> actor);
    QList<vtkSmartPointer<vtkProp>>* getActorForComponent(OpenSim::Object *component);
    void clearModelUpdateIndices(OpenSim::Model *model);


    BackgroundType backgroundType() const;
    void setBackgroundType(const BackgroundType &backgroundType);

    //operations on the scene
    void clearTheScene();
    void removeModelActors(OpenSim::Model *model);
    void getModelBounds(OpenSim::Model *model,double *bounds);
    void focusOnCurrentModel();
    void selectActorInNavigator(vtkSmartPointer<vtkActor> actor);
    OpenSim::Object* getOpenSimObjectForActor(vtkSmartPointer<vtkActor> actor);
    void selectOpenSimObject(OpenSim::Object *obj);

    //Navigator Actions
    void setComponetVisibility(OpenSim::Object *obj,bool visible);
    void highlightComponentsProps(OpenSim::Object *obj);

public slots:
    void vtkButtonClicked(vtkObject *clickedObject);
    void onVtkDoubleClicked(vtkObject *obj);
    void onInteractorPick(vtkObject *obj);

private:
    BackgroundType m_backgroundType;
    QMap<OpenSim::Model*, QList<vtkSmartPointer<vtkProp>>*> modelActorsMap;
    QMap<OpenSim::Object*, QList<vtkSmartPointer<vtkProp>>*> componentActorsMap;
    QMap<OpenSim::Model*, QList<OpenSim::Object*>*> actorObjectsMap;

    //optimizing performance
    QMap<vtkSmartPointer<vtkActor>,vtkSmartPointer<vtkLineSource>> muscleActorLineSourceMap;


    OpenSim::Model *currentModel;

    //scene actors
    vtkSmartPointer<vtkRenderer> backgroundRenderer;
    vtkSmartPointer<vtkActor> skyBox;
    vtkSmartPointer<vtkActor> ground;
    vtkSmartPointer<vtkAxesActor> globalFrame;
    vtkSmartPointer<vtkPropPicker> propPicker;

    vtkSmartPointer<vtkActor> selectedActor;
    double selectedActorColorBackup[3];

    //updating index
    QMap<OpenSim::Model*,QMap<vtkSmartPointer<vtkProp>,bool>*> updateIndecies;

    //selection members
    OpenSim::Object *m_selectedOpenSimObject = nullptr;
    QMap<vtkSmartPointer<vtkProp>,double*> selectedOpenSimObjectColors;

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

    vtkSmartPointer<vtkButtonWidget> toggleGroundButton;




    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
signals:
    // signal to inform the MainWindow which OpenSim Object is selected
    void objectSelectedInNavigator(OpenSim::Object *obj);

};

#endif // VTKVISUALIZER_H
