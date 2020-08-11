#ifndef vsOpenGLVisualizer_H
#define vsOpenGLVisualizer_H

#include <QOpenGLWidget>



class vsOpenGLVisualizer : public QOpenGLWidget
{
    Q_OBJECT
public:
    vsOpenGLVisualizer(QWidget *parent = nullptr);

    // QOpenGLWidget interface
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};

#endif // vsOpenGLVisualizer_H
