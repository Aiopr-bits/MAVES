#include "MainWindow.h"
#include "WorkSpaceWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    _putenv("PATH=../solver/of2406/etc;../solver/of2406/bin;%PATH%");
    _putenv("WM_PROJECT_DIR=../solver/of2406/");
    _putenv("MPI_BUFFER_SIZE=20000000");

    QApplication a(argc, argv);
    WorkSpaceWindow w;
    w.show();
    return a.exec();
}