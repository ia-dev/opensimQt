#include "vsWidgets/vsMainWindow.h"

#include <QApplication>

#include <vsTools/vsOpenSimTools.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("OpenSimQt");
    QCoreApplication::setOrganizationName("VisSim");
    vsOpenSimTools::tools->openSimSettings->setValue("applicationName",QCoreApplication::applicationName());
    vsOpenSimTools::tools->openSimSettings->sync();

    vsMainWindow w;
    w.showMaximized();
    return a.exec();
}
