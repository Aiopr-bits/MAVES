#pragma once
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
	, playTimer(new QTimer(this))
	, reverseTimer(new QTimer(this))
	, loopPlayTimer(new QTimer(this))
	, lastClickedButton(nullptr)
	, process(this)
	, processRun(this)
	, chart(new QChart())
	, axisX(new QValueAxis())
	, axisY(new QLogValueAxis())
	, currentTimeStep(0)
	, chartUpdateTimer(new QTimer(this))
	, axisMinX(0)
	, axisMaxX(0)
	, axisMinY(0.01)
	, axisMaxY(1)
	, planeRepModelClip(vtkSmartPointer<vtkImplicitPlaneRepresentation>::New())
	, planeWidgetModelClip(vtkSmartPointer<vtkImplicitPlaneWidget2>::New())
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
    render->SetBackground2(27/255.0, 85/255.0, 133/255.0); 
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

	//布局管理
	QList<int> sizes;
	sizes << 400 << 100; 
	ui->splitter->setSizes(sizes);

	//创建各个子面板
	formMesh = new FormMesh(this);
	formPostprocessing = new FormPostprocessing(this);
	formRun = new FormRun(this);
	formGeometry = new FormGeometry(this);
	formMeshImport = new FormMeshImport(this);
	formModelClip = new FormModelClip(this);
	ui->gridLayout_3->addWidget(formMesh, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formPostprocessing, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formRun, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formGeometry, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formMeshImport, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formModelClip, 0, 0, 1, 1);
	formMesh->hide();
	formPostprocessing->hide();
	formRun->hide();
	formGeometry->hide();
	formMeshImport->hide();
	formModelClip->hide();

	//程序启动点击几何页面
	on_pushButton_clicked();
	ui->pushButton->setStyleSheet("QPushButton { background-color: rgb(232, 232, 232); border: none; text-align: left; padding-left: 50px; }");
	lastClickedButton = ui->pushButton;

	//对信息输出框滚动条进行polish抛光
	ui->textBrowser->verticalScrollBar()->style()->polish(ui->textBrowser->verticalScrollBar());

	// 初始化图表
	axisX->setTitleText("迭代次数");
	axisX->setLabelFormat("%d");
	axisY->setTitleText("残差");
	axisY->setBase(10);  
	axisY->setMin(axisMinY);
	axisY->setMax(axisMaxY);
	axisX->setMin(axisMinX);
	axisX->setMax(axisMaxX);
	chart->addAxis(axisX, Qt::AlignBottom);
	chart->addAxis(axisY, Qt::AlignLeft);
	chart->setAnimationOptions(QChart::SeriesAnimations);

	// 设置图表的上框线和右框线
	QPen pen(QColor(196, 196, 196));
	pen.setWidth(1);
	chart->setPlotAreaBackgroundVisible(true);
	chart->setPlotAreaBackgroundPen(pen);

	ui->chartView->setChart(chart);
	ui->chartView->setRenderHint(QPainter::Antialiasing);

	// 初始化残差图刷新定时器
	chartUpdateTimer->start(100);

    // 工具栏信号处理
	connect(ui->action1, &QAction::triggered, this, &MainWindow::handleAction1Triggered);														//信息框
	connect(ui->action2, &QAction::triggered, this, &MainWindow::handleAction2Triggered);														//x正向
	connect(ui->action3, &QAction::triggered, this, &MainWindow::handleAction3Triggered);														//x负向
	connect(ui->action4, &QAction::triggered, this, &MainWindow::handleAction4Triggered);														//y正向
	connect(ui->action5, &QAction::triggered, this, &MainWindow::handleAction5Triggered);														//y负向
	connect(ui->action6, &QAction::triggered, this, &MainWindow::handleAction6Triggered);														//z正向
	connect(ui->action7, &QAction::triggered, this, &MainWindow::handleAction7Triggered);														//z负向
	connect(ui->action8, &QAction::triggered, this, &MainWindow::handleAction8Triggered);														//适应窗口
	connect(ui->action9, &QAction::triggered, this, &MainWindow::handleAction9Triggered);														//模型切分

	//主界面其他事件处理
	connect(playTimer, &QTimer::timeout, this, &MainWindow::onPlayTimerTimeout);																//播放
	connect(reverseTimer, &QTimer::timeout, this, &MainWindow::onReverseTimerTimeout);															//倒放
	connect(loopPlayTimer, &QTimer::timeout, this, &MainWindow::onLoopPlayTimerTimeout);														//循环播放
	connect(&processRun, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessRunOutput);											//求解计算进程输出
	connect(&processFoamToVTK, &QProcess::readyReadStandardOutput, this, &MainWindow::onprocessFoamToVTKOutput);								//foamToVTK进程输出
	connect(&process, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessOutput);													//进程输出
	connect(&process, &QProcess::readyReadStandardError, this, &MainWindow::onProcessError);													//进程错误
	connect(&processRun, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onProcessRunFinished);				//求解计算进程结束
	connect(&processFoamToVTK, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onProcessFoamToVTKFinished);	//foamToVTK进程结束
	connect(chartUpdateTimer, &QTimer::timeout, this, &MainWindow::updateChart); 																//更新残差图
	planeRepModelClip->AddObserver(vtkCommand::ModifiedEvent, this, &MainWindow::updatePlaneRepModelClipValues); 				 				//更新模型切分平面选择器的值

	//副控制面板事件处理
	connect(formGeometry, &FormGeometry::geometryImported, this, &MainWindow::formGeometry_import);												//导入几何
	connect(formMeshImport, &FormMeshImport::meshImported, this, &MainWindow::formMeshImport_import);											//导入网格
	connect(formMesh, &FormMesh::meshVisibilityChanged, this, &MainWindow::formMesh_apply);														//网格应用
	connect(formRun, &FormRun::run, this, &MainWindow::formRun_run);																			//求解计算
	connect(formRun, &FormRun::stopRun, this, &MainWindow::formRun_stopRun);																	//停止计算
	connect(formPostprocessing, &FormPostprocessing::resultDataLoaded, this, &MainWindow::formPostprocessing_loadData);							//加载结果数据
	connect(formPostprocessing, &FormPostprocessing::apply, this, &MainWindow::formPostprocessing_apply);										//更新渲染窗口
	connect(formPostprocessing, &FormPostprocessing::firstFrame, this, &MainWindow::formPostprocessing_firstFrame);								//第一帧
	connect(formPostprocessing, &FormPostprocessing::previousFrame, this, &MainWindow::formPostprocessing_previousFrame);						//上一帧
	connect(formPostprocessing, &FormPostprocessing::reverse, this, &MainWindow::formPostprocessing_reverse);									//重新播放
	connect(formPostprocessing, &FormPostprocessing::play, this, &MainWindow::formPostprocessing_play);											//播放
	connect(formPostprocessing, &FormPostprocessing::nextFrame, this, &MainWindow::formPostprocessing_nextFrame);								//下一帧
	connect(formPostprocessing, &FormPostprocessing::lastFrame, this, &MainWindow::formPostprocessing_lastFrame);								//最后一帧
	connect(formPostprocessing, &FormPostprocessing::loopPlay, this, &MainWindow::formPostprocessing_loopPlay);									//循环播放
	connect(formPostprocessing, &FormPostprocessing::playPause, this, &MainWindow::formPostprocessing_playPause);								//播放暂停
	connect(formPostprocessing, &FormPostprocessing::reversePause, this, &MainWindow::formPostprocessing_reversePause);							//反向播放暂停
	connect(formPostprocessing, &FormPostprocessing::loopPlayPause, this, &MainWindow::formPostprocessing_loopPlayPause);						//循环播放暂停
	connect(formModelClip, &FormModelClip::checkBoxToggled, this, &MainWindow::formModelClip_checkBoxToggle);									//模型切分页面CheckBox切换
	connect(formModelClip, &FormModelClip::lineEditsChanged, this, &MainWindow::formModelClip_lineEditsChanged);								//模型切分页面LineEdit值改变
	connect(formModelClip, &FormModelClip::xPositive, this, &MainWindow::formModelClip_xPositive);												//模型切分：X正向
	connect(formModelClip, &FormModelClip::yPositive, this, &MainWindow::formModelClip_yPositive);												//模型切分：Y正向
	connect(formModelClip, &FormModelClip::zPositive, this, &MainWindow::formModelClip_zPositive);												//模型切分：Z正向
	connect(formModelClip, &FormModelClip::cameraDirection, this, &MainWindow::formModelClip_cameraDirection);									//模型切分：相机方向
	connect(formModelClip, &FormModelClip::alignView, this, &MainWindow::formModelClip_alignView);												//模型切分：对齐视角
	connect(formModelClip, &FormModelClip::resetPlane, this, &MainWindow::formModelClip_resetPlane);											//模型切分：重置平面
	connect(formModelClip, &FormModelClip::apply, this, &MainWindow::formModelClip_apply);														//模型切分：应用
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
	formRun->hide();
	formGeometry->hide();
	formMeshImport->hide();
	formModelClip->hide();
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

void MainWindow::handleAction9Triggered()
{
	on_pushButton_3_clicked();
}

void MainWindow::on_pushButton_clicked()
{
	hideAllSubForm();
	formGeometry->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::on_pushButton_4_clicked()
{
	hideAllSubForm();
	formMeshImport->show();	
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::on_pushButton_2_clicked()
{
	hideAllSubForm();
	formMesh->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::on_pushButton_16_clicked()
{
	hideAllSubForm();
	formRun->show();
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::on_pushButton_17_clicked()
{
	hideAllSubForm();
	formPostprocessing->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::on_pushButton_3_clicked()
{
	hideAllSubForm();
	formModelClip->show();
	ui->tabWidget->setCurrentIndex(0);
	formModelClip->ui->checkBox->setChecked(true);

	//按钮颜色处理
	QPushButton* clickedButton = ui->pushButton_3;
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

	//平面选择器
	planeRepModelClip = vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();
	planeWidgetModelClip = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
	planeRepModelClip->AddObserver(vtkCommand::ModifiedEvent, this, &MainWindow::updatePlaneRepModelClipValues);
	planeRepModelClip->SetPlaceFactor(1.5);
	vtkSmartPointer<vtkPropCollection> actors = render->GetViewProps();
	actors->InitTraversal();
	vtkSmartPointer<vtkProp> actor = actors->GetNextProp();

	double bounds[6] = { VTK_DOUBLE_MAX, VTK_DOUBLE_MIN, VTK_DOUBLE_MAX, VTK_DOUBLE_MIN, VTK_DOUBLE_MAX, VTK_DOUBLE_MIN };
	int visibleActorCount = 0;
	while (actor)
	{
		vtkSmartPointer<vtkActor> actor_ = vtkActor::SafeDownCast(actor);
		if (actor_ && actor_->GetVisibility())
		{
			double* actorBounds = actor_->GetBounds();
			bounds[0] = std::min(bounds[0], actorBounds[0]);
			bounds[1] = std::max(bounds[1], actorBounds[1]);
			bounds[2] = std::min(bounds[2], actorBounds[2]);
			bounds[3] = std::max(bounds[3], actorBounds[3]);
			bounds[4] = std::min(bounds[4], actorBounds[4]);
			bounds[5] = std::max(bounds[5], actorBounds[5]);
			visibleActorCount++;
		}
		actor = actors->GetNextProp();
	}

	if (visibleActorCount == 0) return;

	double origin[3];
	origin[0] = (bounds[0] + bounds[1]) / 2.0;
	origin[1] = (bounds[2] + bounds[3]) / 2.0;
	origin[2] = (bounds[4] + bounds[5]) / 2.0;

	double normal[3] = { 1, 1, 0 };

	planeRepModelClip->SetOrigin(origin);
	planeRepModelClip->SetNormal(normal);
	planeRepModelClip->PlaceWidget(bounds); 

	planeWidgetModelClip->SetRepresentation(planeRepModelClip);
	planeWidgetModelClip->SetInteractor(ui->openGLWidget->renderWindow()->GetInteractor());
	planeWidgetModelClip->On();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::updatePlaneRepModelClipValues()
{
	double origin[3];
	double normal[3];

	planeRepModelClip->GetOrigin(origin);
	planeRepModelClip->GetNormal(normal);

	// 临时禁用信号
	QSignalBlocker blocker1(formModelClip->ui->lineEdit);
	QSignalBlocker blocker2(formModelClip->ui->lineEdit_2);
	QSignalBlocker blocker3(formModelClip->ui->lineEdit_3);
	QSignalBlocker blocker4(formModelClip->ui->lineEdit_4);
	QSignalBlocker blocker5(formModelClip->ui->lineEdit_5);
	QSignalBlocker blocker6(formModelClip->ui->lineEdit_6);

	formModelClip->ui->lineEdit->setText(QString::number(origin[0]));
	formModelClip->ui->lineEdit_2->setText(QString::number(origin[1]));
	formModelClip->ui->lineEdit_3->setText(QString::number(origin[2]));
	formModelClip->ui->lineEdit_4->setText(QString::number(normal[0]));
	formModelClip->ui->lineEdit_5->setText(QString::number(normal[1]));
	formModelClip->ui->lineEdit_6->setText(QString::number(normal[2]));
}

void MainWindow::formGeometry_import(const QString& filePath)
{
	QFileInfo fileInfo(filePath);
	if (!fileInfo.exists())
	{
		QMessageBox::warning(this, tr("错误"), tr("文件不存在"));
		return;
	}

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

void MainWindow::formMeshImport_import(const QString& filePath)
{
	QFileInfo fileInfo(filePath);
	if (!fileInfo.exists())
	{
		QMessageBox::warning(this, tr("错误"), tr("文件不存在"));
		return;
	}

	QString type = fileInfo.suffix().toLower();
	render->RemoveAllViewProps();

	if (type == "foam")
	{
		QString casePath = fileInfo.path();
		std::string command = "foamToVTK -time 0 -case " + casePath.toStdString();

		//删除原来的VTK文件夹
		QDir dirVTK(casePath + "/VTK");
		if (dirVTK.exists())
		{
			dirVTK.removeRecursively();
		}

		process.setProgram("cmd.exe");
		process.setArguments(QStringList() << "/C" << QString::fromStdString(command));
		process.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments* args) {
			args->flags |= CREATE_NO_WINDOW;
			});
		process.start();
		process.waitForFinished();

		QString folderName = casePath.split("/").last();
		QString vtpPath = casePath + "/VTK/" + folderName + "_0/boundary/";

		// 获取vtpPath文件夹下的所有.vtp文件路径
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
			actor->GetProperty()->SetColor(0, 221, 221);

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

		//网格导入成功之后，自动跳转到网格页面
		on_pushButton_2_clicked();
		ui->pushButton_2->setStyleSheet("QPushButton { background-color: rgb(232, 232, 232); border: none; text-align: left; padding-left: 50px; }");
		lastClickedButton->setStyleSheet("QPushButton { background-color: rgb(255, 255, 255); border: none; text-align: left; padding-left: 50px; } QPushButton:hover { background-color: rgb(242, 242, 242); }");
		lastClickedButton = ui->pushButton_2;
	}
}

void MainWindow::formMesh_apply()
{
	render->RemoveAllViewProps();
	const auto& meshFaceActors = GlobalData::getInstance().getCaseData()->meshFaceActors;
	const auto& meshEdgeActors = GlobalData::getInstance().getCaseData()->meshEdgeActors;

	// 遍历 treeView 并更新 actor
	for (int i = 0; i < formMesh->treeViewModel->rowCount(); ++i)
	{
		QStandardItem* item = formMesh->treeViewModel->item(i);
		const auto& actor = meshFaceActors.find(item->text());
		if (actor != meshFaceActors.end() && item->checkState() == Qt::Checked)
		{
			render->AddActor(actor->second);
		}

		const auto& edgeActor = meshEdgeActors.find(item->text());
		if (edgeActor != meshEdgeActors.end() && item->checkState() == Qt::Checked)
		{
			render->AddActor(edgeActor->second);
		}
	}

	render->ResetCamera();
	renderWindow->Render();
}

void MainWindow::formRun_run()
{
	//保存界面上所有的配置参数，并校验是否符合要求


	//初始化残差图数据
	seriesMap.clear();
	seriesRangeMap.clear();
	currentTimeStep = 0;
	chart->removeAllSeries();
	axisMaxX = 0;
	ui->tabWidget->setCurrentIndex(1);	

	//获取案例路径
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QFileInfo fileInfo(casePath);
	QString caseDir = fileInfo.path();
	QString controlDictPath = caseDir + "/system/controlDict";

	// 删除原来的计算结果
	QDir dir(caseDir);
	QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	foreach(QString folder, folders)
	{
		bool ok;
		double folderNumber = folder.toDouble(&ok);
		if (ok && folderNumber != 0.0)
		{
			QDir folderDir(caseDir + "/" + folder);
			folderDir.removeRecursively();
		}
	}

	// 打开 controlDict 文件
	QFile controlDictFile(controlDictPath);
	if (!controlDictFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("错误"), tr("无法打开 controlDict 文件"));
		return;
	}

	// 读取 controlDict 文件中的 application 字段
	QString application;
	QTextStream in(&controlDictFile);
	while (!in.atEnd()) {
		QString line = in.readLine();
		if (line.trimmed().startsWith("application")) {
			QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			if (parts.size() >= 2) {
				application = parts[1].trimmed();
				if (application.endsWith(";")) {
					application.chop(1); // 去除末尾的分号
				}
				break;
			}
		}
	}
	controlDictFile.close();

	if (application.isEmpty()) {
		QMessageBox::warning(this, tr("错误"), tr("未找到 application 字段"));
		return;
	}

	// 构建并执行命令
	QString command = application + " -case " + caseDir;

	processRun.setProgram("cmd.exe");
	processRun.setArguments(QStringList() << "/C" << command);
	processRun.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments* args) {
		args->flags |= CREATE_NO_WINDOW;
		});
	processRun.start();
}

void MainWindow::onProcessRunFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitCode);
	Q_UNUSED(exitStatus);
	formRun->on_pushButton_clicked_2();

	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QFileInfo fileInfo(casePath);
	QString caseDir = fileInfo.path();

	//删除原来的VTK文件夹
	QDir dir(caseDir + "/VTK");
	if (dir.exists())
	{
		dir.removeRecursively();
	}

	QString command = "foamToVTK -case " + caseDir;
	processFoamToVTK.setProgram("cmd.exe");
	processFoamToVTK.setArguments(QStringList() << "/C" << command);
	processFoamToVTK.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments* args) {
		args->flags |= CREATE_NO_WINDOW;
		});
	processFoamToVTK.start();
}

void MainWindow::onProcessFoamToVTKFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitCode);
	Q_UNUSED(exitStatus);
	formPostprocessing->loadResultData();
}

void MainWindow::formRun_stopRun()
{
	if (processRun.state() == QProcess::Running) {
		processRun.kill();
	}
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

void MainWindow::formModelClip_checkBoxToggle()
{
	//如果没有可见演员则直接返回
	if (render->GetActors()->GetNumberOfItems() == 0) {
		return;
	}

	planeWidgetModelClip->SetInteractor(ui->openGLWidget->renderWindow()->GetInteractor());
	
	if (formModelClip->ui->checkBox->isChecked()) {
		planeWidgetModelClip->On();
	}
	else {
		planeWidgetModelClip->Off();
	}
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::formModelClip_lineEditsChanged()
{
	double origin[3];
	double normal[3];
	origin[0] = formModelClip->ui->lineEdit->text().toDouble();
	origin[1] = formModelClip->ui->lineEdit_2->text().toDouble();
	origin[2] = formModelClip->ui->lineEdit_3->text().toDouble();
	normal[0] = formModelClip->ui->lineEdit_4->text().toDouble();
	normal[1] = formModelClip->ui->lineEdit_5->text().toDouble();
	normal[2] = formModelClip->ui->lineEdit_6->text().toDouble();
	planeRepModelClip->SetOrigin(origin);
	planeRepModelClip->SetNormal(normal);
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::formModelClip_xPositive()
{
	if (render->GetActors()->GetNumberOfItems() == 0) {
		return;
	}

	planeRepModelClip->SetNormal(1, 0, 0); 
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::formModelClip_yPositive()
{
	if (render->GetActors()->GetNumberOfItems() == 0) {
		return;
	}
	planeRepModelClip->SetNormal(0, 1, 0);
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::formModelClip_zPositive()
{
	if (render->GetActors()->GetNumberOfItems() == 0) {
		return;
	}
	planeRepModelClip->SetNormal(0, 0, 1);
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::formModelClip_cameraDirection()
{
	if (render->GetActors()->GetNumberOfItems() == 0) {
		return;
	}
	vtkCamera* camera = render->GetActiveCamera();
	double cameraDirection[3];
	camera->GetDirectionOfProjection(cameraDirection);
	planeRepModelClip->SetNormal(cameraDirection);
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::formModelClip_alignView()
{
	if (render->GetActors()->GetNumberOfItems() == 0) {
		return;
	}

	double normal[3];
	double origin[3];
	planeRepModelClip->GetNormal(normal);
	planeRepModelClip->GetOrigin(origin);

	vtkCamera* camera = render->GetActiveCamera();
	double cameraPosition[3];
	double distance = camera->GetDistance();
	for (int i = 0; i < 3; ++i) {
		cameraPosition[i] = origin[i] - normal[i] * distance;
	}
	camera->SetPosition(cameraPosition);
	camera->SetFocalPoint(origin);

	const double* viewUpPtr = camera->GetViewUp();
	double viewUp[3] = { viewUpPtr[0], viewUpPtr[1], viewUpPtr[2] };
	if (normal[0] == 0.0 && normal[1] == 0.0) {
		viewUp[1] = 1.0; // 如果法向量在Z轴方向上，设置Y轴为上方向
	}
	camera->SetViewUp(viewUp);

	render->ResetCamera();
	planeWidgetModelClip->GetInteractor()->Render();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::formModelClip_resetPlane()
{
	if (render->GetActors()->GetNumberOfItems() == 0) {
		return;
	}

	// 重新创建平面选择器和表示对象
	planeRepModelClip = vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();
	planeWidgetModelClip = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
	planeRepModelClip->AddObserver(vtkCommand::ModifiedEvent, this, &MainWindow::updatePlaneRepModelClipValues); 
	formModelClip->ui->checkBox->setChecked(true);

	// 设置平面选择器的放置因子
	planeRepModelClip->SetPlaceFactor(1.5);

	// 获取所有演员的边界
	vtkSmartPointer<vtkPropCollection> actors = render->GetViewProps();
	actors->InitTraversal();
	vtkSmartPointer<vtkProp> actor = actors->GetNextProp();

	double bounds[6] = { VTK_DOUBLE_MAX, VTK_DOUBLE_MIN, VTK_DOUBLE_MAX, VTK_DOUBLE_MIN, VTK_DOUBLE_MAX, VTK_DOUBLE_MIN };
	while (actor)
	{
		vtkSmartPointer<vtkActor> actor_ = vtkActor::SafeDownCast(actor);
		if (actor_ && actor_->GetVisibility())
		{
			double* actorBounds = actor_->GetBounds();
			bounds[0] = std::min(bounds[0], actorBounds[0]);
			bounds[1] = std::max(bounds[1], actorBounds[1]);
			bounds[2] = std::min(bounds[2], actorBounds[2]);
			bounds[3] = std::max(bounds[3], actorBounds[3]);
			bounds[4] = std::min(bounds[4], actorBounds[4]);
			bounds[5] = std::max(bounds[5], actorBounds[5]);
		}
		actor = actors->GetNextProp();
	}

	// 计算平面选择器的原点和法向量
	double origin[3];
	origin[0] = (bounds[0] + bounds[1]) / 2.0;
	origin[1] = (bounds[2] + bounds[3]) / 2.0;
	origin[2] = (bounds[4] + bounds[5]) / 2.0;

	double normal[3] = { 1, 1, 0 };

	// 设置平面选择器的原点和法向量
	planeRepModelClip->SetOrigin(origin);
	planeRepModelClip->SetNormal(normal);
	planeRepModelClip->PlaceWidget(bounds);

	// 启用平面选择器
	planeWidgetModelClip->SetRepresentation(planeRepModelClip);
	planeWidgetModelClip->SetInteractor(ui->openGLWidget->renderWindow()->GetInteractor());
	planeWidgetModelClip->On();

	// 渲染窗口
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::formModelClip_apply()
{
	if (render->GetActors()->GetNumberOfItems() == 0) {
		return;
	}

	formModelClip->ui->checkBox->setChecked(false);

	// 获取平面选择器的原点和法向量
	double origin[3];
	double normal[3];
	planeRepModelClip->GetOrigin(origin);
	planeRepModelClip->GetNormal(normal);

	// 创建一个平面
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetOrigin(origin);
	plane->SetNormal(normal);

	// 存储切分后的演员
	std::vector<vtkSmartPointer<vtkActor>> clippedActors;

	// 遍历所有可见的演员并进行切分
	vtkSmartPointer<vtkPropCollection> actors = render->GetViewProps();
	actors->InitTraversal();
	vtkSmartPointer<vtkProp> actor = actors->GetNextProp();

	while (actor)
	{
		vtkSmartPointer<vtkActor> actor_ = vtkActor::SafeDownCast(actor);
		if (actor_ && actor_->GetVisibility())
		{
			vtkSmartPointer<vtkPolyDataMapper> polyDataMapper = vtkPolyDataMapper::SafeDownCast(actor_->GetMapper());
			vtkSmartPointer<vtkDataSetMapper> dataSetMapper = vtkDataSetMapper::SafeDownCast(actor_->GetMapper());

			if (polyDataMapper || dataSetMapper)
			{
				vtkSmartPointer<vtkPolyData> polyData = nullptr;

				if (polyDataMapper)
				{
					polyData = polyDataMapper->GetInput();
				}
				else if (dataSetMapper)
				{
					vtkSmartPointer<vtkDataSet> dataSet = dataSetMapper->GetInput();
					polyData = vtkPolyData::SafeDownCast(dataSet);

					// 如果数据集不是 vtkPolyData 类型，则尝试将其转换为 vtkPolyData
					if (!polyData)
					{
						vtkSmartPointer<vtkGeometryFilter> geometryFilter = vtkSmartPointer<vtkGeometryFilter>::New();
						geometryFilter->SetInputData(dataSet);
						geometryFilter->Update();
						polyData = geometryFilter->GetOutput();
					}
				}

				if (polyData)
				{
					// 使用平面切分多边形数据
					vtkSmartPointer<vtkClipPolyData> clipper = vtkSmartPointer<vtkClipPolyData>::New();
					clipper->SetInputData(polyData);
					clipper->SetClipFunction(plane);
					clipper->Update();

					// 获取切分后的数据
					vtkSmartPointer<vtkPolyData> clippedPolyData = clipper->GetOutput();

					// 创建新的映射器和演员
					vtkSmartPointer<vtkPolyDataMapper> clippedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
					clippedMapper->SetInputData(clippedPolyData);

					vtkSmartPointer<vtkActor> clippedActor = vtkSmartPointer<vtkActor>::New();
					clippedActor->SetMapper(clippedMapper);
					clippedActor->GetProperty()->SetColor(actor_->GetProperty()->GetColor());

					// 存储切分后的演员
					clippedActors.push_back(clippedActor);

					// 移除原始演员
					render->RemoveActor(actor_);
				}
			}
		}
		actor = actors->GetNextProp();
	}

	// 将切分后的演员添加到渲染器中
	for (auto& clippedActor : clippedActors)
	{
		render->AddActor(clippedActor);
	}

	// 渲染窗口
	ui->openGLWidget->renderWindow()->Render();
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

void MainWindow::onProcessRunOutput()
{
	while (processRun.canReadLine()) {
		QByteArray output = processRun.readLine();
		ui->textBrowser->append(QString::fromLocal8Bit(output));
		ui->textBrowser->repaint();

		// 解析输出信息并更新图表
		parseOutput(QString::fromLocal8Bit(output));
		ui->tab_2->repaint();
	}
}

void MainWindow::onprocessFoamToVTKOutput()
{
	while (processFoamToVTK.canReadLine()) {
		QByteArray output = processFoamToVTK.readLine();
		ui->textBrowser->append(QString::fromLocal8Bit(output));
		ui->textBrowser->repaint();

		// 解析输出信息并更新图表
		parseOutput(QString::fromLocal8Bit(output));
		ui->tab_2->repaint();
	}
}

void MainWindow::onProcessOutput()
{
	while (process.canReadLine()) {
		QByteArray output = process.readLine();
		ui->textBrowser->append(QString::fromLocal8Bit(output));
		ui->textBrowser->repaint();

		// 解析输出信息并更新图表
		parseOutput(QString::fromLocal8Bit(output));
		ui->tab_2->repaint();
	}
}

void MainWindow::parseOutput(const QString& output)
{
	QRegExp regex("Solving for (\\w+), Initial residual = ([\\d\\.eE\\-]+), Final residual = ([\\d\\.eE\\-]+)");

	if (output.startsWith("Time = ")) {
		currentTimeStep = output.split("=").last().trimmed().toDouble();

		axisMaxX++;
	}

	if (regex.indexIn(output) != -1) {
		QString variable = regex.cap(1);
		double initialResidual = regex.cap(2).toDouble();
		
		// 如果该变量的 QLineSeries 不存在，则创建一个新的
		if (!seriesMap.contains(variable)) {
			QLineSeries* newSeries = new QLineSeries();
			newSeries->setName(variable);
			seriesMap.insert(variable, newSeries);
			chart->addSeries(newSeries);
			chart->createDefaultAxes();
			seriesRangeMap.insert(variable, qMakePair(initialResidual, initialResidual));
		}

		// 更新 QLineSeries
		seriesMap[variable]->append(axisMaxX, initialResidual);

		if (initialResidual < axisMinY) {
			axisMinY /= 10;
		}
		if (initialResidual > axisMaxY) {
			axisMinY *= 10;
		}
	}
}

void MainWindow::updateChart()
{
	chart->axisX()->setTitleText("迭代次数");
	chart->axisX()->setRange(axisMinX, axisMaxX);

	// 确保 axisY 保持为对数坐标系
	QLogValueAxis* logAxisY = new QLogValueAxis();
	logAxisY->setTitleText("残差");
	logAxisY->setBase(10);
	logAxisY->setRange(axisMinY, axisMaxY);
	logAxisY->setLabelFormat("%.1e");
	chart->setAxisY(logAxisY);

	// 设置横坐标为整数
	QValueAxis* axisX = qobject_cast<QValueAxis*>(chart->axisX());
	if (axisX) {
		axisX->setLabelFormat("%d");
	}

	chart->legend()->setAlignment(Qt::AlignRight);
	chart->update();
}

void MainWindow::onProcessError()
{
	//while (process.canReadLine()) {
	//	QByteArray error = process.readLine();
	//	ui->textBrowser->append(QString::fromLocal8Bit(error));
	//	ui->textBrowser->repaint(); 
	//}
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