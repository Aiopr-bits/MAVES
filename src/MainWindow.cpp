#pragma once
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
#include <QVTKOpenGLNativeWidget.h>
#include <vtkActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkTextProperty.h>
#include <vtkWorldPointPicker.h>
#include <IGESControl_Reader.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <QBoxLayout>
#include <QDockWidget>
#include <QLineEdit>
#include <QMenuBar>
#include <QTextBrowser>
#include <QToolBar>
#include <QTreeWidget>
#include <STEPControl_Reader.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <vtkSmartPointer.h>
#include <QMetaType>
#include <QMainWindow>

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

void MainWindow::on_pushButton_clicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("打开文件"), "",
		tr("所有类型 (*.stp *.step *.igs *.iges *.brep);;"
			"STP 文件 (*.stp *.step);;"
			"IGS 文件 (*.igs *.iges);;"
			"BREP 文件 (*.brep)"));
	QFileInfo fileInfo(filePath);

	if (fileInfo.exists())
	{
		QString type = fileInfo.suffix().toLower();
		vtkNew<vtkPolyDataMapper> mapper;

		renderer->RemoveAllViewProps();

		if (type == "step" || type == "stp")
		{
			STEPControl_Reader Reader;
			Reader.ReadFile(filePath.toStdString().c_str());
			Reader.TransferRoots();
			IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(Reader.Shape());
			vtkNew<IVtkTools_ShapeDataSource> DS;
			DS->SetShape(aShapeImpl);
			DS->Update();
			vtkNew<IVtkTools_DisplayModeFilter> filter;
			filter->SetInputData(DS->GetOutput());
			filter->SetDisplayMode(DM_Shading);
			filter->SetSmoothShading(1);
			filter->Update();
			mapper->SetInputData(filter->GetOutput());
		}
		else if (type == "iges" || type == "igs")
		{
			IGESControl_Reader Reader;
			Reader.ReadFile(filePath.toStdString().c_str());
			Reader.TransferRoots();
			IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(Reader.Shape());
			vtkNew<IVtkTools_ShapeDataSource> DS;
			DS->SetShape(aShapeImpl);
			DS->Update();
			vtkNew<IVtkTools_DisplayModeFilter> filter;
			filter->SetInputData(DS->GetOutput());
			filter->SetDisplayMode(DM_Shading);
			filter->SetSmoothShading(1);
			filter->Update();
			mapper->SetInputData(filter->GetOutput());
		}
		else if (type == "brep")
		{
			BRep_Builder builder;
			TopoDS_Shape shape;
			BRepTools::Read(shape, filePath.toStdString().c_str(), builder);
			IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(shape);
			vtkNew<IVtkTools_ShapeDataSource> DS;
			DS->SetShape(aShapeImpl);
			DS->Update();
			vtkNew<IVtkTools_DisplayModeFilter> filter;
			filter->SetInputData(DS->GetOutput());
			filter->SetDisplayMode(DM_Shading);
			filter->SetSmoothShading(1);
			filter->Update();
			mapper->SetInputData(filter->GetOutput());
		}
		else
		{
			QMessageBox::warning(this, tr("错误"), tr("文件格式不支持"));
		}

		vtkNew<vtkActor> actor;
		actor->SetMapper(mapper);
		renderer->AddActor(actor);
		renderer->ResetCamera();
		ui->openGLWidget->renderWindow()->Render();
	}
}


void MainWindow::handleAction1Triggered()
{
    ui->textBrowser->setVisible(!ui->textBrowser->isVisible());
}