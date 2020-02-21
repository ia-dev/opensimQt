#ifndef VTKVISUALIZER_H
#define VTKVISUALIZER_H

#include <QVTKOpenGLWidget.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <QVector3D>
#include <QVector4D>

enum class BackgroundType{
    Solid,GroundAndSky
};

class VisualizerVTK : public QVTKOpenGLWidget
{
    Q_OBJECT
public:
    VisualizerVTK(QWidget *parent = nullptr);
    void renderingTest();
    void renderVtpMesh(QString fileName);
    vtkSmartPointer<vtkActor> addBox();
    vtkSmartPointer<vtkActor> addGround();
    vtkSmartPointer<vtkActor> addSkyBox();

    BackgroundType backgroundType() const;
    void setBackgroundType(const BackgroundType &backgroundType);

private:
    BackgroundType m_backgroundType;


};

#endif // VTKVISUALIZER_H
