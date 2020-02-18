#include "openglvisualizer.h"

#include <QOpenGLFunctions>


OpenGLVisualizer::OpenGLVisualizer(QWidget *parent):
    QOpenGLWidget(parent)
{

}

void OpenGLVisualizer::initializeGL()
{

}

void OpenGLVisualizer::resizeGL(int w, int h)
{

}

void OpenGLVisualizer::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->initializeOpenGLFunctions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
