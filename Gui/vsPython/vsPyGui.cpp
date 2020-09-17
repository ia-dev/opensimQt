#include <vsPython/vsPyGui.h>
#include <vsPython/vsMacroManager.h>
#include <vsWidgets/vsMainWindow.h>
#include <QMenuBar>
#include <QMenu>


vsPyGui::vsPyGui(QObject *parent) : QObject{parent}
{
    //init the class variable name to by used in python scripting
    classVar = "vsGui";

    vsMainWindow* mainWindow = static_cast<vsMainWindow*>(parent->parent());
    auto menus = mainWindow->menuBar()->findChildren<QMenu*>();
    for(QMenu* m:menus){
            menu_actions.append(m->actions());
    }

    //connect all the action - slots of File Menu and File Toolbar
    connect(this,SIGNAL(openModel()),mainWindow,SLOT(on_actionOpen_Model_triggered()));
    QAction* openAction = findAction("actionOpen_Model");
    vsMacroManager::instance().registerAction(classVar+".openModel()",openAction);

    connect(this,SIGNAL(newModel()),mainWindow,SLOT(on_actionNew_Model_triggered()));
    QAction* newAction = findAction("actionNew_Model");
    vsMacroManager::instance().registerAction(classVar+".newModel()",newAction);

    connect(this,SIGNAL(loadMotion()),mainWindow,SLOT(on_actionLoad_Motion_triggered()));
    QAction* loadAction = findAction("actionLoad_Motion");
    vsMacroManager::instance().registerAction(classVar+".loadMotion()",loadAction);

    connect(this,SIGNAL(closeModel()),mainWindow,SLOT(on_actionClose_Model_triggered()));
    QAction* closeAction = findAction("actionClose_Model");
    vsMacroManager::instance().registerAction(classVar+".closeModel()",closeAction);

    connect(this,SIGNAL(closeAll()),mainWindow,SLOT(on_actionClose_All_triggered()));
    QAction* closeAllAction = findAction("actionClose_All");
    vsMacroManager::instance().registerAction(classVar+".closeAll()",closeAllAction);

    connect(this,SIGNAL(saveModel()),mainWindow,SLOT(on_actionSave_Model_triggered()));
    QAction* saveModelAction = findAction("actionSave_Model");
    vsMacroManager::instance().registerAction(classVar+".saveModel()",saveModelAction);

    connect(this,SIGNAL(saveModelAs()),mainWindow,SLOT(on_actionSave_Model_As_triggered()));
    QAction* saveModelAsAction = findAction("actionSave_Model_As");
    vsMacroManager::instance().registerAction(classVar+".saveModelAs()",saveModelAsAction);

    connect(this,SIGNAL(saveAll()),mainWindow,SLOT(on_actionSave_All_triggered()));
    QAction* saveAllAction = findAction("actionSave_All");
    vsMacroManager::instance().registerAction(classVar+".saveAll()",saveAllAction);

    connect(this,SIGNAL(reload()),mainWindow,SLOT(on_actionReload_triggered()));
    QAction* reloadAction = findAction("actionReload");
    vsMacroManager::instance().registerAction(classVar+".reload()",reloadAction);

    connect(this,SIGNAL(exit()),mainWindow,SLOT(on_actionE_xit_triggered()));
    QAction* exitAction = findAction("actionE_xit");
    vsMacroManager::instance().registerAction(classVar+".exit()",exitAction);


    //connect all the tools menu actions

//    connect(this,SIGNAL(plot()),parent,SLOT(on_actionOpen_Model_triggered()));
//    connect(this,SIGNAL(scaleModel()),parent,SLOT(on_actionNew_Model_triggered()));
//    connect(this,SIGNAL(inverseKinematics()),parent,SLOT(on_actionReload_triggered()));
//    connect(this,SIGNAL(inverseDynamics()),parent,SLOT(on_actionSave_Model_triggered()));
//    connect(this,SIGNAL(staticOptimization()),parent,SLOT(on_actionSave_Model_As_triggered()));
//    connect(this,SIGNAL(reduceResiduals()),parent,SLOT(on_actionSave_All_triggered()));
//    connect(this,SIGNAL(computeMuscleControl()),parent,SLOT(on_actionE_xit_triggered()));
//    connect(this,SIGNAL(forwardDynamics()),parent,SLOT(on_actionClose_Model_triggered()));
//    connect(this,SIGNAL(analyze()),parent,SLOT(on_actionClose_All_triggered()));
//    connect(this,SIGNAL(convertFiles()),parent,SLOT(on_actionLoad_Motion_triggered()));

    //connect simulation tools

}

QAction* vsPyGui::findAction(const QString& objectName)
{

     auto itrAction = std::find_if(menu_actions.begin(),menu_actions.end(),[&objectName](QAction* a)
                            {
                                return a->objectName() == objectName;
                            }
                 );
     if(itrAction == menu_actions.end()){
         return NULL;
     }
     return static_cast<QAction*>(*itrAction);
}


