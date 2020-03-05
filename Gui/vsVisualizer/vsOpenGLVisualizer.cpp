#include "vsOpenGLVisualizer.h"

#include <QOpenGLFunctions>


vsOpenGLVisualizer::vsOpenGLVisualizer(QWidget *parent):
    QOpenGLWidget(parent)
{

}

void vsOpenGLVisualizer::initializeGL()
{

}

void vsOpenGLVisualizer::resizeGL(int w, int h)
{

}

void vsOpenGLVisualizer::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->initializeOpenGLFunctions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
