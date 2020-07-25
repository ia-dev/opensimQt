#include "vsWidgets/vsMainWindow.h"

#include <QApplication>

#include <vsTools/vsOpenSimTools.h>


/**
 * program entry function.
 * setup the Qt configurations and run the event loop
 * @param argc number of inputs
 * @param input values array
*/
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
