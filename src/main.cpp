#include "WorkSpaceWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WorkSpaceWindow w;
    w.show();
    return a.exec();
}
