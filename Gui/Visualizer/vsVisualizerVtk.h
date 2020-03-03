#ifndef VTKVISUALIZER_H
#define VTKVISUALIZER_H

#include <QVTKOpenGLWidget.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <QVector3D>
#include <QVector4D>
#include <OpenSim.h>

enum class BackgroundType{
    Solid,GroundAndSky
};

class vsVisualizerVTK : public QVTKOpenGLWidget
{
    Q_OBJECT
public:
    vsVisualizerVTK(QWidget *parent = nullptr);
    void renderingTest();

    void getDGColor(const SimTK::DecorativeGeometry& geo,double *color_out);

    vtkSmartPointer<vtkActor> vsVisualizerVTK::renderGeometry(OpenSim::Geometry *geometry);
    vtkSmartPointer<vtkActor> addBox();
    vtkSmartPointer<vtkActor> addGround();
    vtkSmartPointer<vtkActor> addSkyBox();
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


    vtkSmartPointer<vtkMatrix4x4> openSimToVtkTransform(SimTK::Transform stkTransform);
    void addOpenSimModel(OpenSim::Model *model);

    BackgroundType backgroundType() const;
    void setBackgroundType(const BackgroundType &backgroundType);

private:
    BackgroundType m_backgroundType;


};

#endif // VTKVISUALIZER_H
