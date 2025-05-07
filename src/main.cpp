/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#include "ThreeDimensionalComputation.h"
#include "CurvePlotting.h"
#include <QtWidgets/QApplication>
#include <cstdlib>
#include <string>

int main(int argc, char *argv[])
{
    const char* oldPath = getenv("PATH");
    if (oldPath == nullptr) oldPath = "";
    std::string newPath = "../solver/of2406/etc;../solver/of2406/bin;../extlib/Python313;";
    newPath += oldPath;
    _putenv(("PATH=" + newPath).c_str());
    _putenv("WM_PROJECT_DIR=../solver/of2406/");
    _putenv("MPI_BUFFER_SIZE=20000000");

    vtkObject::GlobalWarningDisplayOff();

    QApplication a(argc, argv);
    CurvePlotting w;
    w.show();
    return a.exec();
}