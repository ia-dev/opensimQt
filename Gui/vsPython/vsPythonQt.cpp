#include "vsPythonQt.h"

#include <QHBoxLayout>
#include <cstdarg>
#include <vsWidgets/vsSimulationToolsWidget.h>
#include <vsWidgets/vsMainWindow.h>
#include <QCoreApplication>
#include <vsPython/vsOsim.h>
#include <vsPython/vsPyGui.h>


vsPythonQt::vsPythonQt(QWidget *parent) : QWidget(parent)
{
    //store the main window pointer

    PythonQt::init(PythonQt::RedirectStdOut);
    PythonQt_QtAll::init();
    //qDebug() << "addtional python import path" << QCoreApplication::applicationDirPath() + "/../opensimPython";

    //PythonQt::self()->addSysPath("/Users/ritesh/projects/idhamari/VisSimKoblenz/opensim_install/sdk/Python/opensim");
    m_pyQtContext = PythonQt::self()->getMainModule();
    m_console = new vsPythonCustomConsole(this,m_pyQtContext);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_console);

    layout->setMargin(0);

    setLayout(layout);


    //connect history updated signal from the console
    connect(m_console,SIGNAL(historyUpdated(QString)),this,SLOT(getHistory(QString)));


    //load opensim module
    m_opensimModule = PythonQt::self()->importModule("opensim");
    m_console->runScript("import opensim as osim");

    //load the vsSimulation Module as vsSim
    vsOsim *osim = new vsOsim(this,this->getMainWindow()->getvsNavigator());
    m_pyQtContext.addObject("vsSim",osim);                                                  //! vsSim variable to access simulation api of vsOpenSimQt from Python Scripting Console

    //load the vsGui Module as vsGui
    vsPyGui *pygui = new vsPyGui(this);
    m_pyQtContext.addObject("vsGui",pygui);


}

void vsPythonQt::addApiForPython(QObject *receiver, const QString slot, const QString methodName)
{
    m_pyQtContext.addObject("vsOpenSim",receiver);
    PythonQt::self()->evalScript(m_pyQtContext, "def " + methodName
                                 + ":\n\t vsOpenSim."+ slot);

}

void vsPythonQt::runFile(const QString fileName)
{
    m_console->evalFile(fileName);
}

void vsPythonQt::getHistory(const QString history)
{
    emit this->historyUpdated(history);
}

vsMainWindow *vsPythonQt::getMainWindow()
{
    return static_cast<vsMainWindow*>(this->parent());
}
