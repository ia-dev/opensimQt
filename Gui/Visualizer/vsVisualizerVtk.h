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
    vtkSmartPointer<vtkActor> vsVisualizerVTK::renderGeometry(OpenSim::Geometry *geometry);
    vtkSmartPointer<vtkActor> addBox();
    vtkSmartPointer<vtkActor> addGround();
    vtkSmartPointer<vtkActor> addSkyBox();
    vtkSmartPointer<vtkActor> renderDecorativeMeshFile(const SimTK::DecorativeMeshFile& mesh,
                                                       SimTK::Transform mesh_transform, double *scaleFactors);

    vtkSmartPointer<vtkMatrix4x4> openSimToVtkTransform(SimTK::Transform stkTransform);
    void addOpenSimModel(OpenSim::Model *model);

    BackgroundType backgroundType() const;
    void setBackgroundType(const BackgroundType &backgroundType);

private:
    BackgroundType m_backgroundType;


};

#endif // VTKVISUALIZER_H
