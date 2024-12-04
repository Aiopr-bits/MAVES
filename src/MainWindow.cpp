#include "MainWindow.h"
#include <vtkSTLReader.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkCamera.h>
#include <vtkLight.h>
#include <vtkPropCollection.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>
#include <iostream>
#include <vtkOBJReader.h>
#include <vtkPLYReader.h>
#include <vtkXMLPolyDataReader.h>
#include <QTimer>
#include <qdebug.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);

    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();
    ui->openGLWidget->setRenderWindow(renderWindow);
    renderWindow->AddRenderer(renderer);

    renderer->SetBackground(1.0, 1.0, 1.0);
    renderer->SetBackground2(0.0, 0.8039, 1.0); 
    renderer->GradientBackgroundOn();

    addCoordinateAxes();

    // 连接信号和槽
    connect(ui->action1, &QAction::triggered, this, &MainWindow::handleAction1Triggered);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::addCoordinateAxes()
{
    vtkSmartPointer<vtkAxesActor> actor_axes = vtkSmartPointer<vtkAxesActor>::New();

    axesWidget->SetOrientationMarker(actor_axes);
    axesWidget->SetInteractor(ui->openGLWidget->renderWindow()->GetInteractor());
    axesWidget->SetEnabled(true);
    axesWidget->InteractiveOn();
    axesWidget->SetInteractive(false);

    axesWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
}

void MainWindow::handleAction2Triggered()
{
}

void MainWindow::handleAction3Triggered()
{
}

void MainWindow::handleAction4Triggered()
{
}

void MainWindow::handleAction5Triggered()
{
}

void MainWindow::handleAction6Triggered()
{
}

void MainWindow::handleAction7Triggered()
{
}

void MainWindow::handleAction8Triggered()
{

}

void MainWindow::handleAction1Triggered()
{
    ui->textBrowser->setVisible(!ui->textBrowser->isVisible());
}