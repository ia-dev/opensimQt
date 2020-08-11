#ifndef VSPYTHONQT_H
#define VSPYTHONQT_H

#include "PythonQt.h"
#include "PythonQt_QtAll.h"
#include "gui/PythonQtScriptingConsole.h"
#include <QWidget>

class vsPythonQt : public QWidget
{
    Q_OBJECT
public:
    explicit vsPythonQt(QWidget *parent = nullptr);

    void addApiForPython(QObject *receiver, const QString slot, const QString methodName);

signals:
    void openModel();
    void newModel();
    void reload();
    void saveModel();
    void saveModelAs();
    void saveAll();
    void exit();
    void closeModel();
    void closeAll();
    void loadMotion();
    void poses();

private:
    PythonQtObjectPtr m_pyQtContext;
    PythonQtScriptingConsole *m_console;

};

#endif // VSPYTHONQT_H
