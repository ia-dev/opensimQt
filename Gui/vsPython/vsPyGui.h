//----------------------------------------------------------------------------------
/*!
// \file    vsPyGui.h
// \author  Ritesh Sangani
// \author  Last changed by $Author: ritesh $
// \date    2020-09-14
*/
//----------------------------------------------------------------------------------
#ifndef VSPYGUI_H
#define VSPYGUI_H

#include <QObject>
#include <QAction>

class vsPyGui : public QObject
{
    Q_OBJECT
public:
    vsPyGui(QObject *parent = nullptr);
    virtual ~vsPyGui() {}

signals:
    ///////////////////////////////////////////////////////////////////////////////////
    ///        FILE MENU AND RELATED TOOLBAR ACCESS THROUGH PYTHON                 ///
    //////////////////////////////////////////////////////////////////////////////////


    void openModel();
    void loadMotion();
    void closeModel();
    void closeAll();
    void saveModel();
    void saveModelAs();
    void saveAll();
    void reload();
    void previewExperimentData();
    void exit();


    ///////////////////////////////////////////////////////////////////////////////////
    ///        TOOLS MENU AND RELATED TOOLBAR ACCESS THROUGH PYTHON                ///
    //////////////////////////////////////////////////////////////////////////////////

    void plot();
    void scaleModel();
    void inverseKinematics();
    void inverseDynamics();
    void staticOptimization();
    void reduceResiduals();
    void computeMuscleControl();
    void forwardDynamics();
    void analyze();
    void convertFiles();


    void newModel();
    void poses();
private:
    QAction* findAction(const QString& actionText);

    QList<QAction*> menu_actions;
    QString classVar;

};

#endif // VSPYGUI_H
