#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QMainWindow>
#include "ui_WorkSpaceWindow.h"
#include "MainWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WorkSpaceWindowClass; };
QT_END_NAMESPACE

class WorkSpaceWindow : public QMainWindow
{
    Q_OBJECT

public:
    WorkSpaceWindow(QWidget *parent = nullptr);
    ~WorkSpaceWindow();

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_PushButton_3_DoubleClicked();

private:
    Ui::WorkSpaceWindowClass *ui;
	MainWindow mainWindow;
};
