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
#include <QString>
#include <vtkColorTransferFunction.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
	, playTimer(new QTimer(this))
	, reverseTimer(new QTimer(this))
	, loopPlayTimer(new QTimer(this))
	, lastClickedButton(nullptr)
{
	//全屏
	this->setWindowState(Qt::WindowMaximized);
	ui->setupUi(this);

	//初始化三维窗口
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    render = vtkSmartPointer<vtkRenderer>::New();
    ui->openGLWidget->setRenderWindow(renderWindow);
    renderWindow->AddRenderer(render);
    render->SetBackground(1.0, 1.0, 1.0);
    render->SetBackground2(0.0, 0.8039, 1.0); 
    render->GradientBackgroundOn();
    addCoordinateAxes();

	// 初始化按钮
	QPushButton* buttons[] = {
		ui->pushButton, ui->pushButton_2, ui->pushButton_3, ui->pushButton_4, ui->pushButton_5,
		ui->pushButton_6, ui->pushButton_7, ui->pushButton_8, ui->pushButton_9, ui->pushButton_10,
		ui->pushButton_11, ui->pushButton_12, ui->pushButton_13, ui->pushButton_14, ui->pushButton_15,
		ui->pushButton_16, ui->pushButton_17, ui->pushButton_18, ui->pushButton_19, ui->pushButton_20
	};
	for (QPushButton* button : buttons) {
		connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
	}

	//创建各个子面板
	formMesh = new FormMesh(this);
	formPostprocessing = new FormPostprocessing(this);
	ui->gridLayout_3->addWidget(formMesh, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formPostprocessing, 0, 0, 1, 1);
	formMesh->hide();
	formPostprocessing->hide();

    // 连接信号和槽
	connect(ui->action1, &QAction::triggered, this, &MainWindow::handleAction1Triggered);			//信息框
	connect(ui->action2, &QAction::triggered, this, &MainWindow::handleAction2Triggered);			//x正向
	connect(ui->action3, &QAction::triggered, this, &MainWindow::handleAction3Triggered);			//x负向
	connect(ui->action4, &QAction::triggered, this, &MainWindow::handleAction4Triggered);			//y正向
	connect(ui->action5, &QAction::triggered, this, &MainWindow::handleAction5Triggered);			//y负向
	connect(ui->action6, &QAction::triggered, this, &MainWindow::handleAction6Triggered);			//z正向
	connect(ui->action7, &QAction::triggered, this, &MainWindow::handleAction7Triggered);			//z负向
	connect(ui->action8, &QAction::triggered, this, &MainWindow::handleAction8Triggered);			//适应窗口
	connect(playTimer, &QTimer::timeout, this, &MainWindow::onPlayTimerTimeout);					//播放
	connect(reverseTimer, &QTimer::timeout, this, &MainWindow::onReverseTimerTimeout);				//倒放
	connect(loopPlayTimer, &QTimer::timeout, this, &MainWindow::onLoopPlayTimerTimeout);			//循环播放

	connect(formMesh, &FormMesh::meshVisibilityChanged, this, &MainWindow::formMesh_apply);									//更新渲染窗口
	connect(formPostprocessing, &FormPostprocessing::resultDataLoaded, this, &MainWindow::formPostprocessing_loadData);		//渲染结果数据
	connect(formPostprocessing, &FormPostprocessing::apply, this, &MainWindow::formPostprocessing_apply);					//更新渲染窗口
	connect(formPostprocessing, &FormPostprocessing::firstFrame, this, &MainWindow::formPostprocessing_firstFrame);			//第一帧
	connect(formPostprocessing, &FormPostprocessing::previousFrame, this, &MainWindow::formPostprocessing_previousFrame);	//上一帧
	connect(formPostprocessing, &FormPostprocessing::reverse, this, &MainWindow::formPostprocessing_reverse);				//重新播放
	connect(formPostprocessing, &FormPostprocessing::play, this, &MainWindow::formPostprocessing_play);						//播放
	connect(formPostprocessing, &FormPostprocessing::nextFrame, this, &MainWindow::formPostprocessing_nextFrame);			//下一帧
	connect(formPostprocessing, &FormPostprocessing::lastFrame, this, &MainWindow::formPostprocessing_lastFrame);			//最后一帧
	connect(formPostprocessing, &FormPostprocessing::loopPlay, this, &MainWindow::formPostprocessing_loopPlay);				//循环播放
	connect(formPostprocessing, &FormPostprocessing::playPause, this, &MainWindow::formPostprocessing_playPause);			//播放暂停
	connect(formPostprocessing, &FormPostprocessing::reversePause, this, &MainWindow::formPostprocessing_reversePause);		//反向播放暂停
	connect(formPostprocessing, &FormPostprocessing::loopPlayPause, this, &MainWindow::formPostprocessing_loopPlayPause);	//循环播放暂停
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

void MainWindow::hideAllSubForm()
{
	formMesh->hide();
	formPostprocessing->hide();
}

void MainWindow::handleAction2Triggered()
{
	vtkSmartPointer<vtkCamera> camera = render->GetActiveCamera();
	camera->SetPosition(1, 0, 0); 
	camera->SetFocalPoint(0, 0, 0); 
	camera->SetViewUp(0, 0, 1);
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
	camera->Zoom(1); 
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
			return;
		}

		vtkNew<vtkActor> actor;
		actor->SetMapper(mapper);
		actor->GetProperty()->SetColor(97.0 / 255.0, 111.0 / 255.0, 125.0 / 255.0); 
		render->AddActor(actor);
		render->ResetCamera();
		ui->openGLWidget->renderWindow()->Render();
	}
}


void MainWindow::on_pushButton_4_clicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("打开文件"), "",
		tr(/*"所有类型 (*.vtk *.foam);;"
			"VTK 文件 (*.vtk);;"*/
			"OpenFOAM 文件 (*.foam)"));
	QFileInfo fileInfo(filePath);
	if (fileInfo.exists())
	{
		QString type = fileInfo.suffix().toLower();
		render->RemoveAllViewProps();

		if (type == "foam")
		{
			QString casePath = fileInfo.path();
			std::string command = "foamToVTK -time 0 -case " + casePath.toStdString();
			std::system(command.c_str());

			QString folderName = casePath.split("/").last();
			QString vtpPath = casePath + "/VTK/" + folderName + "_0/boundary/";

			//获取vtpPath文件夹下的所有.vtp文件路径
			QDir dir(vtpPath);
			QStringList vtpFiles = dir.entryList(QStringList() << "*.vtp", QDir::Files);

			foreach(QString vtpFile, vtpFiles)
			{
				QString fullPath = dir.absoluteFilePath(vtpFile);
				vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
				reader->SetFileName(fullPath.toStdString().c_str());
				reader->Update();

				vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
				mapper->SetInputConnection(reader->GetOutputPort());

				vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
				actor->SetMapper(mapper);
				actor->GetProperty()->SetColor(0,221,221);

				render->AddActor(actor);

				vtkSmartPointer<vtkExtractEdges> extractEdges = vtkSmartPointer<vtkExtractEdges>::New();
				extractEdges->SetInputConnection(reader->GetOutputPort());
				extractEdges->Update();

				vtkSmartPointer<vtkPolyDataMapper> edgeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
				edgeMapper->SetInputConnection(extractEdges->GetOutputPort());

				vtkSmartPointer<vtkActor> edgeActor = vtkSmartPointer<vtkActor>::New();
				edgeActor->SetMapper(edgeMapper);
				edgeActor->GetProperty()->SetColor(0, 0, 0);

				render->AddActor(edgeActor);

				QString baseName = vtpFile.left(vtpFile.lastIndexOf('.'));
				GlobalData::getInstance().getCaseData()->meshFaceActors.insert(std::make_pair(baseName, actor));
				GlobalData::getInstance().getCaseData()->meshEdgeActors.insert(std::make_pair(baseName, edgeActor));
			}
			GlobalData::getInstance().getCaseData()->meshPath = fileInfo.path().toStdString();

			formMesh->updateForm();

			render->ResetCamera();
			renderWindow->Render();
		}
		else
		{
			QMessageBox::warning(this, tr("错误"), tr("文件格式不支持"));
		}
	}
}

void MainWindow::on_pushButton_2_clicked()
{
	hideAllSubForm();
	formMesh->show();
}

void MainWindow::on_pushButton_17_clicked()
{
	hideAllSubForm();
	formPostprocessing->show();
}

void MainWindow::formMesh_apply()
{
	renderWindow->Render();
}

std::tuple<vtkSmartPointer<vtkActor>, vtkSmartPointer<vtkColorTransferFunction>, std::array<double, 2>> createActorFromFile(const QString& filePath, const QString& variableName)
{
	vtkSmartPointer<vtkAlgorithm> reader;

	// 根据文件扩展名选择合适的读取器
	if (filePath.endsWith(".vtk", Qt::CaseInsensitive)) {
		reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
	}
	else if (filePath.endsWith(".vtu", Qt::CaseInsensitive)) {
		reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
	}
	else if (filePath.endsWith(".vtp", Qt::CaseInsensitive)) {
		reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
	}
	else {
		return std::make_tuple(nullptr, nullptr, std::array<double, 2>{0.0, 1.0});
	}

	// 设置文件名并更新读取器
	if (auto unstructuredGridReader = vtkUnstructuredGridReader::SafeDownCast(reader)) {
		unstructuredGridReader->SetFileName(filePath.toStdString().c_str());
	}
	else if (auto xmlUnstructuredGridReader = vtkXMLUnstructuredGridReader::SafeDownCast(reader)) {
		xmlUnstructuredGridReader->SetFileName(filePath.toStdString().c_str());
	}
	else if (auto xmlPolyDataReader = vtkXMLPolyDataReader::SafeDownCast(reader)) {
		xmlPolyDataReader->SetFileName(filePath.toStdString().c_str());
	}
	reader->Update();

	// 获取数据集
	vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid = vtkUnstructuredGrid::SafeDownCast(reader->GetOutputDataObject(0));
	vtkSmartPointer<vtkPolyData> polyData = vtkPolyData::SafeDownCast(reader->GetOutputDataObject(0));

	vtkSmartPointer<vtkDataSet> dataSet;
	if (unstructuredGrid) {
		dataSet = unstructuredGrid;
	}
	else if (polyData) {
		dataSet = polyData;
	}
	else {
		return std::make_tuple(nullptr, nullptr, std::array<double, 2>{0.0, 1.0});
	}

	// 检查是否包含指定的物理量
	if (!dataSet->GetPointData()->HasArray(variableName.toStdString().c_str())) {
		return std::make_tuple(nullptr, nullptr, std::array<double, 2>{0.0, 1.0});
	}

	// 创建颜色传输函数
	vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	colorTransferFunction->SetColorSpaceToRGB();

	// 设置颜色映射范围
	double range[2];
	dataSet->GetPointData()->GetArray(variableName.toStdString().c_str())->GetRange(range);

	// 添加颜色点
	colorTransferFunction->AddRGBPoint(range[0], 0 / 255.0, 127 / 255.0, 255/ 255.0); // 蓝色
	colorTransferFunction->AddRGBPoint((range[0] + range[1]) / 2.0, 234.0 / 255.0, 213.0 / 255.0, 201.0 / 255.0); // 白色
	colorTransferFunction->AddRGBPoint(range[1], 180.0 / 255.0, 0 / 255.0, 0 / 255.0); // 红色

	// 设置标量数据为指定的物理量
	dataSet->GetPointData()->SetScalars(dataSet->GetPointData()->GetArray(variableName.toStdString().c_str()));

	// 创建面映射器
	vtkSmartPointer<vtkDataSetMapper> surfaceMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	surfaceMapper->SetInputData(dataSet);
	surfaceMapper->SetLookupTable(colorTransferFunction);
	surfaceMapper->SetScalarRange(range);

	// 创建面演员
	vtkSmartPointer<vtkActor> surfaceActor = vtkSmartPointer<vtkActor>::New();
	surfaceActor->SetMapper(surfaceMapper);

	return std::make_tuple(surfaceActor, colorTransferFunction, std::array<double, 2>{range[0], range[1]});
}

std::tuple<vtkSmartPointer<vtkColorTransferFunction>, std::array<double, 2>> createLengendFromFile(const QString& filePath, const QString& variableName)
{
	vtkSmartPointer<vtkAlgorithm> reader;

	// 根据文件扩展名选择合适的读取器
	if (filePath.endsWith(".vtk", Qt::CaseInsensitive)) {
		reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
	}
	else if (filePath.endsWith(".vtu", Qt::CaseInsensitive)) {
		reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
	}
	else if (filePath.endsWith(".vtp", Qt::CaseInsensitive)) {
		reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
	}
	else {
		return std::make_tuple( nullptr, std::array<double, 2>{0.0, 1.0});
	}

	// 设置文件名并更新读取器
	if (auto unstructuredGridReader = vtkUnstructuredGridReader::SafeDownCast(reader)) {
		unstructuredGridReader->SetFileName(filePath.toStdString().c_str());
	}
	else if (auto xmlUnstructuredGridReader = vtkXMLUnstructuredGridReader::SafeDownCast(reader)) {
		xmlUnstructuredGridReader->SetFileName(filePath.toStdString().c_str());
	}
	else if (auto xmlPolyDataReader = vtkXMLPolyDataReader::SafeDownCast(reader)) {
		xmlPolyDataReader->SetFileName(filePath.toStdString().c_str());
	}
	reader->Update();

	// 获取数据集
	vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid = vtkUnstructuredGrid::SafeDownCast(reader->GetOutputDataObject(0));
	vtkSmartPointer<vtkPolyData> polyData = vtkPolyData::SafeDownCast(reader->GetOutputDataObject(0));

	vtkSmartPointer<vtkDataSet> dataSet;
	if (unstructuredGrid) {
		dataSet = unstructuredGrid;
	}
	else if (polyData) {
		dataSet = polyData;
	}
	else {
		return std::make_tuple(nullptr, std::array<double, 2>{0.0, 1.0});
	}

	// 检查是否包含指定的物理量
	if (!dataSet->GetPointData()->HasArray(variableName.toStdString().c_str())) {
		return std::make_tuple( nullptr, std::array<double, 2>{0.0, 1.0});
	}

	// 创建颜色传输函数
	vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	colorTransferFunction->SetColorSpaceToRGB();

	// 设置颜色映射范围
	double range[2];
	dataSet->GetPointData()->GetArray(variableName.toStdString().c_str())->GetRange(range);

	// 添加颜色点
	colorTransferFunction->AddRGBPoint(range[0], 0 / 255.0, 127 / 255.0, 255 / 255.0); // 蓝色
	colorTransferFunction->AddRGBPoint((range[0] + range[1]) / 2.0, 234.0 / 255.0, 213.0 / 255.0, 201.0 / 255.0); // 白色
	colorTransferFunction->AddRGBPoint(range[1], 180.0 / 255.0, 0 / 255.0, 0 / 255.0); // 红色

	// 设置标量数据为指定的物理量
	dataSet->GetPointData()->SetScalars(dataSet->GetPointData()->GetArray(variableName.toStdString().c_str()));

	return std::make_tuple(colorTransferFunction, std::array<double, 2>{range[0], range[1]});
}

void MainWindow::formPostprocessing_loadData()
{
	double time = GlobalData::getInstance().getCaseData()->times.back();
	QString variableName = GlobalData::getInstance().getCaseData()->variableNames[0];
	QString meshPartName = GlobalData::getInstance().getCaseData()->meshPartName[0];

	QString caseFilePath = QString::fromStdString(GlobalData::getInstance().getCaseData()->casePath);
	QFileInfo fileInfo(caseFilePath);
	QString caseDirPath = fileInfo.absolutePath();
	QString caseDirName = fileInfo.dir().dirName();
	QString  internalPath = caseDirPath + "/VTK/" + caseDirName + "_" + QString::number(time) + "/internal.vtu";

	auto result = createActorFromFile(internalPath, variableName);
	vtkSmartPointer<vtkActor> actor = std::get<0>(result);
	vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = std::get<1>(result);
	std::array<double, 2> range = std::get<2>(result);

	if (actor) {
		// 清除以前的演员
		render->RemoveAllViewProps();

		// 添加新的演员
		render->AddActor(actor);

		// 创建图例
		vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
		scalarBar->SetLookupTable(colorTransferFunction);
		//scalarBar->SetTitle(variableName.toStdString().c_str());
		scalarBar->SetNumberOfLabels(4);
		scalarBar->SetOrientationToVertical();
		scalarBar->SetPosition(0.92, 0.01); // 设置图例的位置
		scalarBar->SetWidth(0.06); // 设置图例的宽度（相对于渲染窗口的比例）
		scalarBar->SetHeight(0.3); // 设置图例的高度（相对于渲染窗口的比例）
		scalarBar->SetLabelFormat("%1.2e"); // 设置标签格式为科学计数法，保留两位小数

		// 设置图例标题的文本属性
		vtkSmartPointer<vtkTextProperty> titleTextProperty = vtkSmartPointer<vtkTextProperty>::New();
		titleTextProperty->SetFontSize(24); // 设置标题字体大小
		titleTextProperty->SetColor(1.0, 1.0, 1.0); // 设置标题颜色为白色
		titleTextProperty->SetBold(1); // 设置标题为粗体
		titleTextProperty->SetJustificationToCentered(); // 设置标题居中对齐
		scalarBar->SetTitleTextProperty(titleTextProperty);

		// 设置图例标签的文本属性
		vtkSmartPointer<vtkTextProperty> labelTextProperty = vtkSmartPointer<vtkTextProperty>::New();
		labelTextProperty->SetFontSize(18); // 设置标签字体大小
		labelTextProperty->SetColor(0, 0, 0); // 设置标签颜色为白色
		scalarBar->SetLabelTextProperty(labelTextProperty);

		// 添加图例
		render->AddActor2D(scalarBar);

		// 调整视角到合适的大小
		render->ResetCamera();

		// 渲染
		renderWindow->Render();
	}
}

void MainWindow::formPostprocessing_apply()
{
	double time = formPostprocessing->ui->comboBox->currentText().toDouble();
	QString variableName = formPostprocessing->ui->comboBox_2->currentText();

	// 获取 QTreeView 的模型
	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(formPostprocessing->ui->treeView->model());
	if (!model) {
		return;
	}

	// 遍历模型中的所有项
	render->RemoveAllViewProps();
	QString internalPath;
	QString caseFilePath = QString::fromStdString(GlobalData::getInstance().getCaseData()->casePath);
	QFileInfo fileInfo(caseFilePath);
	QString caseDirPath = fileInfo.absolutePath();
	QString caseDirName = fileInfo.dir().dirName();
	for (int row = 0; row < model->rowCount(); ++row) {
		QStandardItem* item = model->item(row);
		if (item->checkState() == Qt::Checked) {
			QString meshPartName = item->text();
			if (meshPartName == "internal") {
				internalPath = caseDirPath + "/VTK/" + caseDirName + "_" + QString::number(time) + "/" + meshPartName + ".vtu";
			}
			else {
				internalPath = caseDirPath + "/VTK/" + caseDirName + "_" + QString::number(time) + "/boundary/" + meshPartName + ".vtp";
			}

			auto result = createActorFromFile(internalPath, variableName);
			vtkSmartPointer<vtkActor> actor = std::get<0>(result);
			vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = std::get<1>(result);
			std::array<double, 2> range = std::get<2>(result);
			render->AddActor(actor);
		}
	}

	//创建图例
	internalPath = caseDirPath + "/VTK/" + caseDirName + "_" + formPostprocessing->ui->comboBox->itemText(formPostprocessing->ui->comboBox->count() - 1) + "/internal.vtu";
	auto result = createLengendFromFile(internalPath, variableName);
	vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = std::get<0>(result);
	std::array<double, 2> range = std::get<1>(result);

	// 创建图例
	vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
	scalarBar->SetLookupTable(colorTransferFunction);
	//scalarBar->SetTitle(variableName.toStdString().c_str());
	scalarBar->SetNumberOfLabels(4);
	scalarBar->SetOrientationToVertical();
	scalarBar->SetPosition(0.92, 0.01); // 设置图例的位置
	scalarBar->SetWidth(0.06); // 设置图例的宽度（相对于渲染窗口的比例）
	scalarBar->SetHeight(0.3); // 设置图例的高度（相对于渲染窗口的比例）
	scalarBar->SetLabelFormat("%1.2e"); // 设置标签格式为科学计数法，保留两位小数

	// 设置图例标题的文本属性
	vtkSmartPointer<vtkTextProperty> titleTextProperty = vtkSmartPointer<vtkTextProperty>::New();
	titleTextProperty->SetFontSize(24); // 设置标题字体大小
	titleTextProperty->SetColor(1.0, 1.0, 1.0); // 设置标题颜色为白色
	titleTextProperty->SetBold(1); // 设置标题为粗体
	titleTextProperty->SetJustificationToCentered(); // 设置标题居中对齐
	scalarBar->SetTitleTextProperty(titleTextProperty);

	// 设置图例标签的文本属性
	vtkSmartPointer<vtkTextProperty> labelTextProperty = vtkSmartPointer<vtkTextProperty>::New();
	labelTextProperty->SetFontSize(18); // 设置标签字体大小
	labelTextProperty->SetColor(0, 0, 0); // 设置标签颜色为白色
	scalarBar->SetLabelTextProperty(labelTextProperty);

	// 添加图例
	render->AddActor2D(scalarBar);

	// 渲染
	renderWindow->Render();
}

void MainWindow::formPostprocessing_firstFrame()
{
	formPostprocessing->ui->comboBox->setCurrentIndex(0);
	formPostprocessing_apply();
}

void MainWindow::formPostprocessing_previousFrame()
{
	int index = formPostprocessing->ui->comboBox->currentIndex();
	if (index > 0) {
		formPostprocessing->ui->comboBox->setCurrentIndex(index - 1);
		formPostprocessing_apply();
	}
}

void MainWindow::formPostprocessing_reverse()
{
	reverseTimer->start(100);
}

void MainWindow::formPostprocessing_play()
{
	playTimer->start(100);
}

void MainWindow::formPostprocessing_nextFrame()
{
	int index = formPostprocessing->ui->comboBox->currentIndex();
	if (index < formPostprocessing->ui->comboBox->count() - 1) {
		formPostprocessing->ui->comboBox->setCurrentIndex(index + 1);
		formPostprocessing_apply();
	}
}

void MainWindow::formPostprocessing_lastFrame()
{
	formPostprocessing->ui->comboBox->setCurrentIndex(formPostprocessing->ui->comboBox->count() - 1);
	formPostprocessing_apply();
}

void MainWindow::formPostprocessing_loopPlay()
{
	loopPlayTimer->start(100);
}

void MainWindow::formPostprocessing_playPause()
{
	playTimer->stop();
	formPostprocessing->pushButtonPlayTimerPause->hide();
	formPostprocessing->ui->pushButton_6->show();
}

void MainWindow::formPostprocessing_reversePause()
{
	reverseTimer->stop();
	formPostprocessing->pushButtonReverseTimerPause->hide();
	formPostprocessing->ui->pushButton_5->show();
}

void MainWindow::formPostprocessing_loopPlayPause()
{
	loopPlayTimer->stop();
	formPostprocessing->pushButtonLoopPlayTimerPause->hide();
	formPostprocessing->ui->pushButton_9->show();
}

void MainWindow::onButtonClicked()
{
	QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
	if (clickedButton) {
		// 还原上一个点击的按钮背景色
		if (lastClickedButton) {
			lastClickedButton->setStyleSheet(
				"QPushButton {"
				"    background-color: rgb(255, 255, 255);"
				"    border: none;"
				"	 text-align: left;"
				"	 padding-left: 50px;"
				"}"
				"QPushButton:hover {"
				"    background-color: rgb(242, 242, 242);"
				"}"
			);
		}
		// 设置当前点击的按钮背景色
		clickedButton->setStyleSheet(
			"QPushButton {"
			"    background-color: rgb(232, 232, 232);"
			"    border: none;"
			"	 text-align: left;"
			"	 padding-left: 50px;"
			"}"
		);
		// 更新上一个点击的按钮
		lastClickedButton = clickedButton;
	}
}

void MainWindow::onPlayTimerTimeout()
{
	int index = formPostprocessing->ui->comboBox->currentIndex();
	if (index < formPostprocessing->ui->comboBox->count() - 1) {
		formPostprocessing->ui->comboBox->setCurrentIndex(index + 1);
		formPostprocessing_apply();
		formPostprocessing->pushButtonPlayTimerPause->show();
		formPostprocessing->ui->pushButton_6->hide();
	}
	else {
		playTimer->stop();
		formPostprocessing->pushButtonPlayTimerPause->hide();
		formPostprocessing->ui->pushButton_6->show();
	}
}

void MainWindow::onReverseTimerTimeout()
{
	int index = formPostprocessing->ui->comboBox->currentIndex();
	if (index > 0) {
		formPostprocessing->ui->comboBox->setCurrentIndex(index - 1);
		formPostprocessing_apply();
		formPostprocessing->pushButtonReverseTimerPause->show();
		formPostprocessing->ui->pushButton_5->hide();
	}
	else {
		reverseTimer->stop();
		formPostprocessing->pushButtonReverseTimerPause->hide();
		formPostprocessing->ui->pushButton_5->show();
	}
}

void MainWindow::onLoopPlayTimerTimeout()
{
	int index = formPostprocessing->ui->comboBox->currentIndex();
	if (index < formPostprocessing->ui->comboBox->count() - 1) {
		formPostprocessing->ui->comboBox->setCurrentIndex(index + 1);

	}
	else {
		formPostprocessing->ui->comboBox->setCurrentIndex(0);
	}
	formPostprocessing_apply();
	formPostprocessing->pushButtonLoopPlayTimerPause->show();
	formPostprocessing->ui->pushButton_9->hide();
}

void MainWindow::handleAction1Triggered()
{
    ui->textBrowser->setVisible(!ui->textBrowser->isVisible());
}