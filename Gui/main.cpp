#include "OpenSimWidgets/vsMainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    vsMainWindow w;
    w.showMaximized();
    return a.exec();
}
