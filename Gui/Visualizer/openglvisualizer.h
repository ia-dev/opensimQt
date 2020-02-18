#ifndef OPENGLVISUALIZER_H
#define OPENGLVISUALIZER_H

#include <QOpenGLWidget>



class OpenGLVisualizer : public QOpenGLWidget
{
    Q_OBJECT
public:
    OpenGLVisualizer(QWidget *parent = nullptr);

    // QOpenGLWidget interface
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};

#endif // OPENGLVISUALIZER_H
