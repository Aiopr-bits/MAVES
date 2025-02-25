#include "MainWindow.h"
#include "WorkSpaceWindow.h"
#include <QtWidgets/QApplication>
#include <cstdlib>
#include <string>

int main(int argc, char *argv[])
{
    const char* oldPath = getenv("PATH");
    if (oldPath == nullptr) oldPath = "";
    std::string newPath = "../solver/of2406/etc;../solver/of2406/bin;";
    newPath += oldPath;
    _putenv(("PATH=" + newPath).c_str());
    _putenv("WM_PROJECT_DIR=../solver/of2406/");
    _putenv("MPI_BUFFER_SIZE=20000000");

    QApplication a(argc, argv);
    WorkSpaceWindow w;
    w.show();
    return a.exec();
}