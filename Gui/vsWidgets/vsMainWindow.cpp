#include "vsMainWindow.h"
#include "./ui_vsMainWindow.h"
#include "vsPluginActivationDialog.h"

#include <QLabel>
#include <OpenSim.h>
#include <QFileDialog>
#include <QDebug>
#include <QMimeData>
#include <QMessageBox>
#include <QDesktopServices>
#include <QtWebEngineWidgets/qwebengineview.h>
#include <vsTools/vsMotionsUtils.h>
#include <vsTools/vsOpenSimTools.h>
#include "vsVisualizer/vsOpenGLVisualizer.h"


vsMainWindow::vsMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::vsMainWindow),coordinatesWidget(new vsCoordinatesWidget(this))
{
    ui->setupUi(this);

    setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);

    tabifyDockWidget(ui->scriptingDock,ui->messagesDock);
    tabifyDockWidget(ui->navigatorDock,ui->coordinatesDock);
    ui->navigatorDock->raise();
    //resizing automatically depennding on the selected dock widget(coordinates or navigator)
    connect(ui->coordinatesDock,&QDockWidget::visibilityChanged,[this](bool a){
        if(a){
            resizeDocks(QList<QDockWidget*>()<<ui->coordinatesDock<<ui->navigatorDock,QList<int>()<<500<<500,
                        Qt::Orientation::Horizontal);
        }else{
            resizeDocks(QList<QDockWidget*>()<<ui->coordinatesDock<<ui->navigatorDock,QList<int>()<<290<<290,
                        Qt::Orientation::Horizontal);
        }
    });

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
    connect(navigatorModel,&vsNavigatorModel::cleanCurrentModelProcesses,this,&vsMainWindow::onCleanCurrentModelProcesses);


    vsOpenSimTools::tools->setNavigatorModel(navigatorModel);

    //setting the properties
    propertiesModel = new vsPropertyModel(this);
    ui->propertyTreeView->setModel(propertiesModel);

    //setting the visualizer
    //ui->Visualizer->load(QUrl("http:/localhost:8002/threejs/editor/index.html"));
    //ui->Visualizer->setUrl(QUrl("http://www.facebook.com"));
    //ui->Visualizer->show();

    //setting the model preferences
    OpenSim::ModelVisualizer::addDirToGeometrySearchPaths("./vsWorkSpace/opensim-models/Geometry");
	OpenSim::ModelVisualizer::addDirToGeometrySearchPaths("../vsWorkSpace/Geometry");                       //linux build
	OpenSim::ModelVisualizer::addDirToGeometrySearchPaths("../../../opensim-gui/opensim-models/Geometry");    //Windows build


    //setting the logging
    connect(vsOpenSimTools::tools,&vsOpenSimTools::messageLogged,ui->messagesTextEdit,&QTextEdit::append);
    connect(vsOpenSimTools::tools,&vsOpenSimTools::messageLoggedPlain,ui->messagesTextEdit,&QTextEdit::insertPlainText);
    vsOpenSimTools::tools->log("Log display connected","",vsOpenSimTools::Success,true);

    //setting the update of the coordintaes dock
    ui->scrollArea->setWidget(coordinatesWidget);
    connect(vsOpenSimTools::tools,&vsOpenSimTools::currentModelUpdated,this,&vsMainWindow::onCurrentModelUpdated);


    //setting up the context menu
    ui->navigatorTreeView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(ui->navigatorTreeView,&QTreeView::customContextMenuRequested,this,&vsMainWindow::customMenuRequestedNavigator);
    connect(ui->navigatorTreeView,&QTreeView::clicked,this,&vsMainWindow::onNavigatorClicked);

    //connecting dock resize events to setup the vtk widget

    //connecting actor selection in the visualizer
    connect(ui->vtkVisualiser,&vsVisualizerVTK::objectSelectedInNavigator,this,&vsMainWindow::onSelectedObjectActor);

    //the sumulation configs
    connect(vsMotionsUtils::getInstance(),&vsMotionsUtils::currentMotionChanged,simulationWidget,&vsSimulationToolsWidget::onCurrentMotionChanged);

    //plugins
    vsOpenSimTools::tools->loadOnEntryPlugins();
    listUserPlugins();


}

void vsMainWindow::listUserPlugins()
{
    ui->menuuser_plugins->clear();
    //TODO clean the removed actions
    QDir pDir(QApplication::applicationDirPath());
    if(!pDir.exists(pDir.path()+"/plugins"))pDir.mkdir("plugins");
    pDir.cd("plugins");
    pDir.setNameFilters(QStringList() << vsOpenSimTools::getPluginExtentionForOS());
    foreach (auto entryInfo, pDir.entryInfoList(QDir::Files)) {
        QString fileName = vsOpenSimTools::getOSName()=="windows"?entryInfo.fileName():entryInfo.baseName();
        QAction *pluginAction = new QAction(fileName,this);
        ui->menuuser_plugins->addAction(pluginAction);
        connect(pluginAction,&QAction::triggered,[this,fileName,pDir](){
            qDebug() << "pluging is loading " << fileName;
            OpenSim::LoadOpenSimLibrary((pDir.path()+"/"+fileName).toStdString());
//            bool loaded = OpenSim::LoadOpenSimLibraryExact(entryInfo.filePath().toStdString());
//            if(!loaded){
//                vsOpenSimTools::tools->log("could not load library","",vsOpenSimTools::Error);
//            }
            vsPluginActivationDialog dlg(fileName);
            dlg.exec();
        });
    }

}

vsMainWindow::~vsMainWindow()
{
    delete ui;
}

void vsMainWindow::onCurrentModelUpdated()
{
    //update the coordinates tab
    foreach (auto deleg, currentCoordinatesDelegates) {
        //ui->coordinatesLayout->removeWidget(deleg);
        //ui->scrollAreaLayout->removeWidget(deleg);
        coordinatesWidget->removeCoordinateDelegate(deleg);
        currentCoordinatesDelegates.removeOne(deleg);
        deleg->deleteLater();
    }
    auto currentModel = vsOpenSimTools::tools->getNavigatorModel()->getActiveModel();
    if(!currentModel) return;
    qDebug() << "creating coordinates delegates";
//    currentModel->updCoordinateSet().so
    ui->modelLabel->setText("Model : "+QString::fromStdString(currentModel->getName()));

    //OpenSim::Array<std::string> coordinateNames;
    //currentModel->updCoordinateSet().getNames(coordinateNames)
    for (int i = 0; i < currentModel->getNumCoordinates(); ++i) {

        //TODO the function get return a variable that is not complete and need to be investigated.
        //for now only the index will be supplimented

        //OpenSim::Coordinate coordinate = currentModel->updCoordinateSet().get;

        //qDebug() << "coordinate name: " << QString::fromStdString(coordinate.getName()) << "value: " << coordinate.getValue(currentModel->updWorkingState());
         //qDebug() << "coordinate name: " << QString::fromStdString(currentModel->getCoordinateSet().get(i).getName()) << "value: " << currentModel->getCoordinateSet().get(i).getValue(currentModel->updWorkingState());
        auto coordinateDelegate =  new vsCoordinateDelegate(i,currentModel,this);
        //ui->coordinatesLayout->addWidget(coordinateDelegate);
        //ui->scrollAreaLayout->addWidget(coordinateDelegate);
        coordinatesWidget->addCoordinateDelegate(coordinateDelegate);
        currentCoordinatesDelegates.append(coordinateDelegate);
    }
    coordinatesWidget->initializeWidgetForNewModel();
    //using the group solution
}

void vsMainWindow::onCleanCurrentModelProcesses()
{
    qDebug() << "cleaning the processecies(simulation...) for the current model";
    simulationWidget->cleanSimulationWidget();

}

void vsMainWindow::userPluginClicked(QString pluginFileName)
{
//implemented in lambda

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
        QString historyPath =QApplication::applicationDirPath()
                +"/History/"+QDateTime::currentDateTime().toString(Qt::ISODate).replace(":","_");
        QDir hDir;
        if(hDir.exists(QApplication::applicationDirPath()+"/History/"))hDir.mkdir(QApplication::applicationDirPath()+"/History/");
        vsOpenSimTools::tools->saveScene(historyPath+".vs");
        qDebug() << "exiting" << historyPath;
        QFile::copy(QApplication::applicationDirPath()+"/OpenSimQtLog_.txt",historyPath+".txt");
        event->accept();
    }
}

void vsMainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    ui->vtkVisualiser->updateVtkButtons();
}

void vsMainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    ui->vtkVisualiser->updateVtkButtons();
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
    onCleanCurrentModelProcesses();
    //TODO clean the simulation bar
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

void vsMainWindow::onNavigatorClicked(const QModelIndex modelIndex)
{
    vsNavigatorNode *selectedNode = navigatorModel->nodeForIndex(modelIndex);
    propertiesModel->setSelectedNavigarorNode(selectedNode);
    //ui->propertyTreeView->update(ui->propertyTreeView->rect());
    ui->propertyTreeView->expandAll();
    selectedNode->selectVisualizerActors();
}


void vsMainWindow::onExpendIndex(const QModelIndex modelIndex)
{
    ui->navigatorTreeView->expand(modelIndex);
}

void vsMainWindow::onSelectedObjectActor(OpenSim::Object *object)
{
    if(object == nullptr){
        //navigatorModel->selectObject(nullptr);
        ui->navigatorTreeView->setCurrentIndex(QModelIndex());
        propertiesModel->setSelectedNavigarorNode(nullptr);
        //TODO update the properties
        return ;
    }
    QModelIndex selectedIndex = navigatorModel->selectObject(object);
    if(!selectedIndex.isValid()) return;
    ui->navigatorTreeView->setCurrentIndex(selectedIndex);
    ui->navigatorTreeView->scrollTo(selectedIndex);
    propertiesModel->setSelectedNavigarorNode(navigatorModel->nodeForIndex(selectedIndex));
    ui->propertyTreeView->expandAll();
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
        QString historyPath =QApplication::applicationDirPath()
                +"/History/"+QDateTime::currentDateTime().toString(Qt::ISODate).replace(":","_");
        QDir hDir;
        if(hDir.exists(QApplication::applicationDirPath()+"/History/"))hDir.mkdir(QApplication::applicationDirPath()+"/History/");
        vsOpenSimTools::tools->saveScene(historyPath+".vs");
        qDebug() << "exiting" << historyPath;
        QFile::copy(QApplication::applicationDirPath()+"/OpenSimQtLog_.txt",historyPath+".txt");
        //move the log to the HistoryFolder
        //save the scene
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

void vsMainWindow::on_actionLoad_Motion_triggered()
{
    //calling the action for LoadMotion for the active model node
    if(navigatorModel->getActiveModel()){
        vsMotionsUtils::getInstance()->openLoadMotionDialog(navigatorModel->getActiveModel());
    }
    else return vsOpenSimTools::tools->log("No Active Model is present","vsMainWindow");

}

void vsMainWindow::on_posesButton_clicked()
{
    auto posesMenu = coordinatesWidget->getPosesMenu();
    //posesMenu->setParent();
    posesMenu->move(ui->coordinatesDock->mapToGlobal(ui->posesButton->pos())+QPoint(0,30));
    posesMenu->show();
}

void vsMainWindow::on_actionCurrent_model_Externally_triggered()
{

    if(navigatorModel->getActiveModel() == nullptr){
        vsOpenSimTools::tools->log("can not open model externally , current model in null","",vsOpenSimTools::Error);
        return;
    }
    auto ret = QDesktopServices::openUrl(QString::fromStdString(navigatorModel->getActiveModel()->getInputFileName()));
    vsOpenSimTools::tools->log((ret?"model opened externaly":"no default program is selected from type"));


//    qDebug() <<"edit current model in a text editor...";
//    if(navigatorModel->getActiveModel()){
//      //QString modelFilename =  navigatorModel->getActiveModel()->getInputFileName();
//      // open the file in system text editor
//      try{
//          //QProcess *vsProcess = new QProcess(this);
//        qDebug() << "productType():" << QSysInfo::productType();
////        if (QSysInfo::productType() =="Windows")
////            vsProcess::start(" Write " + modelFilename );
////        else if (QSysInfo::productType() =="Linux")
////            vsProcess::startc(" gedit " + modelFilename );
////        else if (QSysInfo::productType() =="Windows")
////            vsProcess::start(" open " + modelFilename );
////        else
////            qDebug() <<"un supported operating system ....";
//      } catch (...) {
//          vsOpenSimTools::tools->log("Can not open the current model in external editor ...","",vsOpenSimTools::Error,true);
//      }
//    }//if

}

void vsMainWindow::on_actionNew_Model_triggered()
{
  //copy the file from template folders to the build folder newmodel.osim

  //load to the new model
  try {
      qDebug() <<"create a new model from template ...";

      QString newModelFileName = QApplication::applicationDirPath()+"/templates/template.osim" ;
      OpenSim::Model  *newModel = new OpenSim::Model(newModelFileName.toStdString());
      qDebug() << QString::fromStdString(newModel->getName());
      navigatorModel->loadOpenSimModel(newModel);
      //TODO save the state somewhere
      ui->navigatorTreeView->update(ui->navigatorTreeView->visibleRegion());
//      //update the openModelsFile
     vsOpenSimTools::tools->addToOpenModels(newModel);
  }catch (...) {
      vsOpenSimTools::tools->log("No valid OpenSim model was created","",vsOpenSimTools::Error,true);
  }

}

void vsMainWindow::on_actionimport_new_plugin_triggered()
{

    QUrl pluginFileURL = QFileDialog::getOpenFileUrl(nullptr,"plugin library file",
                                                     QApplication::applicationDirPath(),
                                                     vsOpenSimTools::getPluginExtentionForOS());
    if(!pluginFileURL.isValid()){
        vsOpenSimTools::tools->log("no plugin was selected !","MainWindow",vsOpenSimTools::Error);
        return;
    }
    //test if the plugin dir exist
    QDir hDir(QApplication::applicationDirPath());
    if(!hDir.exists(hDir.path()+"/plugins"))hDir.mkdir("plugins");
    hDir.cd("plugins");
    QFile::copy(pluginFileURL.toLocalFile(),hDir.path()+"/"+pluginFileURL.fileName());
    //TODO update the menue for user plugins
    vsOpenSimTools::tools->log("a new library was listed from "+pluginFileURL.toLocalFile()+" to "+hDir.path()+"/"+pluginFileURL.fileName(),"MainWindow",vsOpenSimTools::Success);
    listUserPlugins();
}
