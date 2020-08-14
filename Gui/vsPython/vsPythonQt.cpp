#include "vsPythonQt.h"

#include <QHBoxLayout>
#include <cstdarg>
#include <vsWidgets/vsSimulationToolsWidget.h>
#include <QCoreApplication>


vsPythonQt::vsPythonQt(QWidget *parent) : QWidget(parent)
{
    PythonQt::init(PythonQt::RedirectStdOut);
    PythonQt_QtAll::init();
    qDebug() << "addtional python import path" << QCoreApplication::applicationDirPath() + "/../opensimPython";
    //PythonQt::self()->addSysPath(QCoreApplication::applicationDirPath() + "/../opensimPython");
    PythonQt::self()->addSysPath("/Users/ritesh/projects/idhamari/VisSimKoblenz/opensim_install/sdk/Python/opensim");
    m_pyQtContext = PythonQt::self()->getMainModule();
    m_console = new vsPythonCustomConsole(this,m_pyQtContext);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_console);

    layout->setMargin(0);

    setLayout(layout);

    m_pyQtContext.addObject("openSim",this);
    connect(this,SIGNAL(openModel()),parent,SLOT(on_actionOpen_Model_triggered()));
    connect(this,SIGNAL(newModel()),parent,SLOT(on_actionNew_Model_triggered()));
    connect(this,SIGNAL(reload()),parent,SLOT(on_actionReload_triggered()));
    connect(this,SIGNAL(saveModel()),parent,SLOT(on_actionSave_Model_triggered()));
    connect(this,SIGNAL(saveModelAs()),parent,SLOT(on_actionSave_Model_As_triggered()));
    connect(this,SIGNAL(saveAll()),parent,SLOT(on_actionSave_All_triggered()));
    connect(this,SIGNAL(exit()),parent,SLOT(on_actionE_xit_triggered()));
    connect(this,SIGNAL(closeModel()),parent,SLOT(on_actionClose_Model_triggered()));
    connect(this,SIGNAL(closeAll()),parent,SLOT(on_actionClose_All_triggered()));
    connect(this,SIGNAL(loadMotion()),parent,SLOT(on_actionLoad_Motion_triggered()));

    //connect history updated signal from the console
    connect(m_console,SIGNAL(historyUpdated(QString)),this,SLOT(getHistory(QString)));

}

void vsPythonQt::addApiForPython(QObject *receiver, const QString slot, const QString methodName)
{
    m_pyQtContext.addObject("vsSim",receiver);
    PythonQt::self()->evalScript(m_pyQtContext, "def " + methodName
                                 + ":\n\t vsSim."+ slot);

}

void vsPythonQt::runFile(const QString fileName)
{
    m_console->evalFile(fileName);
}

void vsPythonQt::getHistory(const QString history)
{
    emit historyUpdated(history);
}
