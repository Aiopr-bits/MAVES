#include "WorkSpaceWindow.h"

WorkSpaceWindow::WorkSpaceWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WorkSpaceWindowClass())
{
    ui->setupUi(this);
}

WorkSpaceWindow::~WorkSpaceWindow()
{
    delete ui;
}
