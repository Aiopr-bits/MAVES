#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_WorkSpaceWindow.h"
#include "MainWindow.h"

#pragma execution_character_set("utf-8")

QT_BEGIN_NAMESPACE
namespace Ui { class WorkSpaceWindowClass; };
QT_END_NAMESPACE

class WorkSpaceWindow : public QMainWindow
{
    Q_OBJECT

public:
    WorkSpaceWindow(QWidget *parent = nullptr);
    ~WorkSpaceWindow();

private slots:
    void on_PushButton_3_DoubleClicked();

private:
    Ui::WorkSpaceWindowClass *ui;
	MainWindow mainWindow;
};
