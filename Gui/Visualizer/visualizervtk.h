#ifndef VTKVISUALIZER_H
#define VTKVISUALIZER_H

#include <QVTKOpenGLWidget.h>



class VisualizerVTK : public QVTKOpenGLWidget
{
    Q_OBJECT
public:
    VisualizerVTK(QWidget *parent = nullptr);
};

#endif // VTKVISUALIZER_H
