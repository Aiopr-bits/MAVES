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
#include <QMessageBox.h>
#include <vtkOpenFOAMReader.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	//全屏
	this->setWindowState(Qt::WindowMaximized);
	ui->setupUi(this);

    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    render = vtkSmartPointer<vtkRenderer>::New();
    ui->openGLWidget->setRenderWindow(renderWindow);
    renderWindow->AddRenderer(render);

    render->SetBackground(1.0, 1.0, 1.0);
    render->SetBackground2(0.0, 0.8039, 1.0); 
    render->GradientBackgroundOn();

    addCoordinateAxes();

    // 连接信号和槽
	connect(ui->action1, &QAction::triggered, this, &MainWindow::handleAction1Triggered);			//信息框
	connect(ui->action2, &QAction::triggered, this, &MainWindow::handleAction2Triggered);			//x正向
	connect(ui->action3, &QAction::triggered, this, &MainWindow::handleAction3Triggered);			//x负向
	connect(ui->action4, &QAction::triggered, this, &MainWindow::handleAction4Triggered);			//y正向
	connect(ui->action5, &QAction::triggered, this, &MainWindow::handleAction5Triggered);			//y负向
	connect(ui->action6, &QAction::triggered, this, &MainWindow::handleAction6Triggered);			//z正向
	connect(ui->action7, &QAction::triggered, this, &MainWindow::handleAction7Triggered);			//z负向
	connect(ui->action8, &QAction::triggered, this, &MainWindow::handleAction8Triggered);			//适应窗口
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

    axesWidget->SetViewport(0.0, 0.0, 0.14, 0.2);
}

void MainWindow::handleAction2Triggered()
{
	vtkSmartPointer<vtkCamera> camera = render->GetActiveCamera();
	camera->SetPosition(1, 0, 0); // 设置相机位置到 x 方向正方向
	camera->SetFocalPoint(0, 0, 0); // 设置相机焦点到原点
	camera->SetViewUp(0, 0, 1); // 设置相机的上方向为 z 方向正方向
	render->ResetCamera();
	renderWindow->Render();
}

void MainWindow::handleAction3Triggered()
{
	vtkSmartPointer<vtkCamera> camera = render->GetActiveCamera();
	camera->SetPosition(-1, 0, 0);
	camera->SetFocalPoint(0, 0, 0);
	camera->SetViewUp(0, 0, 1);
	render->ResetCamera();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::handleAction4Triggered()
{
	vtkSmartPointer<vtkCamera> camera = render->GetActiveCamera();
	camera->SetPosition(0, 1, 0);
	camera->SetFocalPoint(0, 0, 0);
	camera->SetViewUp(0, 0, 1);
	render->ResetCamera();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::handleAction5Triggered()
{
	vtkSmartPointer<vtkCamera> camera = render->GetActiveCamera();
	camera->SetPosition(0, -1, 0);
	camera->SetFocalPoint(0, 0, 0);
	camera->SetViewUp(0, 0, 1);
	render->ResetCamera();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::handleAction6Triggered()
{
	vtkSmartPointer<vtkCamera> camera = render->GetActiveCamera();
	camera->SetPosition(0, 0, 1);
	camera->SetFocalPoint(0, 0, 0);
	camera->SetViewUp(0, 1, 0);
	render->ResetCamera();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::handleAction7Triggered()
{
	vtkSmartPointer<vtkCamera> camera = render->GetActiveCamera();
	camera->SetPosition(0, 0, -1);
	camera->SetFocalPoint(0, 0, 0);
	camera->SetViewUp(0, 1, 0);
	render->ResetCamera();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::handleAction8Triggered()
{
	vtkSmartPointer<vtkCamera> camera = render->GetActiveCamera();
	render->ResetCamera();
	camera->Zoom(1.5); // 调整缩放比例，可以根据需要调整
	ui->openGLWidget->renderWindow()->Render();
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

		render->RemoveAllViewProps();

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
		render->AddActor(actor);
		render->ResetCamera();
		ui->openGLWidget->renderWindow()->Render();
	}
}

void MainWindow::on_pushButton_4_clicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("打开文件"), "",
		tr("VTK 文件 (*.vtk);;"));
	QFileInfo fileInfo(filePath);
	if (fileInfo.exists())
	{
		QString type = fileInfo.suffix().toLower();
		render->RemoveAllViewProps();

		if (type == "vtk")
		{
			vtkSmartPointer<vtkUnstructuredGridReader> reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
			reader->SetFileName(filePath.toStdString().c_str());
			reader->Update();

			vtkSmartPointer<vtkDataSetMapper> surfaceMapper = vtkSmartPointer<vtkDataSetMapper>::New();
			surfaceMapper->SetInputConnection(reader->GetOutputPort());

			vtkSmartPointer<vtkActor> surfaceActor = vtkSmartPointer<vtkActor>::New();
			surfaceActor->SetMapper(surfaceMapper);

			vtkSmartPointer<vtkGeometryFilter> geometryFilter = vtkSmartPointer<vtkGeometryFilter>::New();
			geometryFilter->SetInputConnection(reader->GetOutputPort());
			geometryFilter->Update();

			vtkSmartPointer<vtkExtractEdges> extractEdges = vtkSmartPointer<vtkExtractEdges>::New();
			extractEdges->SetInputConnection(geometryFilter->GetOutputPort());
			extractEdges->Update();

			vtkSmartPointer<vtkPolyDataMapper> edgeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			edgeMapper->SetInputConnection(extractEdges->GetOutputPort());

			vtkSmartPointer<vtkActor> edgeActor = vtkSmartPointer<vtkActor>::New();
			edgeActor->SetMapper(edgeMapper);
			edgeActor->GetProperty()->SetColor(0, 0, 0);

			render->AddActor(surfaceActor);
			render->AddActor(edgeActor);

			render->ResetCamera();
			renderWindow->Render();
		}
		else
		{
			QMessageBox::warning(this, tr("错误"), tr("文件格式不支持"));
		}
	}
}


void MainWindow::handleAction1Triggered()
{
    ui->textBrowser->setVisible(!ui->textBrowser->isVisible());
}