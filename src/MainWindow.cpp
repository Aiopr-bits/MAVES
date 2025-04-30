/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
	, playTimer(new QTimer(this))
	, reverseTimer(new QTimer(this))
	, loopPlayTimer(new QTimer(this))
	, lastClickedButton(nullptr)
	, processRun(this)
	, processDecomposePar(this)
	, processReconstructPar(this)
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
	, previousTabWidgetIndex(0)
	, m_currentFrame(0)
{
	ui->setupUi(this);
	initialize();

	//全屏
	this->setWindowState(Qt::WindowMaximized);

	//确保当前窗口缩小到任务栏
	setWindowFlags(Qt::Window);
#ifdef _WIN32
	SetWindowLong(HWND(this->winId()), GWL_EXSTYLE, WS_EX_APPWINDOW);
#endif

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
		ui->pushButton_16, ui->pushButton_17, ui->pushButton_18, ui->pushButton_19, ui->pushButton_20,
		ui->pushButton_21, ui->pushButton_22
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
	formRadiation = new FormRadiation(this);
	formTurbulence = new FormTurbulence(this);
	formThermo = new FormThermo(this);
	formTransportProperties = new FormTransportProperties(this);
	formCellZones = new FormCellZones(this);
	formBoundaryConditions = new FormBoundaryConditions(this);
	formInitialConditions = new FormInitialConditions(this);
	formRun = new FormRun(this);
	formPostprocessing = new FormPostprocessing(this);
	formModelClip = new FormModelClip(this);
	ui->gridLayout_3->addWidget(formGeometry, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formMesh, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formMeshImport, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formSolver, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formRadiation, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formTurbulence, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formThermo, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formTransportProperties, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formCellZones, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formBoundaryConditions, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formInitialConditions, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formRun, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formPostprocessing, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formModelClip, 0, 0, 1, 1);
	formGeometry->hide();
	formMesh->hide();
	formMeshImport->hide();
	formSolver->hide();
	formRadiation->hide();
	formTurbulence->hide();
	formThermo->hide();
	formTransportProperties->hide();
	formCellZones->hide();
	formBoundaryConditions->hide();
	formInitialConditions->hide();
	formRun->hide();
	formPostprocessing->hide();
	formModelClip->hide();

	//弹窗
	dialogResultMerge = new DialogResultMerge(this);
	dialogResultMerge->setWindowModality(Qt::WindowModal);

	dialogInformationPrompt = new DialogInformationPrompt(this);
	dialogInformationPrompt->setWindowModality(Qt::WindowModal);

	//程序启动点击几何页面
	on_pushButton_clicked();
	ui->pushButton->setStyleSheet("QPushButton { background-color: rgb(232, 232, 232); border: none; text-align: left; padding-left: 50px; }");
	lastClickedButton = ui->pushButton;

	//对滚动条进行polish抛光
	ui->textBrowser->verticalScrollBar()->style()->polish(ui->textBrowser->verticalScrollBar());
	ui->scrollArea->verticalScrollBar()->style()->polish(ui->scrollArea->verticalScrollBar());
	ui->scrollArea->horizontalScrollBar()->style()->polish(ui->scrollArea->horizontalScrollBar());

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

	//初始化previousPanelButton	
	previousPanelButton = "几何";

	//初始化tabWidget控件可见性
	ui->pushButton_23->hide();	//网格加密
	ui->pushButton_24->hide();	//翼型
	ui->pushButton_22->hide();	//辐射
	ui->pushButton_6->hide();	//湍流
	ui->pushButton_25->hide();	//动态网格
	ui->pushButton_26->hide();	//离散项
	ui->pushButton_27->hide();	//燃烧
	ui->pushButton_7->hide();	//热
	ui->pushButton_28->hide();	//弹性模量
	ui->pushButton_21->hide();	//传输特性
	ui->pushButton_8->hide();	//离散化
	ui->pushButton_9->hide();	//线性求解器
	ui->pushButton_10->hide();	//被动标量
	ui->pushButton_11->hide();	//参考值
	ui->pushButton_12->hide();	//区域设置
	ui->pushButton_13->hide();	//边界条件
	ui->pushButton_14->hide();	//初始条件
	ui->pushButton_15->hide();	//监视器
	ui->line_5->hide();
	ui->pushButton_16->hide();	//求解计算
	ui->line_4->hide();
	ui->pushButton_17->hide();	//后处理
	ui->pushButton_3->hide();	//模型切分
	ui->pushButton_18->hide();	//计算

	// 工具栏信号处理
	connect(ui->action1, &QAction::triggered, this, &MainWindow::handleAction1Triggered);																//信息框
	connect(ui->action2, &QAction::triggered, this, &MainWindow::handleAction2Triggered);																//x正向
	connect(ui->action3, &QAction::triggered, this, &MainWindow::handleAction3Triggered);																//x负向
	connect(ui->action4, &QAction::triggered, this, &MainWindow::handleAction4Triggered);																//y正向
	connect(ui->action5, &QAction::triggered, this, &MainWindow::handleAction5Triggered);																//y负向
	connect(ui->action6, &QAction::triggered, this, &MainWindow::handleAction6Triggered);																//z正向
	connect(ui->action7, &QAction::triggered, this, &MainWindow::handleAction7Triggered);																//z负向
	connect(ui->action8, &QAction::triggered, this, &MainWindow::handleAction8Triggered);																//适应窗口
	connect(ui->action9, &QAction::triggered, this, &MainWindow::handleAction9Triggered);																//模型切分
	connect(ui->action10, &QAction::triggered, this, &MainWindow::handleAction10Triggered);																//导入案例

	//主界面其他事件处理
	connect(playTimer, &QTimer::timeout, this, &MainWindow::onPlayTimerTimeout);																		//播放
	connect(reverseTimer, &QTimer::timeout, this, &MainWindow::onReverseTimerTimeout);																	//倒放
	connect(loopPlayTimer, &QTimer::timeout, this, &MainWindow::onLoopPlayTimerTimeout);																//循环播放
	connect(&processRun, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessRunOutput);													//求解计算进程输出
	connect(&processDecomposePar, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessDecomposeParOutput);									//分解网格进程输出
	connect(&processReconstructPar, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessReconstructParOutput);								//合并网格进程输出
	connect(&processRun, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onProcessRunFinished);						//求解计算进程结束
	connect(&processDecomposePar, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onProcessDecomposeParFinished);		//分解网格进程结束
	connect(&processReconstructPar, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onProcessReconstructParFinished);	//重构网格进程结束
	connect(chartUpdateTimer, &QTimer::timeout, this, &MainWindow::updateChart); 																		//更新残差图
	planeRepModelClip->AddObserver(vtkCommand::ModifiedEvent, this, &MainWindow::updatePlaneRepModelClipValues); 				 						//更新模型切分平面选择器的值
	connect(this, &MainWindow::panelPushButtonClicked, this, &MainWindow::on_panelPushButton_clicked);													//控制面板按钮点击

	//副控制面板事件处理
	connect(formGeometry, &FormGeometry::geometryImported, this, &MainWindow::formGeometry_import);														//导入几何
	connect(formMeshImport, &FormMeshImport::meshImported, this, &MainWindow::formMeshImport_import);													//导入网格
	connect(formMesh, &FormMesh::apply, this, &MainWindow::formMesh_apply);																				//网格应用
	connect(formMesh, &FormMesh::updateFormFinished, this, &MainWindow::formMesh_updateFormFinished);													//更新界面完成
	connect(formMesh, &FormMesh::topoSet, this, &MainWindow::formMesh_topoSet);																			//网格拓扑集
	connect(formMesh, &FormMesh::cellZonesToRegions, this, &MainWindow::formMesh_splitMeshRegions);														//网格cellZones转区域
	connect(formSolver, &FormSolver::labelText_8_Changed, this, &MainWindow::formSolver_select);														//求解器改变
	connect(formSolver, &FormSolver::labelText_8_Changed, this, &MainWindow::onSubPanelInitialized);													//求解器改变，子面板初始化
	connect(formTurbulence, &FormTurbulence::turbulenceTypeChanged, this, &MainWindow::onSubPanelInitialized);											//湍流模型改变，子面板初始化
	connect(formRun, &FormRun::run, this, &MainWindow::formRun_run);																					//求解计算
	connect(formRun, &FormRun::stopRun, this, &MainWindow::formRun_stopRun);																			//停止计算
	connect(formPostprocessing, &FormPostprocessing::apply, this, &MainWindow::formPostprocessing_apply);												//更新渲染窗口
	connect(formPostprocessing, &FormPostprocessing::firstFrame, this, &MainWindow::formPostprocessing_firstFrame);										//第一帧
	connect(formPostprocessing, &FormPostprocessing::previousFrame, this, &MainWindow::formPostprocessing_previousFrame);								//上一帧
	connect(formPostprocessing, &FormPostprocessing::reverse, this, &MainWindow::formPostprocessing_reverse);											//重新播放
	connect(formPostprocessing, &FormPostprocessing::play, this, &MainWindow::formPostprocessing_play);													//播放
	connect(formPostprocessing, &FormPostprocessing::nextFrame, this, &MainWindow::formPostprocessing_nextFrame);										//下一帧
	connect(formPostprocessing, &FormPostprocessing::lastFrame, this, &MainWindow::formPostprocessing_lastFrame);										//最后一帧
	connect(formPostprocessing, &FormPostprocessing::loopPlay, this, &MainWindow::formPostprocessing_loopPlay);											//循环播放
	connect(formPostprocessing, &FormPostprocessing::playPause, this, &MainWindow::formPostprocessing_playPause);										//播放暂停
	connect(formPostprocessing, &FormPostprocessing::reversePause, this, &MainWindow::formPostprocessing_reversePause);									//反向播放
	connect(formPostprocessing, &FormPostprocessing::loopPlayPause, this, &MainWindow::formPostprocessing_loopPlayPause);								//循环播放
	connect(formPostprocessing, &FormPostprocessing::loadData, this, &MainWindow::formPostprocessing_loadData);											//加载数据
	connect(formPostprocessing, &FormPostprocessing::updateFormFinished, this, &MainWindow::formPostprocessing_updateFormFinished);						//更新界面完成
	connect(formModelClip, &FormModelClip::checkBoxToggled, this, &MainWindow::formModelClip_checkBoxToggle);											//模型切分页面CheckBox切换
	connect(formModelClip, &FormModelClip::lineEditsChanged, this, &MainWindow::formModelClip_lineEditsChanged);										//模型切分页面LineEdit值改变
	connect(formModelClip, &FormModelClip::xPositive, this, &MainWindow::formModelClip_xPositive);														//模型切分：X正向
	connect(formModelClip, &FormModelClip::yPositive, this, &MainWindow::formModelClip_yPositive);														//模型切分：Y正向
	connect(formModelClip, &FormModelClip::zPositive, this, &MainWindow::formModelClip_zPositive);														//模型切分：Z正向
	connect(formModelClip, &FormModelClip::cameraDirection, this, &MainWindow::formModelClip_cameraDirection);											//模型切分：相机方向
	connect(formModelClip, &FormModelClip::alignView, this, &MainWindow::formModelClip_alignView);														//模型切分：对齐视角
	connect(formModelClip, &FormModelClip::resetPlane, this, &MainWindow::formModelClip_resetPlane);													//模型切分：重置平面
	connect(formModelClip, &FormModelClip::apply, this, &MainWindow::formModelClip_apply);																//模型切分：应用
	connect(dialogResultMerge, &DialogResultMerge::interrupt, this, &MainWindow::dialogResultMerge_interrupt);											//结果合并弹窗:中断
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
	formRadiation->hide();
	formTurbulence->hide();
	formThermo->hide();
	formTransportProperties->hide();
	formCellZones->hide();
	formBoundaryConditions->hide();
	formInitialConditions->hide();
	formRun->hide();
	formPostprocessing->hide();
	formModelClip->hide();
}

// 二次贝塞尔曲线函数
static void quadraticBezier3D(double t, const double p0[3], const double p1[3],
	const double p2[3], double out[3])
{
	double u = 1.0 - t;
	double b0 = u * u;
	double b1 = 2.0 * u * t;
	double b2 = t * t;
	for (int i = 0; i < 3; i++) {
		out[i] = p0[i] * b0 + p1[i] * b1 + p2[i] * b2;
	}
}

void MainWindow::onCameraAnimationTimeout()
{
	m_camera->SetClippingRange(0.01, 10000.0);

	// 计算动画进度
	double t = m_currentFrame / static_cast<double>(m_totalFrames);

	// 计算 focal 和 up 向量
	double focal[3], up[3];
	for (int i = 0; i < 3; i++) {
		focal[i] = m_startFocal[i] * (1.0 - t) + m_endFocal[i] * t;
		up[i] = m_startUp[i] * (1.0 - t) + m_endUp[i] * t;
	}

	// 计算控制点
	double center[3] = { 0.5 * (m_startPos[0] + m_endPos[0]),
		0.5 * (m_startPos[1] + m_endPos[1]),
		0.5 * (m_startPos[2] + m_endPos[2]) };
	double control[3] = { center[0], center[1], center[2] };

	// 计算m_startPos和m_endPos关于center的向量
	double v1[3] = { m_startPos[0] - center[0],
	m_startPos[1] - center[1],
	m_startPos[2] - center[2] };
	double v2[3] = { m_endPos[0] - center[0],
	m_endPos[1] - center[1],
	m_endPos[2] - center[2] };

	double d1 = sqrt(v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2]);
	double d2 = sqrt(v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2]);
	double avgDist = 0.5 * (d1 + d2);

	// 当两个向量的夹角小于170度时，沿原来control（这里取v1方向，若d1为0则用v2）方向
	// 否则，将控制点移动到center + (avgDist, 0, 0)
	double dot = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	double angle = 0.0;
	if (d1 > 1e-6 && d2 > 1e-6)
	{
		angle = acos(dot / (d1 * d2)) * 180.0 / M_PI;
	}
	if (angle < 170.0)
	{
		double dir[3] = { 0.0, 0.0, 0.0 };
		if (d1 > 1e-6)
		{
			dir[0] = v1[0] / d1;
			dir[1] = v1[1] / d1;
			dir[2] = v1[2] / d1;
		}
		else if (d2 > 1e-6)
		{
			dir[0] = v2[0] / d2;
			dir[1] = v2[1] / d2;
			dir[2] = v2[2] / d2;
		}
		control[0] = center[0] + dir[0] * avgDist * 1.73;
		control[1] = center[1] + dir[1] * avgDist * 1.73;
		control[2] = center[2] + dir[2] * avgDist * 1.73;
	}
	else
	{
		// 若夹角>=170度，则沿x轴正方向赋值
		control[0] = center[0] + avgDist * 1.73;
		control[1] = center[1] + avgDist * 1.73;
		control[2] = center[2];
	}

	// 定义曲线三个控制点：起点、中间控制点和终点
	double p0[3] = { m_startPos[0], m_startPos[1], m_startPos[2] };
	double p1[3] = { control[0], control[1], control[2] };
	double p2[3] = { m_endPos[0], m_endPos[1], m_endPos[2] };

	// 通过二次贝塞尔曲线计算新的相机位置
	double newPos[3];
	quadraticBezier3D(t, p0, p1, p2, newPos);

	// 更新相机相关参数
	m_camera->SetPosition(newPos);
	m_camera->SetFocalPoint(focal);
	m_camera->SetViewUp(up);
	ui->openGLWidget->renderWindow()->Render();

	// 帧数控制
	m_currentFrame++;
	if (m_currentFrame > m_totalFrames) {
		m_animationTimer->stop();
		m_animationTimer->deleteLater();

		ui->action2->setEnabled(true);
		ui->action3->setEnabled(true);
		ui->action4->setEnabled(true);
		ui->action5->setEnabled(true);
		ui->action6->setEnabled(true);
		ui->action7->setEnabled(true);
		ui->action8->setEnabled(true);
	}
}

void MainWindow::onSubPanelInitialized()
{
	//获取求解器
	QString solver = formSolver->ui->label_8->text();
	if (solver == "transientIncompressibleSolver")
	{
		solver = "buoyantBoussinesqPimpleFoam";
	}
	else if (solver == "steadyCompressibleSolver")
	{
		solver = "rhoSimpleFoam";
	}
	else if (solver == "multiRegionSolver")
	{
		solver = "chtMultiRegionSimpleFoam";
	}

	//获取湍流类型
    QString turbulenceType;  
    QList<QRadioButton*> radioButtons = formTurbulence->ui->groupBox->findChildren<QRadioButton*>();  
    for (QRadioButton* radioButton : radioButtons) {  
       if (radioButton->isChecked()) {  
           turbulenceType = radioButton->text();  
           break;  
       }  
    }
	if (turbulenceType == "层流")
	{
		turbulenceType = "Laminar";
	}
	else if (turbulenceType == "雷诺平均")
	{
		turbulenceType = formTurbulence->ui->comboBox_5->currentText();
	}
	else if (turbulenceType == "大涡模拟")
	{
		turbulenceType = formTurbulence->ui->comboBox_6->currentText();
	}

	formThermo->initialization(solver);
	formBoundaryConditions->initialization(solver, turbulenceType);
	formInitialConditions->initialization(solver, "fluid", turbulenceType);
	formCellZones->initialization(solver, "fluid", turbulenceType);

}

void MainWindow::startCameraAnimation()
{
	ui->action2->setEnabled(false);
	ui->action3->setEnabled(false);
	ui->action4->setEnabled(false);
	ui->action5->setEnabled(false);
	ui->action6->setEnabled(false);
	ui->action7->setEnabled(false);
	ui->action8->setEnabled(false);

	m_totalFrames = 30;
	int intervalMs = 100 / m_totalFrames;
	m_currentFrame = 0;

	m_animationTimer = new QTimer(this);
	connect(m_animationTimer, &QTimer::timeout, this, &MainWindow::onCameraAnimationTimeout);
	m_animationTimer->start(intervalMs);
}

void MainWindow::setupCameraAnimation(double posX, double posY, double posZ,
	double focalX, double focalY, double focalZ,
	double upX, double upY, double upZ,
	bool resetCamera)
{
	// 获取开始位置
	m_camera = render->GetActiveCamera();
	m_camera->GetPosition(m_startPos);
	m_camera->GetFocalPoint(m_startFocal);
	m_camera->GetViewUp(m_startUp);

	// 获取终点位置
	m_camera->SetPosition(posX, posY, posZ);
	m_camera->SetFocalPoint(focalX, focalY, focalZ);
	m_camera->SetViewUp(upX, upY, upZ);

	if (resetCamera) {
		render->ResetCamera();
	}

	m_camera->GetPosition(m_endPos);
	m_camera->GetFocalPoint(m_endFocal);
	m_camera->GetViewUp(m_endUp);

	// 恢复初始相机位置
	m_camera->SetPosition(m_startPos);
	m_camera->SetFocalPoint(m_startFocal);
	m_camera->SetViewUp(m_startUp);

	// 启动动画
	startCameraAnimation();
}

void MainWindow::handleAction2Triggered()
{
	// X正向视图
	setupCameraAnimation(1, 0, 0, 0, 0, 0, 0, 0, 1, true);
}

void MainWindow::handleAction3Triggered()
{
	// X负向视图
	setupCameraAnimation(-1, 0, 0, 0, 0, 0, 0, 0, 1, true);
}

void MainWindow::handleAction4Triggered()
{
	// Y正向视图
	setupCameraAnimation(0, 1, 0, 0, 0, 0, 0, 0, 1, true);
}

void MainWindow::handleAction5Triggered()
{
	// Y负向视图
	setupCameraAnimation(0, -1, 0, 0, 0, 0, 0, 0, 1, true);
}

void MainWindow::handleAction6Triggered()
{
	// Z正向视图
	setupCameraAnimation(0, 0, 1, 0, 0, 0, 0, 1, 0, true);
}

void MainWindow::handleAction7Triggered()
{
	// Z负向视图
	setupCameraAnimation(0, 0, -1, 0, 0, 0, 0, 1, 0, true);
}

void MainWindow::handleAction8Triggered()
{
	// 适应窗口
	m_camera = render->GetActiveCamera();
	m_camera->GetPosition(m_startPos);
	m_camera->GetFocalPoint(m_startFocal);
	m_camera->GetViewUp(m_startUp);

	// 获取终点位置
	render->ResetCamera();
	m_camera->GetPosition(m_endPos);
	m_camera->GetFocalPoint(m_endFocal);
	m_camera->GetViewUp(m_endUp);

	// 恢复初始相机位置
	m_camera->SetPosition(m_startPos);
	m_camera->SetFocalPoint(m_startFocal);
	m_camera->SetViewUp(m_startUp);

	// 启动动画
	startCameraAnimation();
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
		GlobalData::getInstance().getCaseData()->casePath = caseFilePath.toStdString();

		//更新后处理数据页面(需补充)
		formPostprocessing->updateForm();

		//更新网格导入页面
		if(GlobalData::getInstance().getCaseData()->timeSteps.size() > 0)
			formMeshImport_import(caseFilePath, false);
		else
			formMeshImport_import(caseFilePath, true);

		//更新参数配置页面(需补充)
		formSolver->importParameter();
		//formRadiation->importParameter();
		//formTurbulence->importParameter();
		//formThermo->importParameter();
		//formTransportProperties->importParameter();
		formRun->importParameter();

		GlobalData::getInstance().getCaseData()->casePath = caseFilePath.toStdString();
		ui->textBrowser->append("Load case successfully!");
	}
}

void MainWindow::on_panelPushButton_clicked(string text)
{
	QWidget* widget = nullptr;
	if (previousPanelButton == "几何") {
		widget = formGeometry;
	}
	else if (previousPanelButton == "网格") {
		widget = formMesh;
	}
	else if (previousPanelButton == "导入") {
		widget = formMeshImport;
	}
	else if (previousPanelButton == "求解器") {
		widget = formSolver;
	}
	else if (previousPanelButton == "辐射") {
		widget = formRadiation;
	}
	else if (previousPanelButton == "湍流") {
		widget = formTurbulence;
	}
	else if (previousPanelButton == "热物理特性") {
		widget = formThermo;
	}
	else if (previousPanelButton == "传输特性") {
		widget = formTransportProperties;
	}
	else if (previousPanelButton == "区域设置")
	{
		widget = formCellZones;	 
	}
	else if (previousPanelButton == "边界条件") {
		widget = formBoundaryConditions;
	}
	else if (previousPanelButton == "初始条件") {
		widget = formInitialConditions;
	}
	else if (previousPanelButton == "求解计算") {
		widget = formRun;
	}
	else if (previousPanelButton == "后处理") {
		widget = formPostprocessing;
	}
	else if (previousPanelButton == "模型切分") {
		widget = formModelClip;
	}
	QPixmap previousSubPanelPixmap = QPixmap::grabWidget(widget);

	if (widget && previousSubPanelPixmap.size().width() > 0) {

		QLabel* pixmapLabel = new QLabel(this);
		pixmapLabel->setPixmap(previousSubPanelPixmap);
		pixmapLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
		ui->gridLayout_3->addWidget(pixmapLabel, 0, 0, 1, 1);

		QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(pixmapLabel);
		pixmapLabel->setGraphicsEffect(opacityEffect);
		QPropertyAnimation* animation = new QPropertyAnimation(opacityEffect, "opacity");
		animation->setDuration(150);
		animation->setStartValue(1.0);
		animation->setEndValue(0.0);
		connect(animation, &QPropertyAnimation::finished, pixmapLabel, &QLabel::deleteLater);
		animation->start(QAbstractAnimation::DeleteWhenStopped);
	}
	previousPanelButton = text;
}

void MainWindow::on_pushButton_clicked()
{
	hideAllSubForm();
	formGeometry->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
	emit panelPushButtonClicked("几何");
}

void MainWindow::on_pushButton_4_clicked()
{
	hideAllSubForm();
	formMeshImport->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
	emit panelPushButtonClicked("导入");
}

void MainWindow::on_pushButton_2_clicked()
{
	hideAllSubForm();
	formMesh->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
	emit panelPushButtonClicked("网格");
}

void MainWindow::on_pushButton_5_clicked()
{
	hideAllSubForm();
	formSolver->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
	emit panelPushButtonClicked("求解器");
}

void MainWindow::on_pushButton_22_clicked()
{
	hideAllSubForm();
	formRadiation->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
	emit panelPushButtonClicked("辐射");
}

void MainWindow::on_pushButton_6_clicked()
{
	hideAllSubForm();
	formTurbulence->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
	emit panelPushButtonClicked("湍流");
}

void MainWindow::on_pushButton_7_clicked()
{
	hideAllSubForm();
	formThermo->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
	emit panelPushButtonClicked("热物理特性");
}

void MainWindow::on_pushButton_21_clicked()
{
	hideAllSubForm();
	formTransportProperties->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
	emit panelPushButtonClicked("传输特性");
}

void MainWindow::on_pushButton_12_clicked()
{
	hideAllSubForm();
	formCellZones->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
	emit panelPushButtonClicked("区域设置");
}

void MainWindow::on_pushButton_13_clicked()
{
	hideAllSubForm();
	formBoundaryConditions->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
	emit panelPushButtonClicked("边界条件");
}

void MainWindow::on_pushButton_14_clicked()
{
	hideAllSubForm();
	formInitialConditions->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
	emit panelPushButtonClicked("初始条件");
}

void MainWindow::on_pushButton_16_clicked()
{
	hideAllSubForm();
	formRun->show();
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
	emit panelPushButtonClicked("求解计算");
}

void MainWindow::on_pushButton_17_clicked()
{
	hideAllSubForm();
	formPostprocessing->show();
	ui->tabWidget->setCurrentIndex(0);
	planeWidgetModelClip->Off();
	ui->openGLWidget->renderWindow()->Render();
	emit panelPushButtonClicked("后处理");
}

void MainWindow::on_pushButton_3_clicked()
{
	hideAllSubForm();
	formModelClip->show();
	ui->tabWidget->setCurrentIndex(0);
	formModelClip->ui->checkBox->setChecked(true);
	emit panelPushButtonClicked("模型切分");

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

void MainWindow::on_pushButton_20_clicked()
{

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

void MainWindow::resizeEvent(QResizeEvent* event)
{
	if (ui->tabWidget->tabBar()) {
		ui->tabWidget->tabBar()->setFixedWidth(ui->tabWidget->width());
	}
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
	QWidget* widget0 = ui->tabWidget->widget(0);
	QWidget* widget1 = ui->tabWidget->widget(1);
	QPixmap pixmap0 = QPixmap::grabWidget(widget0);
	QPixmap pixmap1 = QPixmap::grabWidget(widget1);

	int imageWidth = pixmap0.width() + pixmap1.width();
	int imageHeight = pixmap0.height();
	QImage image(imageWidth, imageHeight, QImage::Format_ARGB32);
	image.fill(QColor(Qt::black));

	QPainter p;
	p.begin(&image);
	QBrush brush(QColor(255, 255, 0), Qt::Dense4Pattern);
	p.setBrush(brush);
	QPen pen;
	pen.setColor(QColor(Qt::red));
	p.setPen(pen);
	p.drawPixmap(0, 0, pixmap0);
	p.drawPixmap(pixmap0.width(), 0, pixmap1);
	p.end();

	QLabel* animationWidget = new QLabel(ui->tabWidget);
	animationWidget->setPixmap(QPixmap::fromImage(image));
	QTabBar* bar = ui->tabWidget->tabBar();
	QSize size1 = bar->size();
	QSize size2 = ui->tabWidget->size();
	int pixmapWidth = pixmap0.width();
	int pixmapHeight = pixmap0.height();

	animationWidget->show();
	animationWidget->raise();
	QPropertyAnimation* move = new QPropertyAnimation(animationWidget, "geometry");
	move->setDuration(200);

	if (previousTabWidgetIndex == 0) {
		if (index == 1) {
			move->setStartValue(QRect(0, bar->size().height() + 10, pixmapWidth, pixmapHeight));
			move->setEndValue(QRect(-pixmapWidth, bar->size().height() + 10, pixmapWidth * 2, pixmapHeight));
		}
	}
	else if (previousTabWidgetIndex == 1) {
		if (index == 0) {
			move->setStartValue(QRect(-pixmapWidth, bar->size().height() + 10, pixmapWidth * 2, pixmapHeight));
			move->setEndValue(QRect(0, bar->size().height() + 10, pixmapWidth, pixmapHeight));
		}
	}

	move->start();
	connect(move, &QAbstractAnimation::finished, this, [=]() {
		delete animationWidget;
		delete move;
		});

	previousTabWidgetIndex = index;
}

void MainWindow::formGeometry_import(const QString& filePath)
{
	render->RemoveAllViewProps();
	QFileInfo fileInfo(filePath);
	if (!fileInfo.exists())
	{
		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "文件不存在!" });
		dialogInformationPrompt->exec();
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
		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "文件格式不支持!" });
		dialogInformationPrompt->exec();
		return;
	}

	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(97.0 / 255.0, 111.0 / 255.0, 125.0 / 255.0);
	render->AddActor(actor);
	render->ResetCamera();
	ui->openGLWidget->renderWindow()->Render();
}

static void AddDataSetsToAppendFilter(vtkDataObject* dataObj,
	vtkAppendFilter* appendFilter,
	int indentLevel = 0)
{
	if (!dataObj) return;
	for (int i = 0; i < indentLevel; ++i) std::cout << "  ";
	std::cout << dataObj->GetClassName() << std::endl;

	if (auto ds = vtkDataSet::SafeDownCast(dataObj)) {
		appendFilter->AddInputData(ds);
	}
	else if (auto mb = vtkMultiBlockDataSet::SafeDownCast(dataObj)) {
		for (unsigned int i = 0; i < mb->GetNumberOfBlocks(); ++i)
			AddDataSetsToAppendFilter(mb->GetBlock(i), appendFilter, indentLevel + 1);
	}
}

std::vector<vtkSmartPointer<vtkActor>> MainWindow::createMeshPatchActor(
	const std::string& casePath,
	std::vector<std::string>& patchGroup)
{
	std::vector<vtkSmartPointer<vtkActor>> actorList;
	actorList.reserve(patchGroup.size());

	// 逐个patch独立读取并生成Actor
	for (const auto& singlePatch : patchGroup)
	{
		// 创建并配置OpenFOAMReader
		auto openFOAMReader = vtkSmartPointer<vtkOpenFOAMReader>::New();
		openFOAMReader->SetFileName(casePath.c_str());
		openFOAMReader->SetCreateCellToPoint(1);
		openFOAMReader->SetSkipZeroTime(1);
		openFOAMReader->UpdateInformation();

		// 仅启用当前patch
		openFOAMReader->DisableAllPatchArrays();
		openFOAMReader->SetPatchArrayStatus(singlePatch.c_str(), 1);
		openFOAMReader->Update();

		// 判断是否是internalMesh
		bool isInternalMesh = (singlePatch.find("internalMesh") != std::string::npos);

		vtkSmartPointer<vtkDataSet> dataSet;
		if (isInternalMesh)
		{
			auto multiBlockDataSet = openFOAMReader->GetOutput();
			auto appendFilter = vtkSmartPointer<vtkAppendFilter>::New();
			AddDataSetsToAppendFilter(multiBlockDataSet, appendFilter);
			appendFilter->Update();
			dataSet = appendFilter->GetOutput();
		}
		else
		{
			auto geometryFilter = vtkSmartPointer<vtkCompositeDataGeometryFilter>::New();
			geometryFilter->SetInputConnection(openFOAMReader->GetOutputPort());
			geometryFilter->Update();
			if (auto polyData = geometryFilter->GetOutput())
			{
				dataSet = polyData;
			}
			else
			{
				// 若读取不到有效数据则跳过
				continue;
			}
		}

		if (!dataSet)
		{
			continue;
		}

		// 创建对应Actor
		auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		mapper->SetInputData(dataSet);
		mapper->ScalarVisibilityOff();

		auto actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		actor->GetProperty()->SetColor(0.0, 221.0 / 255.0, 221.0 / 255.0);
		actor->GetProperty()->SetRepresentationToSurface();
		actor->GetProperty()->EdgeVisibilityOn();
		actor->GetProperty()->SetEdgeColor(0.0, 0.0, 0.0);

		actorList.push_back(actor);
	}

	return actorList;
}

void MainWindow::formMeshImport_import(const QString& filePath, bool isRender)
{
	if (isRender)render->RemoveAllViewProps();
	QFileInfo fileInfo(filePath);
	if (!fileInfo.exists())
	{
		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "文件不存在!" });
		dialogInformationPrompt->exec();
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
		GlobalData::getInstance().getCaseData()->casePath = filePath.toStdString();
		formMesh->updateForm(isRender);

		//网格导入成功,初始化参数配置页面(需补充)
		//formBoundaryConditions->onMeshImported();
	}

	render->ResetCamera();
	renderWindow->Render();
}

void MainWindow::formMesh_apply()
{
	// 移除所有已添加的演员
	render->RemoveAllViewProps();

	// 获取patchGroup
	std::vector<std::string> patchGroup;
	QList<QListWidgetItem*> selectedItems = formMesh->ui->listWidget_2->selectedItems();
	for (const auto& item : selectedItems)
	{
		CustomItemWidget* widget = qobject_cast<CustomItemWidget*>(formMesh->ui->listWidget_2->itemWidget(item));
		if (widget)
		{
			std::vector<QString> textList;
			textList.push_back(widget->Param1);
			textList.push_back(widget->Param2);
			textList.push_back(widget->Param3);
			textList.push_back(widget->Param4);

			for (auto& str : textList)
			{
				QString regionName, patchName;
				int index = str.indexOf(" in ");
				if (index != -1)
				{
					regionName = str.mid(index + 4);
					patchName = str.left(index);

					if (regionName == "default")
					{
						patchGroup.push_back((patchName == "internalMesh") ? patchName.toStdString() : "patch/" + patchName.toStdString());
					}
					else
					{
						std::string prefix = "/" + regionName.toStdString() + "/";
						patchGroup.push_back(prefix + ((patchName == "internalMesh") ? patchName.toStdString() : "patch/" + patchName.toStdString()));
					}
				}
			}
		}
	}

	// 创建 meshPatchActor
	std::vector<vtkSmartPointer<vtkActor>> meshPatchActorList = createMeshPatchActor(GlobalData::getInstance().getCaseData()->casePath, patchGroup);
	for (const auto& meshPatchActor : meshPatchActorList)
	{
		render->AddActor(meshPatchActor);
	}
	renderWindow->Render();
}

void MainWindow::formMesh_updateFormFinished()
{
	if (lastClickedButton != ui->pushButton_2) {
		on_pushButton_2_clicked();
		ui->pushButton_2->setStyleSheet("QPushButton { background-color: rgb(232, 232, 232); border: none; text-align: left; padding-left: 50px; }");
		lastClickedButton->setStyleSheet("QPushButton { background-color: rgb(255, 255, 255); border: none; text-align: left; padding-left: 50px; } QPushButton:hover { background-color: rgb(242, 242, 242); }");
		lastClickedButton = ui->pushButton_2;
	}

	formMesh->on_pushButton_clicked();
	handleAction8Triggered();

	for (int i = 0; i < formMesh->ui->listWidget_2->count(); ++i) 
		formMesh->ui->listWidget_2->item(i)->setSelected(false);
}

void MainWindow::formMesh_topoSet()
{
	QProcess* process = new QProcess(this);

	// 创建非阻塞的提示对话框
	DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "提示", { "正在拓扑设置..." }, false);
	dialogInformationPrompt->show();

	// 连接 QProcess 的标准输出信号
	connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
		while (process->canReadLine()) {
			QByteArray output = process->readLine();
			ui->textBrowser->append(QString::fromLocal8Bit(output));
			ui->textBrowser->repaint();
		}
		});

	// 连接 QProcess 的 finished 信号，在进程结束时关闭并释放对话框
	connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
		dialogInformationPrompt->close();
		dialogInformationPrompt->deleteLater(); // 自动释放对话框
		process->deleteLater();                // 自动释放 QProcess
		});

	// 设置并启动 QProcess
	QFileInfo fileInfo(QString::fromStdString(GlobalData::getInstance().getCaseData()->casePath));
	QString casePath = fileInfo.absolutePath();
	QString command = "topoSet -noFunctionObjects -case " + casePath;
	process->setProgram("cmd.exe");
	process->setArguments(QStringList() << "/C" << command);
	process->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments* args) {
		args->flags |= CREATE_NO_WINDOW;
		});
	process->start();
}

void MainWindow::formMesh_splitMeshRegions()
{
	QProcess* process = new QProcess(this);

	// 创建非阻塞的提示对话框
	DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "提示", { "正在生成计算域..." }, false);
	dialogInformationPrompt->show();

	// 连接 QProcess 的标准输出信号
	connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
		while (process->canReadLine()) {
			QByteArray output = process->readLine();
			ui->textBrowser->append(QString::fromLocal8Bit(output));
			ui->textBrowser->repaint();
		}
		});

	// 连接 QProcess 的 finished 信号，在进程结束时关闭并释放对话框
	connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
		dialogInformationPrompt->close();
		dialogInformationPrompt->deleteLater(); // 自动释放对话框
		process->deleteLater();                // 自动释放 QProcess

		//获取constantPath除了polyMesh以外的文件夹名称
		std::string casePath = GlobalData::getInstance().getCaseData()->casePath;
		std::string constantPath = casePath.substr(0, casePath.find_last_of("/\\")) + "/constant";

		QString result;
		QDir dir(QString::fromStdString(constantPath));
		QStringList subfolders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
		result.clear();

		for (const QString& folder : subfolders) {
			if (folder != "polyMesh") {
				result += folder + " ";
			}
		}

		if (!result.isEmpty()) {
			result.chop(1);
		}


        //生成regionProperties文件       
        std::string regionPropertiesPath = casePath.substr(0, casePath.find_last_of("/\\")) + "/constant/regionProperties";  
        QFile file(QString::fromStdString(regionPropertiesPath));  
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))  
        {  
           QTextStream out(&file);  
           out << "FoamFile\n";  
           out << "{\n";  
           out << "    version     2.0;\n";  
           out << "    format      ascii;\n";  
           out << "    class       dictionary;\n";  
           out << "    object      regionProperties;\n";
           out << "}\n";  
           out << "regions\n";  
           out << "(\n";  
           out << "    fluid       ("+ result +")\n";
           out << "    solid       ()\n";  
           out << ");\n";  

           file.close();  
        }

		// 更新网格导入页面
		formMesh->updateForm(false);
		});

	// 设置并启动 QProcess
	QFileInfo fileInfo(QString::fromStdString(GlobalData::getInstance().getCaseData()->casePath));
	QString casePath = fileInfo.absolutePath();
	QString command = "splitMeshRegions -cellZones -overwrite -prefixRegion -case " + casePath;
	process->setProgram("cmd.exe");
	process->setArguments(QStringList() << "/C" << command);
	process->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments* args) {
		args->flags |= CREATE_NO_WINDOW;
		});
	process->start();
}

void MainWindow::formSolver_select(const QString& application)
{
	//初始化tabWidget控件可见性
	ui->pushButton_23->hide();	//网格加密
	ui->pushButton_24->hide();	//翼型
	ui->pushButton_22->hide();	//辐射
	ui->pushButton_6->hide();	//湍流
	ui->pushButton_25->hide();	//动态网格
	ui->pushButton_26->hide();	//离散项
	ui->pushButton_27->hide();	//燃烧
	ui->pushButton_7->hide();	//热
	ui->pushButton_28->hide();	//弹性模量
	ui->pushButton_21->hide();	//传输特性
	ui->pushButton_8->hide();	//离散化
	ui->pushButton_9->hide();	//线性求解器
	ui->pushButton_10->hide();	//被动标量
	ui->pushButton_11->hide();	//参考值
	ui->pushButton_12->hide();	//区域设置
	ui->pushButton_13->hide();	//边界条件
	ui->pushButton_14->hide();	//初始条件
	ui->pushButton_15->hide();	//监视器
	ui->line_5->hide();
	ui->pushButton_16->hide();	//求解计算
	ui->line_4->hide();
	ui->pushButton_17->hide();	//后处理
	ui->pushButton_3->hide();	//模型切分
	ui->pushButton_18->hide();	//计算

	if (application == "rhoSimpleFoam")
	{
		ui->pushButton_6->show();	//湍流
		ui->pushButton_7->show();	//热
		ui->pushButton_8->show();	//离散化
		ui->pushButton_9->show();	//线性求解器
		ui->pushButton_10->show();	//被动标量
		ui->pushButton_11->show();	//参考值
		ui->pushButton_12->show();	//区域设置
		ui->pushButton_13->show();	//边界条件
		ui->pushButton_14->show();	//初始条件
		ui->pushButton_15->show();	//监视器
		ui->line_5->show();
		ui->pushButton_16->show();	//求解计算
		ui->line_4->show();
		ui->pushButton_17->show();	//后处理
		ui->pushButton_3->show();	//模型切分
		ui->pushButton_18->show();	//计算
	}
	else if (application == "buoyantBoussinesqPimpleFoam")
	{
		ui->pushButton_22->show();	//辐射
		ui->pushButton_6->show();	//湍流
		ui->pushButton_21->show();	//传输特性
		ui->pushButton_8->show();	//离散化
		ui->pushButton_9->show();	//线性求解器
		ui->pushButton_10->show();	//被动标量
		ui->pushButton_11->show();	//参考值
		ui->pushButton_12->show();	//区域设置
		ui->pushButton_13->show();	//边界条件
		ui->pushButton_14->show();	//初始条件
		ui->pushButton_15->show();	//监视器
		ui->line_5->show();
		ui->pushButton_16->show();	//求解计算
		ui->line_4->show();
		ui->pushButton_17->show();	//后处理
		ui->pushButton_3->show();	//模型切分
		ui->pushButton_18->show();	//计算
	}
	else if (application == "chtMultiRegionSimpleFoam")
	{
		ui->pushButton_22->show();	//辐射
		ui->pushButton_6->show();	//湍流
		ui->pushButton_7->show();	//热
		ui->pushButton_8->show();	//离散化
		ui->pushButton_9->show();	//线性求解器
		ui->pushButton_10->show();	//被动标量
		ui->pushButton_11->show();	//参考值
		ui->pushButton_12->show();	//区域设置
		ui->pushButton_13->show();	//边界条件
		ui->pushButton_14->show();	//初始条件
		ui->pushButton_15->show();	//监视器
		ui->line_5->show();
		ui->pushButton_16->show();	//求解计算
		ui->line_4->show();
		ui->pushButton_17->show();	//后处理
		ui->pushButton_3->show();	//模型切分
		ui->pushButton_18->show();	//计算
	}
}

void MainWindow::formRun_run()
{
	if (GlobalData::getInstance().getCaseData()->casePath.empty())
	{
		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "请先导入网格!" });
		dialogInformationPrompt->exec();
		return;
	}
	//保存界面上所有的配置参数，并校验是否符合要求(需补充)
	//formSolver->exportParameter();
	//formRadiation->exportParameter();
	//formTurbulence->exportParameter();
	//formThermo->exportParameter();
	//formTransportProperties->exportParameter();
	//formBoundaryConditions->exportParameter();
	formRun->exportParameter();

	//隐藏开始按钮，显示停止按钮
	QThread::msleep(500);
	formRun->ui->pushButton->hide();
	formRun->ui->pushButton_2->show();
	formRun->ui->label_12->show();

	//移除三维窗口中的所有Actor
	render->RemoveAllViewProps();

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

	// 删除原来的计算结果和并行计算文件
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

			if (folder.startsWith("processor"))
			{
				QDir folderDir(caseDir + "/" + folder);
				folderDir.removeRecursively();
			}
		}
	}

	// 打开 controlDict 文件
	QFile controlDictFile(controlDictPath);
	if (!controlDictFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "无法打开 controlDict 文件!" });
		dialogInformationPrompt->exec();
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
		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "未找到 application 字段!" });
		dialogInformationPrompt->exec();
		return;
	}

	//串行计算
	nWriteResults = 0;
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
		processDecomposePar.setProgram("cmd.exe");
		processDecomposePar.setArguments(QStringList() << "/C" << commandDecomposePar);
		processDecomposePar.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments* args) {
			args->flags |= CREATE_NO_WINDOW;
			});
		processDecomposePar.start();
	}
}

void MainWindow::onProcessDecomposeParFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	//获取案例路径
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QFileInfo fileInfo(casePath);
	QString caseDir = fileInfo.path();
	QString controlDictPath = caseDir + "/system/controlDict";

	// 打开 controlDict 文件
	QFile controlDictFile(controlDictPath);
	if (!controlDictFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "无法打开 controlDict 文件!" });
		dialogInformationPrompt->exec();
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
					application.chop(1);
				}
				break;
			}
		}
	}
	controlDictFile.close();

	if (application.isEmpty()) {
		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "未找到 application 字段!" });
		dialogInformationPrompt->exec();
		return;
	}

	//计算
	QString commandRun = "mpiexec -np " + QString::number(formRun->ui->spinBox->value()) + " " + application + " -parallel -case " + caseDir;
	processRun.setProgram("cmd.exe");
	processRun.setArguments(QStringList() << "/C" << commandRun);
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

	// 获取案例路径
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QFileInfo fileInfo(casePath);
	QString caseDir = fileInfo.path();

	// 并行计算需要合并网格
	if (formRun->ui->radioButton_2->isChecked()) {
		dialogResultMerge->ui->progressBar->setValue(0);
		dialogResultMerge->show();

		QString comandReconstructPar = "reconstructPar -case " + caseDir;
		processReconstructPar.setProgram("cmd.exe");
		processReconstructPar.setArguments(QStringList() << "/C" << comandReconstructPar);
		processReconstructPar.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments* args) {
			args->flags |= CREATE_NO_WINDOW;
			});

		processReconstructPar.start();
	}
	else {
		QString caseFilePath = QString::fromStdString(GlobalData::getInstance().getCaseData()->casePath);
		formPostprocessing->updateForm();
	}
}

void MainWindow::onProcessReconstructParFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitCode);
	Q_UNUSED(exitStatus);

	dialogResultMerge->ui->progressBar->setValue(100);
	dialogResultMerge->hide();

	// 更新后处理页面
	QString caseFilePath = QString::fromStdString(GlobalData::getInstance().getCaseData()->casePath);
	formPostprocessing->updateForm();
}

void MainWindow::formRun_stopRun()
{
	QThread::msleep(500);
	formRun->ui->pushButton_2->hide();
	formRun->ui->label_12->hide();
	formRun->ui->pushButton->show();

	if (formRun->ui->radioButton->isChecked() == true) {
		if (processRun.state() == QProcess::Running) processRun.kill();
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

void MainWindow::formPostprocessing_apply(std::vector<QListView*> listViewBoundaries)
{
	// 移除所有已添加的演员
	render->RemoveAllViewProps();

	string casePath = GlobalData::getInstance().getCaseData()->casePath;
	double timeValue = formPostprocessing->ui->comboBox->currentText().toDouble();
	std::string fieldNameValue = formPostprocessing->ui->comboBox_2->currentText().toStdString();

	//获取region名称
	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(formPostprocessing->ui->listView->model());
	QStringList items;
	for (int i = 0; i < model->rowCount(); ++i) {
		items << model->item(i)->text();
	}

	// 遍历 listViewModel 并根据选中状态添加到
	std::vector<std::string> patchGroup;
	for (int i = listViewBoundaries.size() - 1; i >= 0; --i) {
		auto* model = qobject_cast<QStandardItemModel*>(listViewBoundaries[i]->model());
		bool isDefault = (items[items.size() - i - 1] == "default");

		for (int j = 0; j < model->rowCount(); ++j) {
			QStandardItem* item = model->item(j);
			if (!item || item->checkState() != Qt::Checked) continue;

			const std::string text = item->text().toStdString();
			if (isDefault) {
				patchGroup.push_back((text == "internalMesh") ? text : "patch/" + text);
			}
			else {
				std::string prefix = "/" + items[items.size() - i - 1].toStdString() + "/";
				patchGroup.push_back(prefix + ((text == "internalMesh") ? text : "patch/" + text));
			}
		}
	}

	std::pair<double, double> globalRange = GlobalData::getInstance().getCaseData()->fieldsScalarRange[fieldNameValue];
	std::vector<vtkSmartPointer<vtkActor>> actorList = createNephogramPatchActor(casePath, timeValue, fieldNameValue, patchGroup, globalRange);

	for (const auto& actor : actorList)
	{
		render->AddActor(actor);
	}
	vtkSmartPointer<vtkScalarBarActor> scalarBar = createScalarBarActor(globalRange);
	render->AddActor2D(scalarBar);
	renderWindow->Render();
}

void MainWindow::formPostprocessing_firstFrame()
{
	formPostprocessing->ui->comboBox->setCurrentIndex(0);
	formPostprocessing->on_pushButton_2_clicked();
}

void MainWindow::formPostprocessing_previousFrame()
{
	int index = formPostprocessing->ui->comboBox->currentIndex();
	if (index > 0) {
		formPostprocessing->ui->comboBox->setCurrentIndex(index - 1);
		formPostprocessing->on_pushButton_2_clicked();
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
		formPostprocessing->on_pushButton_2_clicked();
	}
}

void MainWindow::formPostprocessing_lastFrame()
{
	formPostprocessing->ui->comboBox->setCurrentIndex(formPostprocessing->ui->comboBox->count() - 1);
	formPostprocessing->on_pushButton_2_clicked();
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
		formPostprocessing->updateForm();
	}
}

void MainWindow::formPostprocessing_updateFormFinished()
{
	//切换到后处理子页面
	if (lastClickedButton != ui->pushButton_17) {
		on_pushButton_17_clicked();
		ui->pushButton_17->setStyleSheet("QPushButton { background-color: rgb(232, 232, 232); border: none; text-align: left; padding-left: 50px; }");
		lastClickedButton->setStyleSheet("QPushButton { background-color: rgb(255, 255, 255); border: none; text-align: left; padding-left: 50px; } QPushButton:hover { background-color: rgb(242, 242, 242); }");
		lastClickedButton = ui->pushButton_17;
	}

	formPostprocessing->on_pushButton_2_clicked();
	handleAction8Triggered();
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
		viewUp[1] = 1.0;
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

std::vector<vtkSmartPointer<vtkActor>> MainWindow::createSlicedActorFromRenderer(double origin[3], double normal[3], bool keepInside)
{
	// 保存切分后的各个模型
	std::vector<vtkSmartPointer<vtkActor>> slicedActors;

	// 获取场景中的 actor 集合
	vtkActorCollection* actors = render->GetActors();
	if (!actors)
		return slicedActors;

	// 定义一个默认的几何模型判断颜色（与原函数一致）
	double geometryColor[3] = { 97.0 / 255.0, 111.0 / 255.0, 125.0 / 255.0 };

	// 创建切分平面
	auto plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetOrigin(origin);
	plane->SetNormal(normal);

	actors->InitTraversal();
	for (vtkActor* actor = actors->GetNextActor(); actor != nullptr; actor = actors->GetNextActor())
	{
		// 不可见时跳过
		if (!actor->GetVisibility())
			continue;

		vtkMapper* srcMapper = actor->GetMapper();
		if (!srcMapper)
			continue;

		// 判断当前 actor 类型
		bool isCloudModel = false;
		bool isGeometryModel = false;
		bool isMeshModel = false;
		double* actorColor = actor->GetProperty()->GetColor();

		// 是否为几何模型（颜色判断）
		if (fabs(actorColor[0] - geometryColor[0]) < 0.01 &&
			fabs(actorColor[1] - geometryColor[1]) < 0.01 &&
			fabs(actorColor[2] - geometryColor[2]) < 0.01)
		{
			isGeometryModel = true;
		}
		// 是否云图模型（是否开启了 ScalarVisibility 等）
		if (srcMapper->GetScalarVisibility() &&
			srcMapper->GetInput() &&
			srcMapper->GetInput()->GetPointData() &&
			srcMapper->GetInput()->GetPointData()->GetScalars())
		{
			isCloudModel = true;
		}
		//判断EdgeVisibility是否打开
		if (actor->GetProperty()->GetEdgeVisibility())
		{
			isMeshModel = true;
		}

		// 执行切分
		vtkDataSet* inputDS = vtkDataSet::SafeDownCast(srcMapper->GetInput());
		if (!inputDS)
			continue;

		auto clipper = vtkSmartPointer<vtkTableBasedClipDataSet>::New();
		clipper->SetInputData(inputDS);
		clipper->SetClipFunction(plane);
		clipper->SetInsideOut(!keepInside);
		clipper->Update();

		// 直接保存 clipper 的输出数据（原始3D数据），不做 vtkDataSetSurfaceFilter 处理
		vtkSmartPointer<vtkDataSet> clippedData3D = clipper->GetOutput();
		if (!clippedData3D || clippedData3D->GetNumberOfPoints() == 0)
			continue;

		// 为当前模型创建新的 mapper 和 actor，后续若需要切分，可以基于保存的 clippedData3D 进行进一步处理
		auto newMapper = vtkSmartPointer<vtkDataSetMapper>::New();
		newMapper->SetInputData(clippedData3D);
		auto newActor = vtkSmartPointer<vtkActor>::New();
		newActor->SetMapper(newMapper);

		// 根据类型设置属性
		if (isCloudModel)
		{
			// 云图模型
			newMapper->ScalarVisibilityOn();
			std::string fieldNameValue = formPostprocessing->ui->comboBox_2->currentText().toStdString();
			std::pair<double, double> range = GlobalData::getInstance().getCaseData()->fieldsScalarRange[fieldNameValue];
			newMapper->SetScalarRange(range.first, range.second);

			auto colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
			colorTransferFunction->SetColorSpaceToRGB();
			colorTransferFunction->AddRGBPoint(range.first, 0.0, 127.0 / 255.0, 1.0);
			colorTransferFunction->AddRGBPoint((range.first + range.second) / 2.0, 234.0 / 255.0, 213.0 / 255.0, 201.0 / 255.0);
			colorTransferFunction->AddRGBPoint(range.second, 180.0 / 255.0, 0.0, 0.0);

			newMapper->SetLookupTable(colorTransferFunction);
			newMapper->UseLookupTableScalarRangeOn();

			newActor->GetProperty()->EdgeVisibilityOff();
			newActor->GetProperty()->SetRepresentationToSurface();
		}
		else if (isGeometryModel)
		{
			// 几何模型
			newMapper->ScalarVisibilityOff();
			newActor->GetProperty()->EdgeVisibilityOff();
			newActor->GetProperty()->SetRepresentationToSurface();
			newActor->GetProperty()->SetColor(geometryColor);
		}
		else if (isMeshModel)
		{
			// 网格模型
			newMapper->ScalarVisibilityOff();
			newActor->GetProperty()->EdgeVisibilityOn();
			newActor->GetProperty()->SetEdgeColor(0.0, 0.0, 0.0);
			newActor->GetProperty()->SetRepresentationToSurface();
			newActor->GetProperty()->SetColor(0.0, 221.0 / 255.0, 221.0 / 255.0);
		}
		else {
			// 其他模型
			newMapper->ScalarVisibilityOff();
			newActor->GetProperty()->EdgeVisibilityOff();
			newActor->GetProperty()->SetRepresentationToSurface();
			newActor->GetProperty()->SetColor(200.0 / 255.0, 197.0 / 255.0, 189.0 / 255.0);
		}

		// 收集到返回向量中
		slicedActors.push_back(newActor);
	}

	return slicedActors;
}

void MainWindow::formModelClip_apply()
{
	// 获取平面选择器的原点和法向量
	double origin[3];
	double normal[3];
	planeRepModelClip->GetOrigin(origin);
	planeRepModelClip->GetNormal(normal);

	// 隐藏平面选择器
	formModelClip->ui->checkBox->setChecked(false);

	// 创建切割后的Actor
	bool keepInside = !formModelClip->ui->checkBox_2->isChecked();
	std::vector<vtkSmartPointer<vtkActor>> slicedActorList = createSlicedActorFromRenderer(origin, normal, keepInside);

	// 清除原有演员
	render->RemoveAllViewProps();

	if (slicedActorList.size() > 0)
	{
		for (const auto& slicedActor : slicedActorList)
		{
			render->AddActor(slicedActor);
		}

		// 如果是云图模型，添加颜色图例
		vtkMapper* mapper = slicedActorList[0]->GetMapper();
		if (mapper && mapper->GetScalarVisibility())
		{
			std::string fieldNameValue = formPostprocessing->ui->comboBox_2->currentText().toStdString();
			std::pair<double, double> range = GlobalData::getInstance().getCaseData()->fieldsScalarRange[fieldNameValue];
			vtkSmartPointer<vtkScalarBarActor> scalarBar = createScalarBarActor(range);
			render->AddActor2D(scalarBar);
		}

		// 重新显示平面选择器
		formModelClip->ui->checkBox->setChecked(true);
		ui->openGLWidget->renderWindow()->Render();
	}
}

void MainWindow::dialogResultMerge_interrupt()
{
	if (processReconstructPar.state() == QProcess::Running) {
		processReconstructPar.kill();
	}
	dialogResultMerge->hide();
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
		ui->chartView->update();
		ui->chartView->repaint();

		//如果输出信息中包含"ExecutionTime"，则更新时间步
		if (output.contains("ExecutionTime") && (currentTimeStep % formRun->ui->spinBox_2->value() == 0)) {
			nWriteResults++;
		}
	}
}

void MainWindow::onProcessDecomposeParOutput()
{
	while (processDecomposePar.canReadLine()) {
		QByteArray output = processDecomposePar.readLine();
		ui->textBrowser->append(QString::fromLocal8Bit(output));
		ui->textBrowser->repaint();
	}
}

void MainWindow::onProcessReconstructParOutput()
{

	while (processReconstructPar.canReadLine()) {
		QByteArray output = processReconstructPar.readLine();
		ui->textBrowser->append(QString::fromLocal8Bit(output));
		ui->textBrowser->repaint();

		//如果输出信息中以“Time =”开始，则更新进度条
		if (output.startsWith("Time =")) {
			dialogResultMerge->ui->progressBar->setValue(dialogResultMerge->ui->progressBar->value() + 1.0 / (nWriteResults + 1) * 100);
		}
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

std::vector<vtkSmartPointer<vtkActor>> MainWindow::createNephogramPatchActor(
	const std::string& casePath,
	double timeValue,
	const std::string& fieldName,
	std::vector<std::string>& patchGroup,
	const std::pair<double, double>& globalRange)
{
	std::vector<vtkSmartPointer<vtkActor>> actorList;
	actorList.reserve(patchGroup.size());

	for (const auto& singlePatch : patchGroup)
	{
		// 配置 OpenFOAMReader
		auto openFOAMReader = vtkSmartPointer<vtkOpenFOAMReader>::New();
		openFOAMReader->SetFileName(casePath.c_str());
		openFOAMReader->SetCreateCellToPoint(1);
		openFOAMReader->SetSkipZeroTime(1);
		openFOAMReader->SetTimeValue(timeValue);
		openFOAMReader->UpdateInformation();

		// 仅启用当前 patch
		openFOAMReader->DisableAllPatchArrays();
		openFOAMReader->SetPatchArrayStatus(singlePatch.c_str(), 1);

		// 更新时间步信息
		vtkInformation* outInfo = openFOAMReader->GetOutputInformation(0);
		outInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEP(), timeValue);
		openFOAMReader->Update();

		// 是否为内部网格
		bool isInternalMesh =
			(singlePatch.find("internalMesh") != std::string::npos);

		vtkSmartPointer<vtkDataSet> dataSet;
		if (isInternalMesh)
		{
			auto multiBlockDataSet = openFOAMReader->GetOutput();
			auto appendFilter = vtkSmartPointer<vtkAppendFilter>::New();
			AddDataSetsToAppendFilter(multiBlockDataSet, appendFilter);
			appendFilter->Update();
			dataSet = appendFilter->GetOutput();
		}
		else
		{
			auto geometryFilter = vtkSmartPointer<vtkCompositeDataGeometryFilter>::New();
			geometryFilter->SetInputConnection(openFOAMReader->GetOutputPort());
			geometryFilter->Update();
			dataSet = geometryFilter->GetOutput();
		}

		if (!dataSet)
		{
			continue;
		}

		// 如果存在 fieldName 则进行云图渲染，否则使用默认颜色
		bool hasField = dataSet->GetPointData()->HasArray(fieldName.c_str());
		if (hasField)
		{
			dataSet->GetPointData()->SetActiveScalars(fieldName.c_str());
		}

		auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		mapper->SetInputData(dataSet);

		if (hasField)
		{
			// 配置颜色映射
			auto colorTF = vtkSmartPointer<vtkColorTransferFunction>::New();
			colorTF->SetColorSpaceToRGB();
			colorTF->AddRGBPoint(globalRange.first, 0.0, 127.0 / 255.0, 1.0);
			colorTF->AddRGBPoint(
				(globalRange.first + globalRange.second) / 2.0,
				234.0 / 255.0, 213.0 / 255.0, 201.0 / 255.0);
			colorTF->AddRGBPoint(globalRange.second, 180.0 / 255.0, 0.0, 0.0);

			mapper->SetLookupTable(colorTF);
			mapper->UseLookupTableScalarRangeOn();
			mapper->SetScalarRange(globalRange.first, globalRange.second);
			mapper->ScalarVisibilityOn();
		}
		else
		{
			// 无字段数据时使用默认颜色
			mapper->ScalarVisibilityOff();
		}

		auto actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);

		if (hasField)
		{
			actor->GetProperty()->EdgeVisibilityOff();
			actor->GetProperty()->SetRepresentationToSurface();
		}
		else
		{
			// 设置成浅色
			actor->GetProperty()->SetColor(200.0 / 255.0, 197.0 / 255.0, 189.0 / 255.0);
			actor->GetProperty()->EdgeVisibilityOff();
			actor->GetProperty()->SetRepresentationToSurface();
		}

		actorList.push_back(actor);
	}

	return actorList;
}

vtkSmartPointer<vtkScalarBarActor> MainWindow::createScalarBarActor(const std::pair<double, double>& range)
{
	// 创建颜色传输函数
	vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	colorTransferFunction->SetColorSpaceToRGB();

	// 添加颜色点
	colorTransferFunction->AddRGBPoint(range.first, 0 / 255.0, 127 / 255.0, 255 / 255.0); // 蓝色
	colorTransferFunction->AddRGBPoint((range.first + range.second) / 2.0, 234.0 / 255.0, 213.0 / 255.0, 201.0 / 255.0); // 白色
	colorTransferFunction->AddRGBPoint(range.second, 180.0 / 255.0, 0 / 255.0, 0 / 255.0); // 红色

	// 创建图例
	vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
	scalarBar->SetLookupTable(colorTransferFunction);
	scalarBar->SetNumberOfLabels(4);
	scalarBar->SetOrientationToVertical();
	scalarBar->SetPosition(0.92, 0.01);
	scalarBar->SetWidth(0.06);
	scalarBar->SetHeight(0.3);
	scalarBar->SetLabelFormat("%1.2e");

	// 设置图例标题的文本属性
	vtkSmartPointer<vtkTextProperty> titleTextProperty = vtkSmartPointer<vtkTextProperty>::New();
	titleTextProperty->SetFontSize(24);
	titleTextProperty->SetColor(1.0, 1.0, 1.0);
	titleTextProperty->SetBold(1);
	titleTextProperty->SetJustificationToCentered();
	scalarBar->SetTitleTextProperty(titleTextProperty);

	// 设置图例标签的文本属性
	vtkSmartPointer<vtkTextProperty> labelTextProperty = vtkSmartPointer<vtkTextProperty>::New();
	labelTextProperty->SetFontSize(18);
	labelTextProperty->SetColor(0, 0, 0);
	scalarBar->SetLabelTextProperty(labelTextProperty);

	return scalarBar;
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
		formPostprocessing->on_pushButton_2_clicked();
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
		formPostprocessing->on_pushButton_2_clicked();
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
	formPostprocessing->on_pushButton_2_clicked();
	formPostprocessing->pushButtonLoopPlayTimerPause->show();
	formPostprocessing->ui->pushButton_9->hide();
}

void MainWindow::handleAction1Triggered()
{
	ui->textBrowser->setVisible(!ui->textBrowser->isVisible());
}