#include "vsMainWindow.h"
#include "./ui_vsMainWindow.h"

#include <QLabel>
#include <OpenSim.h>
#include <QFileDialog>
#include <QDebug>
#include <QMimeData>
#include <QMessageBox>
#include <QtWebEngineWidgets/qwebengineview.h>
#include <vsTools/vsOpenSimTools.h>
#include "vsVisualizer/vsOpenGLVisualizer.h"

vsMainWindow::vsMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::vsMainWindow)
{
    ui->setupUi(this);

    setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);

    tabifyDockWidget(ui->scriptingDock,ui->messagesDock);

    //adding the Simulation Tools Widget to the toolBar

    simulationWidget = new vsSimulationToolsWidget(this);
    ui->simulationToolBar->addWidget(simulationWidget);

//    ui->messagesDock->setGeometry(ui->messagesDock->x(),ui->messagesDock->y(),ui->messagesDock->geometry().width(),150);
//    ui->scriptingDock->setGeometry(ui->messagesDock->x(),ui->messagesDock->y(),ui->messagesDock->geometry().width(),150);
    resizeDocks({ui->messagesDock,ui->scriptingDock},{150,150},Qt::Vertical);

    //setting the navigator
    navigatorModel = new vsNavigatorModel();
    ui->navigatorTreeView->setModel(navigatorModel);
    connect(navigatorModel,&vsNavigatorModel::expendIndex,this,&vsMainWindow::onExpendIndex);

    //setting the visualizer
    ui->Visualizer->load(QUrl("http:/localhost:8002/threejs/editor/index.html"));
    //ui->Visualizer->setUrl(QUrl("http://www.facebook.com"));
    //ui->Visualizer->show();

    //setting the model preferences
    OpenSim::ModelVisualizer::addDirToGeometrySearchPaths("./vsWorkSpace/opensim-models/Geometry");
    OpenSim::ModelVisualizer::addDirToGeometrySearchPaths("../Gui/vsWorkSpace/opensim-models/Geometry");

    //setting the logging
    connect(vsOpenSimTools::tools,&vsOpenSimTools::messageLogged,ui->messagesTextEdit,&QTextEdit::append);
    vsOpenSimTools::tools->log("Log display connected","",vsOpenSimTools::Success,true);

    //setting up the context menu
    ui->navigatorTreeView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(ui->navigatorTreeView,&QTreeView::customContextMenuRequested,this,&vsMainWindow::customMenuRequestedNavigator);

}

vsMainWindow::~vsMainWindow()
{
    delete ui;
}


void vsMainWindow::on_actionOpen_Model_triggered()
{
    try {
        QString fileName = QFileDialog::getOpenFileName(this,tr("Open Model From File"),QString());
        qDebug() << fileName;
        OpenSim::Model  *newModel = new OpenSim::Model(fileName.toStdString());
        qDebug() << QString::fromStdString(newModel->getName());
        navigatorModel->loadOpenSimModel(newModel);
        //TODO save the state somewhere
        ui->navigatorTreeView->update(ui->navigatorTreeView->visibleRegion());

        //update the openModelsFile
        vsOpenSimTools::tools->addToOpenModels(newModel);
    } catch (...) {
        vsOpenSimTools::tools->log("No valid OpenSim model was selected","",vsOpenSimTools::Error,true);
    }

}

void vsMainWindow::dropEvent(QDropEvent *event)
{
    try {
        const QMimeData *mimeData = event->mimeData();
        if (mimeData->hasUrls()) {

            QList<QUrl> pathList = mimeData->urls();
            foreach (QUrl url, pathList) {
               qDebug()<< "Url" << url.path() << " "<< url.toLocalFile();
               OpenSim::Model  *newModel = new OpenSim::Model(url.toLocalFile() .toStdString());
               //newModel->state
               qDebug() << QString::fromStdString(newModel->getName());
               navigatorModel->loadOpenSimModel(newModel);
               ui->navigatorTreeView->update(ui->navigatorTreeView->visibleRegion());
            }

        }
        event->acceptProposedAction();
    } catch (...) {
        vsOpenSimTools::tools->log("The Dragged file does not represent a Model","MainWindow",vsOpenSimTools::Error);
    }
}

void vsMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->setDropAction(Qt::DropAction::LinkAction);
    event->accept();
    event->acceptProposedAction();
}

void vsMainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
    event->acceptProposedAction();
}

void vsMainWindow::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::warning(this,"Exiting","Do you really want to exit ?",(QMessageBox::Yes|QMessageBox::No)) != QMessageBox::Yes)
    {
        event->ignore();
    }else{
        event->accept();
    }
}

//vtkSmartPointer<vtkRenderer> vsMainWindow::m_renderer = nullptr;

void vsMainWindow::on_actionReload_triggered()
{
    vsOpenSimTools::tools->log("Reloading");
    qDebug()<< "Reloading";
    //ui->vtkVisualiser->showMaximized();
    //remove the actors from the scene
    navigatorModel->clean();
    ui->navigatorTreeView->update(ui->navigatorTreeView->visibleRegion());
    ui->vtkVisualiser->clearTheScene();
    ui->vtkVisualiser->update();
    //update the treeview model
    //update the opensim library
    //form the tools reopen the models
    foreach (QString modelPath, vsOpenSimTools::tools->getReloadModelsPaths()) {
        OpenSim::Model  *newModel = new OpenSim::Model(modelPath.toStdString());
        navigatorModel->loadOpenSimModel(newModel);
        ui->navigatorTreeView->update(ui->navigatorTreeView->visibleRegion());
    }

}

void vsMainWindow::customMenuRequestedNavigator(const QPoint &point)
{
    qDebug() << "popup menu requested !";
    QModelIndex indexAtPos = ui->navigatorTreeView->indexAt(point);
    if(indexAtPos.isValid()){
        QMenu *nodeMenu = new QMenu(this);
        navigatorModel->getActionsForIndex(indexAtPos,nodeMenu);
        nodeMenu->popup(ui->navigatorTreeView->viewport()->mapToGlobal(point));
    }
}


void vsMainWindow::onExpendIndex(const QModelIndex modelIndex)
{
    ui->navigatorTreeView->expand(modelIndex);
}


void vsMainWindow::on_actionSave_Model_triggered()
{
    if(navigatorModel->getActiveModel() != nullptr){
        navigatorModel->getActiveModel()->print(navigatorModel->getActiveModel()->getInputFileName());
        vsOpenSimTools::tools->log("Current Model Saved In: "+QString::fromStdString(navigatorModel->
                                    getActiveModel()->getInputFileName()),"MainWindow",vsOpenSimTools::Info);
    }else{
        vsOpenSimTools::tools->log("There Is No Current Model","MainWindow",vsOpenSimTools::Error);
    }

}

void vsMainWindow::on_actionSave_Model_As_triggered()
{
    if(navigatorModel->getActiveModel() != nullptr){
        QString saveFileName = QFileDialog::
                getSaveFileName(this,"Save Current Model As",
                                QString::fromStdString(navigatorModel->getActiveModel()->getName())+".osim","*.osim");
        if(saveFileName != ""){
            qDebug() << "Save file Name: " << saveFileName;
            navigatorModel->getActiveModel()->print(saveFileName.toStdString());
            vsOpenSimTools::tools->log("Current Model Saved Here: "+saveFileName,"MainWindow",vsOpenSimTools::Success);
        }
        else{
            vsOpenSimTools::tools->log("No Valid File was Selected","MainWindow",vsOpenSimTools::Error);
        }
    }else{
        vsOpenSimTools::tools->log("There Is No Current Model","MainWindow",vsOpenSimTools::Error);
    }

}

void vsMainWindow::on_actionSave_All_triggered()
{
    foreach (OpenSim::Model *oneModel, navigatorModel->getOpenModels()) {
        vsOpenSimTools::tools->log("Saving "+QString::fromStdString(oneModel->getName()),"MainWindow",vsOpenSimTools::Info);
        oneModel->print(oneModel->getInputFileName());
    }
    vsOpenSimTools::tools->log("All Models are Saved","MainWindow",vsOpenSimTools::Success);

    vsOpenSimTools::tools->log("Saving The Scene...","MainWindow",vsOpenSimTools::Info);
    if(navigatorModel->getActiveModel() != nullptr){
        QString sceneFileName = QFileDialog::
                getSaveFileName(this,"Save Current Scene As","SceneName.vs","*.vs");
        if(sceneFileName != ""){
            vsOpenSimTools::tools->saveScene(sceneFileName);
            vsOpenSimTools::tools->log("Scene was Saved Here: "+sceneFileName,"MainWindow",vsOpenSimTools::Success);
        }
        else{
            vsOpenSimTools::tools->log("No Valid File was Selected","MainWindow",vsOpenSimTools::Error);
        }
    }else{
        vsOpenSimTools::tools->log("The Scene is empty","MainWindow",vsOpenSimTools::Info);
    }



}

void vsMainWindow::on_actionE_xit_triggered()
{
    if(QMessageBox::warning(this,"Exiting","Do you really want to exit ?",(QMessageBox::Yes|QMessageBox::No)) != QMessageBox::Yes)
    {
        qDebug() << "not Exiting";
    }else{
        qDebug() << "exiting";
        QApplication::exit();
    }

}

void vsMainWindow::on_actionClose_Model_triggered()
{
    navigatorModel->closeCurrentModel();
}

void vsMainWindow::on_actionClose_All_triggered()
{
    navigatorModel->closeAllModels();
}
