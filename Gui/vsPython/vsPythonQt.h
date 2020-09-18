#ifndef VSPYTHONQT_H
#define VSPYTHONQT_H

#include "PythonQt.h"
#include "PythonQt_QtAll.h"
#include "vsManager.h"
#include "vsPythonCustomConsole.h"
#include <QWidget>

class vsMainWindow;

class vsPythonQt : public QWidget
{
    Q_OBJECT
public:
    explicit vsPythonQt(QWidget *parent = nullptr);

    void addApiForPython(QObject *receiver, const QString slot, const QString methodName);
    PythonQtObjectPtr getOpenSimModule() const {return m_opensimModule;}

    //! Run a python script from a file
    void runFile(const QString fileName);
    // return the instance of a vsManager
    vsManager* getManager() const {return this->manager;}

signals:
    //! Signal: Notifies the updated hisotory on python console and provides last statement executed as QString
    //! \code void historyUpdated(const QString history) \code
    //! \param history : QString \param
    //! \returns void \returns the output is stored in the history parameter. Output is last statement executed on console.
    void historyUpdated(const QString history);

private slots:
    //! Slot: Handler for history updated signal from the python console
    //! emits a signal \code historyUpdated(const QString history) \code
    //! which can be used to get updates about history of statements executed on the console.
    void getHistory(const QString history);

private:
    vsMainWindow* getMainWindow();
private:
    PythonQtObjectPtr m_pyQtContext;
    vsPythonCustomConsole *m_console;
    PythonQtObjectPtr m_opensimModule;
    vsManager* manager;

};

#endif // VSPYTHONQT_H
