#include "vsMainWindow.h"
#include "./ui_vsMainWindow.h"

#include <QLabel>
#include <OpenSim.h>
#include <QFileDialog>
#include <QDebug>
#include <QMimeData>
#include <QtWebEngineWidgets/qwebengineview.h>
#include "Visualizer/vsOpenGLVisualizer.h"

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

    //setting the navigator
    navigatorModel = new vsNavigatorModel();
    ui->navigatorTreeView->setModel(navigatorModel);

    //setting the visualizer
    ui->Visualizer->load(QUrl("http:/localhost:8002/threejs/editor/index.html"));
    //ui->Visualizer->setUrl(QUrl("http://www.facebook.com"));
    //ui->Visualizer->show();

    //setting the model preferences
    OpenSim::ModelVisualizer::addDirToGeometrySearchPaths("/home/ibr/myGitHub/opensimQt/WorkSpace/Geometry");

}

vsMainWindow::~vsMainWindow()
{
    delete ui;
}


void vsMainWindow::on_actionOpen_Model_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Model From File"),QString());
    qDebug() << fileName;
    OpenSim::Model  *newModel = new OpenSim::Model(fileName.toStdString());
    qDebug() << QString::fromStdString(newModel->getName());
    navigatorModel->loadOpenSimModel(newModel);
    //TODO save the state somewhere
    ui->navigatorTreeView->update(ui->navigatorTreeView->visibleRegion());
}

void vsMainWindow::dropEvent(QDropEvent *event)
{
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

//vtkSmartPointer<vtkRenderer> vsMainWindow::m_renderer = nullptr;
