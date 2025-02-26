#pragma once
#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
	, playTimer(new QTimer(this))
	, reverseTimer(new QTimer(this))
	, loopPlayTimer(new QTimer(this))
	, lastClickedButton(nullptr)
	, processReadOutput(this)
	, processRun(this)
	, processDecomposeMergeMeshes(this)
	, chart(new QChart())
	, axisX(new QValueAxis())
	, axisY(new QLogValueAxis())
	, currentTimeStep(0)
	, chartUpdateTimer(new QTimer(this))
	, residuals()
	, axisMinX(0)
	, axisMaxX(0)
	, axisMinY(0.01)
	, axisMaxY(1)
	, planeRepModelClip(vtkSmartPointer<vtkImplicitPlaneRepresentation>::New())
	, planeWidgetModelClip(vtkSmartPointer<vtkImplicitPlaneWidget2>::New())
{
	//确保当前窗口缩小到任务栏
	setWindowFlags(Qt::Window);
#ifdef _WIN32
	SetWindowLong(HWND(this->winId()), GWL_EXSTYLE, WS_EX_APPWINDOW);
#endif

	//全屏
	this->setWindowState(Qt::WindowMaximized);
	ui->setupUi(this);

	//初始化三维窗口
	renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	render = vtkSmartPointer<vtkRenderer>::New();
	ui->openGLWidget->setRenderWindow(renderWindow);
	renderWindow->AddRenderer(render);
	render->SetBackground(1.0, 1.0, 1.0);
	render->SetBackground2(27 / 255.0, 85 / 255.0, 133 / 255.0);
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
	formGeometry = new FormGeometry(this);
	formMesh = new FormMesh(this);
	formMeshImport = new FormMeshImport(this);
	formSolver = new FormSolver(this);
	formTurbulence = new FormTurbulence(this);
	formPhysicalPropertyParameter = new FormPhysicalPropertyParameter(this);
	formBoundaryConditions = new FormBoundaryConditions(this);
	formRun = new FormRun(this);
	formPostprocessing = new FormPostprocessing(this);
	formModelClip = new FormModelClip(this);
	ui->gridLayout_3->addWidget(formGeometry, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formMesh, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formMeshImport, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formSolver, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formTurbulence, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formPhysicalPropertyParameter, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formBoundaryConditions, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formRun, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formPostprocessing, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formModelClip, 0, 0, 1, 1);
	formGeometry->hide();
	formMesh->hide();
	formMeshImport->hide();
	formSolver->hide();
	formTurbulence->hide();
	formPhysicalPropertyParameter->hide();
	formBoundaryConditions->hide();
	formRun->hide();
	formPostprocessing->hide();
	formModelClip->hide();

	//弹窗
	dialogResultMerge = new DialogResultMerge(this);
	dialogResultMerge->setWindowModality(Qt::WindowModal);

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
	connect(ui->action10, &QAction::triggered, this, &MainWindow::handleAction10Triggered);														//导入案例

	//主界面其他事件处理
	connect(playTimer, &QTimer::timeout, this, &MainWindow::onPlayTimerTimeout);																//播放
	connect(reverseTimer, &QTimer::timeout, this, &MainWindow::onReverseTimerTimeout);															//倒放
	connect(loopPlayTimer, &QTimer::timeout, this, &MainWindow::onLoopPlayTimerTimeout);														//循环播放
	connect(&processRun, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessRunOutput);											//求解计算进程输出
	connect(&processDecomposeMergeMeshes, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessDecomposeMergeMeshes);				//分解合并网格进程输出
	connect(&processRun, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onProcessRunFinished);				//求解计算进程结束
	connect(chartUpdateTimer, &QTimer::timeout, this, &MainWindow::updateChart); 																//更新残差图
	planeRepModelClip->AddObserver(vtkCommand::ModifiedEvent, this, &MainWindow::updatePlaneRepModelClipValues); 				 				//更新模型切分平面选择器的值

	//副控制面板事件处理
	connect(formGeometry, &FormGeometry::geometryImported, this, &MainWindow::formGeometry_import);												//导入几何
	connect(formMeshImport, &FormMeshImport::meshImported, this, &MainWindow::formMeshImport_import);											//导入网格
	connect(formMesh, &FormMesh::meshVisibilityChanged, this, &MainWindow::formMesh_apply);														//网格应用
	connect(formMesh, &FormMesh::itemEntered, this, &MainWindow::formMesh_itemEntered);															//网格页面Item进入
	connect(formMesh, &FormMesh::itemExited, this, &MainWindow::formMesh_itemExited);															//网格页面Item退出
	connect(formSolver, &FormSolver::labelText_8_Changed, formPhysicalPropertyParameter, &FormPhysicalPropertyParameter::solverChanged);		//求解器改变，物性参数控制面板调整
	connect(formRun, &FormRun::run, this, &MainWindow::formRun_run);																			//求解计算
	connect(formRun, &FormRun::stopRun, this, &MainWindow::formRun_stopRun);																	//停止计算
	connect(formPostprocessing, &FormPostprocessing::apply, this, &MainWindow::formPostprocessing_apply);										//更新渲染窗口
	connect(formPostprocessing, &FormPostprocessing::firstFrame, this, &MainWindow::formPostprocessing_firstFrame);								//第一帧
	connect(formPostprocessing, &FormPostprocessing::previousFrame, this, &MainWindow::formPostprocessing_previousFrame);						//上一帧
	connect(formPostprocessing, &FormPostprocessing::reverse, this, &MainWindow::formPostprocessing_reverse);									//重新播放
	connect(formPostprocessing, &FormPostprocessing::play, this, &MainWindow::formPostprocessing_play);											//播放
	connect(formPostprocessing, &FormPostprocessing::nextFrame, this, &MainWindow::formPostprocessing_nextFrame);								//下一帧
	connect(formPostprocessing, &FormPostprocessing::lastFrame, this, &MainWindow::formPostprocessing_lastFrame);								//最后一帧
	connect(formPostprocessing, &FormPostprocessing::loopPlay, this, &MainWindow::formPostprocessing_loopPlay);									//循环播放
	connect(formPostprocessing, &FormPostprocessing::playPause, this, &MainWindow::formPostprocessing_playPause);								//播放暂停
	connect(formPostprocessing, &FormPostprocessing::reversePause, this, &MainWindow::formPostprocessing_reversePause);							//反向播放
	connect(formPostprocessing, &FormPostprocessing::loopPlayPause, this, &MainWindow::formPostprocessing_loopPlayPause);						//循环播放
	connect(formPostprocessing, &FormPostprocessing::loadData, this, &MainWindow::formPostprocessing_loadData);									//加载数据
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
	formGeometry->hide();
	formMesh->hide();
	formMeshImport->hide();
	formSolver->hide();
	formTurbulence->hide();
	formPhysicalPropertyParameter->hide();
	formBoundaryConditions->hide();
	formRun->hide();
	formPostprocessing->hide();
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

void MainWindow::handleAction10Triggered()
{
	QString caseFilePath;
	QFileDialog dialog(this, tr(""), "", tr("OpenFOAM 文件 (*.foam);;"));
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setViewMode(QFileDialog::Detail);

	dialog.setWindowModality(Qt::WindowModal);
	dialog.setWindowFlags(dialog.windowFlags() | Qt::WindowStaysOnTopHint);

	if (dialog.exec() == QDialog::Accepted) {
		caseFilePath = dialog.selectedFiles().first();
		if (caseFilePath.isEmpty()) return;
		GlobalData::getInstance().clearAllData();
		ui->textBrowser->append("Load case：" + caseFilePath);

		//更新网格导入页面
		formMeshImport_import(caseFilePath);

		//更新后处理数据页面(需补充)
		updatePostProcessingPage(caseFilePath);

		//更新参数配置页面(需补充)
		//formSolver->importParameter();
		//formTurbulence->importParameter();
		//formPhysicalPropertyParameter->importParameter();
		////formBoundaryConditions->importParameter();
		formRun->importParameter();

		GlobalData::getInstance().getCaseData()->casePath = caseFilePath.toStdString();
		ui->textBrowser->append("Load case successfully!");
		QMessageBox::information(this, "提示", "案例导入成功");
	}
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

void MainWindow::on_pushButton_5_clicked()
{
	hideAllSubForm();
	formSolver->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::on_pushButton_6_clicked()
{
	hideAllSubForm();
	formTurbulence->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::on_pushButton_7_clicked()
{
	hideAllSubForm();
	formPhysicalPropertyParameter->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::on_pushButton_13_clicked()
{
	hideAllSubForm();
	formBoundaryConditions->show();
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

void MainWindow::on_pushButton_18_clicked()
{
	dialogResultMerge->show();
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
	render->RemoveAllViewProps();
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

vtkSmartPointer<vtkActor> MainWindow::createMeshPatchActor(
	const std::string& casePath,
	const std::vector<std::string>& patchGroup)
{
	// 创建 OpenFOAM 读取器
	vtkSmartPointer<vtkOpenFOAMReader> openFOAMReader = vtkSmartPointer<vtkOpenFOAMReader>::New();
	openFOAMReader->SetFileName(casePath.c_str());
	openFOAMReader->SetCreateCellToPoint(1);
	openFOAMReader->SetSkipZeroTime(1);

	// 更新信息以获取补丁名称
	openFOAMReader->UpdateInformation();
	openFOAMReader->DisableAllPatchArrays();

	// 针对patchGroup启用对应的补丁
	for (const auto& patchName : patchGroup)
	{
		if (patchName == "internalMesh")
		{
			openFOAMReader->SetPatchArrayStatus("internalMesh", 1);
		}
		else
		{
			std::string fullPatchName = "patch/" + patchName;
			openFOAMReader->SetPatchArrayStatus(fullPatchName.c_str(), 1);
		}
	}

	// 更新读取器
	openFOAMReader->Update();

	// 使用 vtkCompositeDataGeometryFilter 提取单一几何
	vtkSmartPointer<vtkCompositeDataGeometryFilter> geometryFilter =
		vtkSmartPointer<vtkCompositeDataGeometryFilter>::New();
	geometryFilter->SetInputConnection(openFOAMReader->GetOutputPort());
	geometryFilter->Update();

	vtkPolyData* polyData = geometryFilter->GetOutput();
	if (!polyData || polyData->GetNumberOfPoints() == 0)
	{
		return nullptr;
	}

	// 创建映射器
	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputData(polyData);
	mapper->ScalarVisibilityOff();

	// 创建单一Actor
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0.0, 221.0 / 255.0, 221.0 / 255.0);
	actor->GetProperty()->EdgeVisibilityOn();
	actor->GetProperty()->SetEdgeColor(0.0, 0.0, 0.0);
	actor->GetProperty()->SetRepresentationToSurface();

	return actor;
}

void MainWindow::getMeshPatchData(const std::string& casePath)
{
	std::vector<std::string> meshPatchNames;

	// 创建 OpenFOAM 读取器
	vtkSmartPointer<vtkOpenFOAMReader> openFOAMReader =
		vtkSmartPointer<vtkOpenFOAMReader>::New();
	openFOAMReader->SetFileName(casePath.c_str());
	openFOAMReader->SetCreateCellToPoint(1);
	openFOAMReader->SetSkipZeroTime(1);

	// 更新信息以获取补丁名称
	openFOAMReader->UpdateInformation();

	// 获取所有补丁名称
	int numPatches = openFOAMReader->GetNumberOfPatchArrays();
	if (numPatches == 0)
	{
		std::cerr << "没有找到任何补丁。" << std::endl;
		return;
	}

	// 禁用所有补丁
	openFOAMReader->DisableAllPatchArrays();

	// 遍历所有补丁并添加进meshPatchNames
	for (int i = 0; i < numPatches; ++i)
	{
		const char* currentPatchName = openFOAMReader->GetPatchArrayName(i);
		if (std::string(currentPatchName).find("patch") == 0 || strcmp(currentPatchName, "internalMesh") == 0)
		{
			if (std::string(currentPatchName).find("patch/") != std::string::npos)
			{
				meshPatchNames.push_back(std::string(currentPatchName).substr(6));
			}
			else
			{
				meshPatchNames.push_back(std::string(currentPatchName));
			}

		}
	}
	GlobalData::getInstance().getCaseData()->meshPatchNames = meshPatchNames;
}

void MainWindow::formMeshImport_import(const QString& filePath)
{
	render->RemoveAllViewProps();
	QFileInfo fileInfo(filePath);
	if (!fileInfo.exists())
	{
		QMessageBox::warning(this, tr("错误"), tr("文件不存在"));
		return;
	}

	QString type = fileInfo.suffix().toLower();
	if (GlobalData::getInstance().getCaseData()->casePath != filePath.toStdString()) {
		render->RemoveAllViewProps();
		GlobalData::getInstance().clearAllData();
		GlobalData::getInstance().getCaseData()->casePath = filePath.toStdString();
	}

	if (type == "foam")
	{
		getMeshPatchData(filePath.toStdString());
		GlobalData::getInstance().getCaseData()->casePath = filePath.toStdString();
		formMesh->updateForm();
		render->ResetCamera();
		renderWindow->Render();

		//网格导入成功之后，自动跳转到网格页面
		on_pushButton_2_clicked();
		ui->pushButton_2->setStyleSheet("QPushButton { background-color: rgb(232, 232, 232); border: none; text-align: left; padding-left: 50px; }");
		lastClickedButton->setStyleSheet("QPushButton { background-color: rgb(255, 255, 255); border: none; text-align: left; padding-left: 50px; } QPushButton:hover { background-color: rgb(242, 242, 242); }");
		lastClickedButton = ui->pushButton_2;

		//网格导入成功,初始化参数配置页面(需补充)
		//formBoundaryConditions->onMeshImported();
	}
}

void MainWindow::formMesh_apply()
{
	// 移除所有已添加的演员
	render->RemoveAllViewProps();

	// 遍历 listViewModel 并根据选中状态添加到
	std::vector<std::string> patchGroup;
	for (int i = 0; i < formMesh->listViewModel->rowCount(); ++i)
	{
		QStandardItem* item = formMesh->listViewModel->item(i);
		if (item->checkState() == Qt::Checked)
		{
			patchGroup.push_back(item->text().toStdString());
		}
	}

	// 创建 meshPatchActor
	vtkSmartPointer<vtkActor> meshPatchActor = createMeshPatchActor(GlobalData::getInstance().getCaseData()->casePath, patchGroup);
	if (meshPatchActor)
	{
		render->AddActor(meshPatchActor);
		renderWindow->Render();
	}
}

void MainWindow::formMesh_itemEntered(const QString& text)
{
	//const auto& meshPatchActors = GlobalData::getInstance().getCaseData()->meshPatchActors;
	//// 将 QString 转换为 std::string
	//std::string key = text.toStdString();
	//auto actorIt = meshPatchActors.find(key);
	//if (actorIt != meshPatchActors.end())
	//{
	//	actorIt->second->GetProperty()->SetColor(204.0 / 255.0, 103.0 / 255.0, 103.0 / 255.0);
	//	renderWindow->Render();
	//}
}

void MainWindow::formMesh_itemExited(const QString& text)
{
	//const auto& meshPatchActors = GlobalData::getInstance().getCaseData()->meshPatchActors;
	//// 将 QString 转换为 std::string
	//std::string key = text.toStdString();
	//auto actorIt = meshPatchActors.find(key);
	//if (actorIt != meshPatchActors.end())
	//{
	//	actorIt->second->GetProperty()->SetColor(0.0, 221.0 / 255.0, 221.0 / 255.0);
	//	renderWindow->Render();
	//}
}

void MainWindow::formRun_run()
{
	if (GlobalData::getInstance().getCaseData()->casePath.empty())
	{
		QMessageBox::warning(this, tr("错误"), tr("请先导入网格"));
		return;
	}
	//保存界面上所有的配置参数，并校验是否符合要求(需补充)
	//formSolver->exportParameter();
	//formTurbulence->exportParameter();
	//formPhysicalPropertyParameter->exportParameter();
	////formBoundaryConditions->exportParameter();
	formRun->exportParameter();

	//隐藏开始按钮，显示停止按钮
	QThread::msleep(500);
	formRun->ui->pushButton->hide();
	formRun->ui->pushButton_2->show();
	formRun->ui->label_12->show();

	//初始化残差图数据
	residuals.clear();
	seriesMap.clear();
	seriesRangeMap.clear();
	currentTimeStep = 0;
	chart->removeAllSeries();
	formPostprocessing->ui->comboBox->clear();
	formPostprocessing->ui->comboBox_2->clear();
	axisMaxX = 0;
	ui->tabWidget->setCurrentIndex(1);

	//获取案例路径
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QFileInfo fileInfo(casePath);
	QString caseDir = fileInfo.path();
	QString controlDictPath = caseDir + "/system/controlDict";

	// 删除原来的计算结果
	if (formRun->ui->checkBox->isChecked())
	{
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

	//串行计算
	if (formRun->ui->radioButton->isChecked())
	{
		QString command = application + " -case " + caseDir;
		processRun.setProgram("cmd.exe");
		processRun.setArguments(QStringList() << "/C" << command);
		processRun.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments* args) {
			args->flags |= CREATE_NO_WINDOW;
			});
		processRun.start();
	}
	//并行计算
	else
	{
		//分解网格
		QDir dir(caseDir);
		QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
		foreach(QString folder, folders)
		{
			if (folder.startsWith("processor"))
			{
				QDir folderDir(caseDir + "/" + folder);
				folderDir.removeRecursively();
			}
		}
		QString commandDecomposePar = "decomposePar -case " + caseDir;
		processDecomposeMergeMeshes.setProgram("cmd.exe");
		processDecomposeMergeMeshes.setArguments(QStringList() << "/C" << commandDecomposePar);
		processDecomposeMergeMeshes.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments* args) {
			args->flags |= CREATE_NO_WINDOW;
			});
		processDecomposeMergeMeshes.start();
		processDecomposeMergeMeshes.waitForFinished(-1);

		//计算
		QString commandRun = "mpiexec -np " + QString::number(formRun->ui->spinBox->value()) + " " + application + " -parallel -case " + caseDir;
		processRun	.setProgram("cmd.exe");
		processRun.setArguments(QStringList() << "/C" << commandRun);
		processRun.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments* args) {
			args->flags |= CREATE_NO_WINDOW;
			});
		processRun.start();
	}
}

void MainWindow::onProcessRunFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitCode);
	Q_UNUSED(exitStatus);
	formRun->on_pushButton_clicked_2();

	//获取案例路径
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QFileInfo fileInfo(casePath);
	QString caseDir = fileInfo.path();

	if (formRun->ui->radioButton_2->isChecked()) {
		QString comandReconstructPar = "reconstructPar -case " + caseDir;
		processDecomposeMergeMeshes.setProgram("cmd.exe");
		processDecomposeMergeMeshes.setArguments(QStringList() << "/C" << comandReconstructPar);
		processDecomposeMergeMeshes.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments* args) {
			args->flags |= CREATE_NO_WINDOW;
			});
		processDecomposeMergeMeshes.start();
		processDecomposeMergeMeshes.waitForFinished(-1);
	}

	QString caseFilePath = QString::fromStdString(GlobalData::getInstance().getCaseData()->casePath);
	updatePostProcessingPage(caseFilePath);
}

void MainWindow::formRun_stopRun()
{
	QThread::msleep(500);
	formRun->ui->pushButton_2->hide();
	formRun->ui->label_12->hide();
	formRun->ui->pushButton->show();

	if (formRun->ui->radioButton->isChecked()==true) {
		if(processRun.state() == QProcess::Running) processRun.kill();
	}

	if (formRun->ui->radioButton_2->isChecked() == true)
	{
		if (processRun.state() == QProcess::Running)
		{
			QString killCommand = QString("taskkill /PID %1 /F /T").arg(processRun.processId());
			QProcess::execute(killCommand);
		}
	}
}

void MainWindow::formPostprocessing_apply()
{
	//更新渲染窗口
	render->RemoveAllViewProps();
	string casePath = GlobalData::getInstance().getCaseData()->casePath;
	double timeValue = formPostprocessing->ui->comboBox->currentText().toDouble();
	std::string fieldNameValue = formPostprocessing->ui->comboBox_2->currentText().toStdString();
	std::vector<std::string> patchGroup;
	for (int i = 0; i < formPostprocessing->listViewModel->rowCount(); ++i) {
		QStandardItem* item = formPostprocessing->listViewModel->item(i);
		if (item->checkState() == Qt::Checked) {
			if (item->text().toStdString() == "internalMesh") patchGroup.push_back(item->text().toStdString());
			else patchGroup.push_back("patch/" + item->text().toStdString());
		}
	}
	std::pair<double, double> globalRange = GlobalData::getInstance().getCaseData()->fieldsScalarRange[fieldNameValue];
	vtkSmartPointer<vtkActor> actor = createNephogramPatchActor(casePath, timeValue, fieldNameValue, patchGroup, globalRange);

	if (actor)
	{
		render->AddActor(actor);

		// 创建颜色传输函数
		vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
		colorTransferFunction->SetColorSpaceToRGB();

		// 添加颜色点
		colorTransferFunction->AddRGBPoint(globalRange.first, 0 / 255.0, 127 / 255.0, 255 / 255.0); // 蓝色
		colorTransferFunction->AddRGBPoint((globalRange.first + globalRange.second) / 2.0, 234.0 / 255.0, 213.0 / 255.0, 201.0 / 255.0); // 白色
		colorTransferFunction->AddRGBPoint(globalRange.second, 180.0 / 255.0, 0 / 255.0, 0 / 255.0); // 红色

		// 创建图例
		vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
		scalarBar->SetLookupTable(colorTransferFunction);
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
		labelTextProperty->SetColor(0, 0, 0); // 设置标签颜色为黑色
		scalarBar->SetLabelTextProperty(labelTextProperty);

		// 添加图例到渲染器
		render->AddActor2D(scalarBar);
		renderWindow->Render();
	}
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

void MainWindow::formPostprocessing_loadData()
{
	QString caseFilePath;
	QFileDialog dialog(this, tr(""), "", tr("OpenFOAM 文件 (*.foam);;"));
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setViewMode(QFileDialog::Detail);

	dialog.setWindowModality(Qt::WindowModal);
	dialog.setWindowFlags(dialog.windowFlags() | Qt::WindowStaysOnTopHint);

	if (dialog.exec() == QDialog::Accepted) {
		caseFilePath = dialog.selectedFiles().first();
		if (caseFilePath.isEmpty()) return;
		GlobalData::getInstance().clearAllData();
		GlobalData::getInstance().getCaseData()->casePath = caseFilePath.toStdString();
		updatePostProcessingPage(caseFilePath);
	}
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
	formModelClip->ui->checkBox->setChecked(true);

	// 获取平面选择器的原点和法向量
	double origin[3];
	double normal[3];
	planeRepModelClip->GetOrigin(origin);
	planeRepModelClip->GetNormal(normal);

	// 创建一个平面
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetOrigin(origin);
	plane->SetNormal(normal);

	// 获取当前选中的活动标量
	QString activeScalar = this->formPostprocessing->ui->comboBox_2->currentText();

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

			if (polyDataMapper)
			{
				vtkSmartPointer<vtkPolyData> polyData = vtkPolyData::SafeDownCast(polyDataMapper->GetInput());
				vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkColorTransferFunction::SafeDownCast(polyDataMapper->GetLookupTable());
				double range[2] = { 0.0, 1.0 };
				polyDataMapper->GetScalarRange(range);

				if (polyData)
				{
					// 使用平面切分多边形数据
					vtkSmartPointer<vtkClipPolyData> clipper = vtkSmartPointer<vtkClipPolyData>::New();
					clipper->SetInputData(polyData);
					clipper->SetClipFunction(plane);
					clipper->SetInsideOut(!formModelClip->ui->checkBox_2->isChecked());
					clipper->Update();

					// 获取切分后的数据
					vtkSmartPointer<vtkPolyData> clippedPolyData = clipper->GetOutput();

					// 创建新的映射器和演员
					vtkSmartPointer<vtkPolyDataMapper> clippedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
					clippedMapper->SetInputData(clippedPolyData);
					clippedMapper->SetLookupTable(colorTransferFunction);
					clippedMapper->SetScalarRange(range);

					vtkSmartPointer<vtkActor> clippedActor = vtkSmartPointer<vtkActor>::New();
					clippedActor->SetMapper(clippedMapper);
					clippedActor->GetProperty()->SetColor(actor_->GetProperty()->GetColor());

					// 存储切分后的演员
					clippedActors.push_back(clippedActor);

					// 移除原始演员
					render->RemoveActor(actor_);
				}
			}
			else if (dataSetMapper)
			{
				vtkSmartPointer<vtkDataSet> dataSet = vtkDataSet::SafeDownCast(dataSetMapper->GetInput());
				vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkColorTransferFunction::SafeDownCast(dataSetMapper->GetLookupTable());
				double range[2] = { 0.0, 1.0 };
				dataSetMapper->GetScalarRange(range);

				if (dataSet)
				{
					// 使用 vtkClipDataSet 对原始数据进行剪裁
					vtkSmartPointer<vtkClipDataSet> clipper = vtkSmartPointer<vtkClipDataSet>::New();
					clipper->SetInputData(dataSet);
					clipper->SetClipFunction(plane);
					clipper->SetInsideOut(!formModelClip->ui->checkBox_2->isChecked());
					clipper->Update();

					// 提取剪裁后的表面
					vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
					surfaceFilter->SetInputConnection(clipper->GetOutputPort());
					surfaceFilter->Update();

					vtkSmartPointer<vtkPolyData> cutSurface = surfaceFilter->GetOutput();

					// 检查结果是否有效
					if (cutSurface->GetNumberOfPoints() > 0)
					{
						// 设置活动标量为当前选中的物理量
						if (cutSurface->GetPointData()->HasArray(activeScalar.toStdString().c_str()))
						{
							cutSurface->GetPointData()->SetActiveScalars(activeScalar.toStdString().c_str());
						}
						else
						{
							QMessageBox::warning(this, "警告", "剪裁后的数据不包含标量 '" + activeScalar + "'。");
							continue;
						}

						// 获取标量范围（与原始数据一致）
						double scalarRange[2];
						cutSurface->GetPointData()->GetArray(activeScalar.toStdString().c_str())->GetRange(scalarRange);

						// 创建映射器和演员（切分面）
						vtkSmartPointer<vtkPolyDataMapper> cutMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
						cutMapper->SetInputData(cutSurface);
						cutMapper->SetLookupTable(colorTransferFunction);
						cutMapper->SetScalarRange(scalarRange);
						cutMapper->SetScalarModeToUsePointData();

						vtkSmartPointer<vtkActor> cutActor = vtkSmartPointer<vtkActor>::New();
						cutActor->SetMapper(cutMapper);

						// 设置切分面演员属性
						cutActor->GetProperty()->SetOpacity(1.0);
						cutActor->GetProperty()->EdgeVisibilityOff();

						// 存储切分后的演员
						clippedActors.push_back(cutActor);
					}

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

		// 解析输出信息
		parseOutput(QString::fromLocal8Bit(output));
		ui->tab_2->repaint();
	}
}

void MainWindow::onProcessDecomposeMergeMeshes()
{
	while (processDecomposeMergeMeshes.canReadLine()) {
		QByteArray output = processDecomposeMergeMeshes.readLine();
		ui->textBrowser->append(QString::fromLocal8Bit(output));
		ui->textBrowser->repaint();

		// 解析输出信息
		parseOutput(QString::fromLocal8Bit(output));
		ui->tab_2->repaint();
	}
}

void MainWindow::parseOutput(const QString& output)
{
	QRegExp regex("Solving for (\\w+), Initial residual = ([\\d\\.eE\\-]+), Final residual = ([\\d\\.eE\\-]+)");
	if (output.startsWith("Time = ")) {
		// 遇到新的时间步，更新 QLineSeries
		for (auto it = residuals.begin(); it != residuals.end(); ++it) {
			const QString& variable = it.key();
			double initialResidual = it.value();

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

		// 清空当前时间步的残差数据
		residuals.clear();

		// 更新时间步
		currentTimeStep = output.split("=").last().trimmed().toDouble();
		axisMaxX++;
	}

	if (regex.indexIn(output) != -1) {
		QString variable = regex.cap(1);
		double initialResidual = regex.cap(2).toDouble();

		// 存储当前时间步的残差数据
		residuals[variable] = initialResidual;
	}
}

void MainWindow::getNephogramPatchData(
	const std::string& casePath)
{
	std::map<std::string, std::pair<double, double>> result;
	std::vector<double> timeSteps;
	std::vector<std::string> fieldName;
	std::vector<std::string> patchGroup;

	vtkSmartPointer<vtkOpenFOAMReader> reader = vtkSmartPointer<vtkOpenFOAMReader>::New();
	reader->SetFileName(casePath.c_str());
	reader->SetCreateCellToPoint(1);
	reader->SetSkipZeroTime(1);
	reader->UpdateInformation();

	vtkSmartPointer<vtkDoubleArray> timeValues = reader->GetTimeValues();
	if (!timeValues || timeValues->GetNumberOfValues() == 0) {
		std::cerr << "没有找到可用的时间步。" << std::endl;
		return;
	}

	for (int i = 0; i < timeValues->GetNumberOfValues(); ++i) {
		timeSteps.push_back(timeValues->GetValue(i));
	}

	double lastTime = timeSteps.back();
	reader->SetTimeValue(lastTime);

	//获取填充patchGroup
	int numPatches = reader->GetNumberOfPatchArrays();
	for (int i = 0; i < numPatches; ++i) {
		const char* patchName = reader->GetPatchArrayName(i);
		if (std::string(patchName).find("patch/") == 0) {
			patchName = patchName + 6;
			patchGroup.push_back(patchName);
		}

		if (strcmp(patchName, "internalMesh") == 0) {
			patchGroup.push_back(patchName);
		}
	}

	reader->DisableAllPatchArrays();
	reader->EnableAllPatchArrays();
	reader->Update();

	vtkSmartPointer<vtkCompositeDataGeometryFilter> geometryFilter =
		vtkSmartPointer<vtkCompositeDataGeometryFilter>::New();
	geometryFilter->SetInputConnection(reader->GetOutputPort());
	geometryFilter->Update();

	vtkPolyData* polyData = geometryFilter->GetOutput();
	if (!polyData || polyData->GetNumberOfPoints() == 0) {
		std::cerr << "无法提取几何数据。" << std::endl;
		return;
	}

	// 收集标量数组的范围
	int arrayCount = polyData->GetPointData()->GetNumberOfArrays();
	for (int i = 0; i < arrayCount; ++i) {
		vtkDataArray* arr = polyData->GetPointData()->GetArray(i);
		if (!arr) continue;
		std::string arrayName = arr->GetName() ? arr->GetName() : "";
		if (arrayName != "p" && arrayName != "U" && arrayName != "T")continue;

		double range[2];
		arr->GetRange(range);
		result[arrayName] = std::make_pair(range[0], range[1]);
		fieldName.push_back(arrayName);
	}

	GlobalData::getInstance().getCaseData()->timeSteps = timeSteps;
	GlobalData::getInstance().getCaseData()->fieldName = fieldName;
	GlobalData::getInstance().getCaseData()->meshPatchNames = patchGroup;
	GlobalData::getInstance().getCaseData()->fieldsScalarRange = result;
}

vtkSmartPointer<vtkActor> MainWindow::createNephogramPatchActor(
	const std::string& casePath,
	double timeValue,
	const std::string& fieldName,
	const std::vector<std::string>& patchGroup,
	const std::pair<double, double>& globalRange)
{
	// 创建 OpenFOAM 读取器
	vtkSmartPointer<vtkOpenFOAMReader> openFOAMReader =
		vtkSmartPointer<vtkOpenFOAMReader>::New();
	openFOAMReader->SetFileName(casePath.c_str());
	openFOAMReader->SetCreateCellToPoint(1);
	openFOAMReader->SetSkipZeroTime(1);
	openFOAMReader->SetTimeValue(timeValue);
	openFOAMReader->UpdateInformation();

	// 获取并禁用全部补丁
	int numPatches = openFOAMReader->GetNumberOfPatchArrays();
	openFOAMReader->DisableAllPatchArrays();
	// 启用指定补丁
	for (int i = 0; i < numPatches; ++i) {
		const char* currentPatchName = openFOAMReader->GetPatchArrayName(i);
		for (const auto& groupPatch : patchGroup) {
			if (groupPatch == currentPatchName) {
				openFOAMReader->SetPatchArrayStatus(currentPatchName, 1);
				break;
			}
		}
	}

	// 设置UPDATE_TIME_STEP
	vtkInformation* outInfo = openFOAMReader->GetOutputInformation(0);
	outInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEP(), timeValue);

	// 更新读取器
	openFOAMReader->Update();

	// 使用 vtkCompositeDataGeometryFilter 提取几何数据
	vtkSmartPointer<vtkCompositeDataGeometryFilter> geometryFilter =
		vtkSmartPointer<vtkCompositeDataGeometryFilter>::New();
	geometryFilter->SetInputConnection(openFOAMReader->GetOutputPort());
	geometryFilter->Update();

	vtkPolyData* polyData = geometryFilter->GetOutput();
	if (!polyData || polyData->GetNumberOfPoints() == 0) {
		std::cerr << "无法提取补丁的几何数据。" << std::endl;
		return nullptr;
	}

	// 设置活动标量
	if (!polyData->GetPointData()->HasArray(fieldName.c_str())) {
		std::cerr << "指定的物理量不存在：" << fieldName << std::endl;
		return nullptr;
	}
	polyData->GetPointData()->SetActiveScalars(fieldName.c_str());

	// 创建颜色传输函数（使用传入的globalRange）
	vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction =
		vtkSmartPointer<vtkColorTransferFunction>::New();
	colorTransferFunction->SetColorSpaceToRGB();
	// 添加颜色点（蓝-白-红）
	colorTransferFunction->AddRGBPoint(globalRange.first, 0.0, 127.0 / 255.0, 1.0);
	colorTransferFunction->AddRGBPoint(
		(globalRange.first + globalRange.second) / 2.0,
		234.0 / 255.0, 213.0 / 255.0, 201.0 / 255.0);
	colorTransferFunction->AddRGBPoint(globalRange.second, 180.0 / 255.0, 0.0, 0.0);

	// 创建映射器，使用自定义范围
	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputData(polyData);
	mapper->SetLookupTable(colorTransferFunction);
	mapper->UseLookupTableScalarRangeOn();
	mapper->SetScalarRange(globalRange.first, globalRange.second);
	mapper->ScalarVisibilityOn();

	// 创建actor
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOff();
	actor->GetProperty()->SetRepresentationToSurface();

	return actor;
}

void MainWindow::updatePostProcessingPage(const QString& casePath)
{
	//需要清除之前案例信息（需补充）
	if (GlobalData::getInstance().getCaseData()->casePath != casePath.toStdString()) {
		GlobalData::getInstance().clearAllData();
		GlobalData::getInstance().getCaseData()->casePath = casePath.toStdString();
	}

	getNephogramPatchData(casePath.toStdString());
	formPostprocessing->ui->comboBox->clear();
	formPostprocessing->ui->comboBox_2->clear();
	formPostprocessing->listViewModel->clear();

	// 更新时间步和物理量下拉框
	QStringList timeStepList;
	std::vector<double> timeSteps = GlobalData::getInstance().getCaseData()->timeSteps;
	for (const double& timeStep : timeSteps) timeStepList.append(QString::number(timeStep));
	if (timeStepList.size() == 0) return;
	formPostprocessing->ui->comboBox->addItems(timeStepList);

	// 更新物理量下拉框
	QStringList fieldNameList;
	std::vector<std::string> fieldName = GlobalData::getInstance().getCaseData()->fieldName;
	for (const std::string& field : fieldName) fieldNameList.append(QString::fromStdString(field));
	if (fieldNameList.size() == 0) return;
	formPostprocessing->ui->comboBox_2->addItems(fieldNameList);

	//更新补丁下列表
	const std::vector<std::string>& meshPatchNames = GlobalData::getInstance().getCaseData()->meshPatchNames;
	if (meshPatchNames.size() == 0) return;
	for (const std::string& patchName : meshPatchNames) {
		QStandardItem* item = new QStandardItem(QString::fromStdString(patchName));
		item->setCheckable(true);
		if (patchName != "internalMesh") item->setCheckState(Qt::Checked);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		item->setSizeHint(QSize(0, 40));
		formPostprocessing->listViewModel->appendRow(item);
	}

	// 计算所有 item 的总高度
	int totalHeight = 0;
	for (int i = 0; i < formPostprocessing->listViewModel->rowCount(); ++i) {
		totalHeight += formPostprocessing->ui->listView->sizeHintForRow(i);
	}
	formPostprocessing->ui->listView->setFixedHeight(totalHeight + 2 * formPostprocessing->ui->listView->frameWidth());

	//更新渲染窗口
	render->RemoveAllViewProps();
	double timeValue = formPostprocessing->ui->comboBox->currentText().toDouble();
	std::string fieldNameValue = formPostprocessing->ui->comboBox_2->currentText().toStdString();
	std::vector<std::string> patchGroup;
	for (int i = 0; i < formPostprocessing->listViewModel->rowCount(); ++i) {
		QStandardItem* item = formPostprocessing->listViewModel->item(i);
		if (item->checkState() == Qt::Checked) {
			if (item->text().toStdString() == "internalMesh") patchGroup.push_back(item->text().toStdString());
			else patchGroup.push_back("patch/" + item->text().toStdString());
		}
	}
	std::pair<double, double> globalRange = GlobalData::getInstance().getCaseData()->fieldsScalarRange[fieldNameValue];
	vtkSmartPointer<vtkActor> actor = createNephogramPatchActor(casePath.toStdString(), timeValue, fieldNameValue, patchGroup, globalRange);

	if (actor)
	{
		render->AddActor(actor);

		// 创建颜色传输函数
		vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
		colorTransferFunction->SetColorSpaceToRGB();

		// 添加颜色点
		colorTransferFunction->AddRGBPoint(globalRange.first, 0 / 255.0, 127 / 255.0, 255 / 255.0); // 蓝色
		colorTransferFunction->AddRGBPoint((globalRange.first + globalRange.second) / 2.0, 234.0 / 255.0, 213.0 / 255.0, 201.0 / 255.0); // 白色
		colorTransferFunction->AddRGBPoint(globalRange.second, 180.0 / 255.0, 0 / 255.0, 0 / 255.0); // 红色

		// 创建图例
		vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
		scalarBar->SetLookupTable(colorTransferFunction);
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
		labelTextProperty->SetColor(0, 0, 0); // 设置标签颜色为黑色
		scalarBar->SetLabelTextProperty(labelTextProperty);

		// 添加图例到渲染器
		render->AddActor2D(scalarBar);

		render->ResetCamera();
		renderWindow->Render();
	}

	//切换到后处理子页面
	on_pushButton_17_clicked();
	ui->pushButton_17->setStyleSheet("QPushButton { background-color: rgb(232, 232, 232); border: none; text-align: left; padding-left: 50px; }");
	lastClickedButton->setStyleSheet("QPushButton { background-color: rgb(255, 255, 255); border: none; text-align: left; padding-left: 50px; } QPushButton:hover { background-color: rgb(242, 242, 242); }");
	lastClickedButton = ui->pushButton_17;
}

void MainWindow::updateChart()
{
	chart->axisX()->setTitleText("迭代次数");
	chart->axisX()->setRange(axisMinX, axisMaxX - 1);

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