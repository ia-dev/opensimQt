#ifndef OPENGLVISUALIZER_H
#define OPENGLVISUALIZER_H

#include <QOpenGLWidget>



class OpenGLVisualizer : public QOpenGLWidget
{
    Q_OBJECT
public:
    OpenGLVisualizer(QWidget *parent = nullptr);
};

#endif // OPENGLVISUALIZER_H
