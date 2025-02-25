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
	//ȷ����ǰ������С��������
	setWindowFlags(Qt::Window);
#ifdef _WIN32
	SetWindowLong(HWND(this->winId()), GWL_EXSTYLE, WS_EX_APPWINDOW);
#endif

	//ȫ��
	this->setWindowState(Qt::WindowMaximized);
	ui->setupUi(this);

	//��ʼ����ά����
	renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	render = vtkSmartPointer<vtkRenderer>::New();
	ui->openGLWidget->setRenderWindow(renderWindow);
	renderWindow->AddRenderer(render);
	render->SetBackground(1.0, 1.0, 1.0);
	render->SetBackground2(27 / 255.0, 85 / 255.0, 133 / 255.0);
	render->GradientBackgroundOn();
	addCoordinateAxes();

	// ��ʼ����ť
	QPushButton* buttons[] = {
		ui->pushButton, ui->pushButton_2, ui->pushButton_3, ui->pushButton_4, ui->pushButton_5,
		ui->pushButton_6, ui->pushButton_7, ui->pushButton_8, ui->pushButton_9, ui->pushButton_10,
		ui->pushButton_11, ui->pushButton_12, ui->pushButton_13, ui->pushButton_14, ui->pushButton_15,
		ui->pushButton_16, ui->pushButton_17, ui->pushButton_18, ui->pushButton_19, ui->pushButton_20
	};
	for (QPushButton* button : buttons) {
		connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
	}

	//���ֹ���
	QList<int> sizes;
	sizes << 400 << 100;
	ui->splitter->setSizes(sizes);

	//�������������
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

	//���������������ҳ��
	on_pushButton_clicked();
	ui->pushButton->setStyleSheet("QPushButton { background-color: rgb(232, 232, 232); border: none; text-align: left; padding-left: 50px; }");
	lastClickedButton = ui->pushButton;

	//����Ϣ��������������polish�׹�
	ui->textBrowser->verticalScrollBar()->style()->polish(ui->textBrowser->verticalScrollBar());

	// ��ʼ��ͼ��
	axisX->setTitleText("��������");
	axisX->setLabelFormat("%d");
	axisY->setTitleText("�в�");
	axisY->setBase(10);
	axisY->setMin(axisMinY);
	axisY->setMax(axisMaxY);
	axisX->setMin(axisMinX);
	axisX->setMax(axisMaxX);
	chart->addAxis(axisX, Qt::AlignBottom);
	chart->addAxis(axisY, Qt::AlignLeft);
	chart->setAnimationOptions(QChart::SeriesAnimations);

	// ����ͼ����Ͽ��ߺ��ҿ���
	QPen pen(QColor(196, 196, 196));
	pen.setWidth(1);
	chart->setPlotAreaBackgroundVisible(true);
	chart->setPlotAreaBackgroundPen(pen);

	ui->chartView->setChart(chart);
	ui->chartView->setRenderHint(QPainter::Antialiasing);

	// ��ʼ���в�ͼˢ�¶�ʱ��
	chartUpdateTimer->start(100);

	// �������źŴ���
	connect(ui->action1, &QAction::triggered, this, &MainWindow::handleAction1Triggered);														//��Ϣ��
	connect(ui->action2, &QAction::triggered, this, &MainWindow::handleAction2Triggered);														//x����
	connect(ui->action3, &QAction::triggered, this, &MainWindow::handleAction3Triggered);														//x����
	connect(ui->action4, &QAction::triggered, this, &MainWindow::handleAction4Triggered);														//y����
	connect(ui->action5, &QAction::triggered, this, &MainWindow::handleAction5Triggered);														//y����
	connect(ui->action6, &QAction::triggered, this, &MainWindow::handleAction6Triggered);														//z����
	connect(ui->action7, &QAction::triggered, this, &MainWindow::handleAction7Triggered);														//z����
	connect(ui->action8, &QAction::triggered, this, &MainWindow::handleAction8Triggered);														//��Ӧ����
	connect(ui->action9, &QAction::triggered, this, &MainWindow::handleAction9Triggered);														//ģ���з�
	connect(ui->action10, &QAction::triggered, this, &MainWindow::handleAction10Triggered);														//���밸��

	//�����������¼�����
	connect(playTimer, &QTimer::timeout, this, &MainWindow::onPlayTimerTimeout);																//����
	connect(reverseTimer, &QTimer::timeout, this, &MainWindow::onReverseTimerTimeout);															//����
	connect(loopPlayTimer, &QTimer::timeout, this, &MainWindow::onLoopPlayTimerTimeout);														//ѭ������
	connect(&processRun, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessRunOutput);											//������������
	connect(&processDecomposeMergeMeshes, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessDecomposeMergeMeshes);				//�ֽ�ϲ�����������
	connect(&processRun, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onProcessRunFinished);				//��������̽���
	connect(chartUpdateTimer, &QTimer::timeout, this, &MainWindow::updateChart); 																//���²в�ͼ
	planeRepModelClip->AddObserver(vtkCommand::ModifiedEvent, this, &MainWindow::updatePlaneRepModelClipValues); 				 				//����ģ���з�ƽ��ѡ������ֵ

	//����������¼�����
	connect(formGeometry, &FormGeometry::geometryImported, this, &MainWindow::formGeometry_import);												//���뼸��
	connect(formMeshImport, &FormMeshImport::meshImported, this, &MainWindow::formMeshImport_import);											//��������
	connect(formMesh, &FormMesh::meshVisibilityChanged, this, &MainWindow::formMesh_apply);														//����Ӧ��
	connect(formMesh, &FormMesh::itemEntered, this, &MainWindow::formMesh_itemEntered);															//����ҳ��Item����
	connect(formMesh, &FormMesh::itemExited, this, &MainWindow::formMesh_itemExited);															//����ҳ��Item�˳�
	connect(formSolver, &FormSolver::labelText_8_Changed, formPhysicalPropertyParameter, &FormPhysicalPropertyParameter::solverChanged);		//������ı䣬���Բ�������������
	connect(formRun, &FormRun::run, this, &MainWindow::formRun_run);																			//������
	connect(formRun, &FormRun::stopRun, this, &MainWindow::formRun_stopRun);																	//ֹͣ����
	connect(formPostprocessing, &FormPostprocessing::apply, this, &MainWindow::formPostprocessing_apply);										//������Ⱦ����
	connect(formPostprocessing, &FormPostprocessing::firstFrame, this, &MainWindow::formPostprocessing_firstFrame);								//��һ֡
	connect(formPostprocessing, &FormPostprocessing::previousFrame, this, &MainWindow::formPostprocessing_previousFrame);						//��һ֡
	connect(formPostprocessing, &FormPostprocessing::reverse, this, &MainWindow::formPostprocessing_reverse);									//���²���
	connect(formPostprocessing, &FormPostprocessing::play, this, &MainWindow::formPostprocessing_play);											//����
	connect(formPostprocessing, &FormPostprocessing::nextFrame, this, &MainWindow::formPostprocessing_nextFrame);								//��һ֡
	connect(formPostprocessing, &FormPostprocessing::lastFrame, this, &MainWindow::formPostprocessing_lastFrame);								//���һ֡
	connect(formPostprocessing, &FormPostprocessing::loopPlay, this, &MainWindow::formPostprocessing_loopPlay);									//ѭ������
	connect(formPostprocessing, &FormPostprocessing::playPause, this, &MainWindow::formPostprocessing_playPause);								//������ͣ
	connect(formPostprocessing, &FormPostprocessing::reversePause, this, &MainWindow::formPostprocessing_reversePause);							//���򲥷�
	connect(formPostprocessing, &FormPostprocessing::loopPlayPause, this, &MainWindow::formPostprocessing_loopPlayPause);						//ѭ������
	connect(formPostprocessing, &FormPostprocessing::loadData, this, &MainWindow::formPostprocessing_loadData);									//��������
	connect(formModelClip, &FormModelClip::checkBoxToggled, this, &MainWindow::formModelClip_checkBoxToggle);									//ģ���з�ҳ��CheckBox�л�
	connect(formModelClip, &FormModelClip::lineEditsChanged, this, &MainWindow::formModelClip_lineEditsChanged);								//ģ���з�ҳ��LineEditֵ�ı�
	connect(formModelClip, &FormModelClip::xPositive, this, &MainWindow::formModelClip_xPositive);												//ģ���з֣�X����
	connect(formModelClip, &FormModelClip::yPositive, this, &MainWindow::formModelClip_yPositive);												//ģ���з֣�Y����
	connect(formModelClip, &FormModelClip::zPositive, this, &MainWindow::formModelClip_zPositive);												//ģ���з֣�Z����
	connect(formModelClip, &FormModelClip::cameraDirection, this, &MainWindow::formModelClip_cameraDirection);									//ģ���з֣��������
	connect(formModelClip, &FormModelClip::alignView, this, &MainWindow::formModelClip_alignView);												//ģ���з֣������ӽ�
	connect(formModelClip, &FormModelClip::resetPlane, this, &MainWindow::formModelClip_resetPlane);											//ģ���з֣�����ƽ��
	connect(formModelClip, &FormModelClip::apply, this, &MainWindow::formModelClip_apply);														//ģ���з֣�Ӧ��
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
	QFileDialog dialog(this, tr(""), "", tr("OpenFOAM �ļ� (*.foam);;"));
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setViewMode(QFileDialog::Detail);

	dialog.setWindowModality(Qt::WindowModal);
	dialog.setWindowFlags(dialog.windowFlags() | Qt::WindowStaysOnTopHint);

	if (dialog.exec() == QDialog::Accepted) {
		caseFilePath = dialog.selectedFiles().first();
		if (caseFilePath.isEmpty()) return;
		GlobalData::getInstance().clearAllData();
		ui->textBrowser->append("Load case��" + caseFilePath);

		//����������ҳ��
		formMeshImport_import(caseFilePath);

		//���º�������ҳ��(�貹��)
		updatePostProcessingPage(caseFilePath);

		//���²�������ҳ��(�貹��)
		//formSolver->importParameter();
		//formTurbulence->importParameter();
		//formPhysicalPropertyParameter->importParameter();
		////formBoundaryConditions->importParameter();
		//formRun->importParameter();

		GlobalData::getInstance().getCaseData()->casePath = caseFilePath.toStdString();
		ui->textBrowser->append("Load case successfully!");
		QMessageBox::information(this, "��ʾ", "��������ɹ�");
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

	//��ť��ɫ����
	QPushButton* clickedButton = ui->pushButton_3;
	if (clickedButton) {
		// ��ԭ��һ������İ�ť����ɫ
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
		// ���õ�ǰ����İ�ť����ɫ
		clickedButton->setStyleSheet(
			"QPushButton {"
			"    background-color: rgb(232, 232, 232);"
			"    border: none;"
			"	 text-align: left;"
			"	 padding-left: 50px;"
			"}"
		);
		// ������һ������İ�ť
		lastClickedButton = clickedButton;
	}

	//ƽ��ѡ����
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

	// ��ʱ�����ź�
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
		QMessageBox::warning(this, tr("����"), tr("�ļ�������"));
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
		QMessageBox::warning(this, tr("����"), tr("�ļ���ʽ��֧��"));
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
	// ���� OpenFOAM ��ȡ��
	vtkSmartPointer<vtkOpenFOAMReader> openFOAMReader = vtkSmartPointer<vtkOpenFOAMReader>::New();
	openFOAMReader->SetFileName(casePath.c_str());
	openFOAMReader->SetCreateCellToPoint(1);
	openFOAMReader->SetSkipZeroTime(1);

	// ������Ϣ�Ի�ȡ��������
	openFOAMReader->UpdateInformation();
	openFOAMReader->DisableAllPatchArrays();

	// ���patchGroup���ö�Ӧ�Ĳ���
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

	// ���¶�ȡ��
	openFOAMReader->Update();

	// ʹ�� vtkCompositeDataGeometryFilter ��ȡ��һ����
	vtkSmartPointer<vtkCompositeDataGeometryFilter> geometryFilter =
		vtkSmartPointer<vtkCompositeDataGeometryFilter>::New();
	geometryFilter->SetInputConnection(openFOAMReader->GetOutputPort());
	geometryFilter->Update();

	vtkPolyData* polyData = geometryFilter->GetOutput();
	if (!polyData || polyData->GetNumberOfPoints() == 0)
	{
		return nullptr;
	}

	// ����ӳ����
	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputData(polyData);
	mapper->ScalarVisibilityOff();

	// ������һActor
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

	// ���� OpenFOAM ��ȡ��
	vtkSmartPointer<vtkOpenFOAMReader> openFOAMReader =
		vtkSmartPointer<vtkOpenFOAMReader>::New();
	openFOAMReader->SetFileName(casePath.c_str());
	openFOAMReader->SetCreateCellToPoint(1);
	openFOAMReader->SetSkipZeroTime(1);

	// ������Ϣ�Ի�ȡ��������
	openFOAMReader->UpdateInformation();

	// ��ȡ���в�������
	int numPatches = openFOAMReader->GetNumberOfPatchArrays();
	if (numPatches == 0)
	{
		std::cerr << "û���ҵ��κβ�����" << std::endl;
		return;
	}

	// �������в���
	openFOAMReader->DisableAllPatchArrays();

	// �������в�������ӽ�meshPatchNames
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
		QMessageBox::warning(this, tr("����"), tr("�ļ�������"));
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

		//������ɹ�֮���Զ���ת������ҳ��
		on_pushButton_2_clicked();
		ui->pushButton_2->setStyleSheet("QPushButton { background-color: rgb(232, 232, 232); border: none; text-align: left; padding-left: 50px; }");
		lastClickedButton->setStyleSheet("QPushButton { background-color: rgb(255, 255, 255); border: none; text-align: left; padding-left: 50px; } QPushButton:hover { background-color: rgb(242, 242, 242); }");
		lastClickedButton = ui->pushButton_2;

		//������ɹ�,��ʼ����������ҳ��(�貹��)
		//formBoundaryConditions->onMeshImported();
	}
}

void MainWindow::formMesh_apply()
{
	// �Ƴ���������ӵ���Ա
	render->RemoveAllViewProps();

	// ���� listViewModel ������ѡ��״̬��ӵ�
	std::vector<std::string> patchGroup;
	for (int i = 0; i < formMesh->listViewModel->rowCount(); ++i)
	{
		QStandardItem* item = formMesh->listViewModel->item(i);
		if (item->checkState() == Qt::Checked)
		{
			patchGroup.push_back(item->text().toStdString());
		}
	}

	// ���� meshPatchActor
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
	//// �� QString ת��Ϊ std::string
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
	//// �� QString ת��Ϊ std::string
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
		QMessageBox::warning(this, tr("����"), tr("���ȵ�������"));
		return;
	}
	//������������е����ò�������У���Ƿ����Ҫ��(�貹��)
	//formSolver->exportParameter();
	//formTurbulence->exportParameter();
	//formPhysicalPropertyParameter->exportParameter();
	////formBoundaryConditions->exportParameter();
	//formRun->exportParameter();

	//���ؿ�ʼ��ť����ʾֹͣ��ť
	QThread::msleep(500);
	formRun->ui->pushButton->hide();
	formRun->ui->pushButton_2->show();
	formRun->ui->label_12->show();

	//��ʼ���в�ͼ����
	residuals.clear();
	seriesMap.clear();
	seriesRangeMap.clear();
	currentTimeStep = 0;
	chart->removeAllSeries();
	formPostprocessing->ui->comboBox->clear();
	formPostprocessing->ui->comboBox_2->clear();
	axisMaxX = 0;
	ui->tabWidget->setCurrentIndex(1);

	//��ȡ����·��
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QFileInfo fileInfo(casePath);
	QString caseDir = fileInfo.path();
	QString controlDictPath = caseDir + "/system/controlDict";

	// ɾ��ԭ���ļ�����
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

	// �� controlDict �ļ�
	QFile controlDictFile(controlDictPath);
	if (!controlDictFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("����"), tr("�޷��� controlDict �ļ�"));
		return;
	}

	// ��ȡ controlDict �ļ��е� application �ֶ�
	QString application;
	QTextStream in(&controlDictFile);
	while (!in.atEnd()) {
		QString line = in.readLine();
		if (line.trimmed().startsWith("application")) {
			QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			if (parts.size() >= 2) {
				application = parts[1].trimmed();
				if (application.endsWith(";")) {
					application.chop(1); // ȥ��ĩβ�ķֺ�
				}
				break;
			}
		}
	}
	controlDictFile.close();

	if (application.isEmpty()) {
		QMessageBox::warning(this, tr("����"), tr("δ�ҵ� application �ֶ�"));
		return;
	}

	//���м���
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
	//���м���
	else
	{
		//�ֽ�����
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

		//����
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

	//��ȡ����·��
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
	//������Ⱦ����
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

		// ������ɫ���亯��
		vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
		colorTransferFunction->SetColorSpaceToRGB();

		// �����ɫ��
		colorTransferFunction->AddRGBPoint(globalRange.first, 0 / 255.0, 127 / 255.0, 255 / 255.0); // ��ɫ
		colorTransferFunction->AddRGBPoint((globalRange.first + globalRange.second) / 2.0, 234.0 / 255.0, 213.0 / 255.0, 201.0 / 255.0); // ��ɫ
		colorTransferFunction->AddRGBPoint(globalRange.second, 180.0 / 255.0, 0 / 255.0, 0 / 255.0); // ��ɫ

		// ����ͼ��
		vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
		scalarBar->SetLookupTable(colorTransferFunction);
		scalarBar->SetNumberOfLabels(4);
		scalarBar->SetOrientationToVertical();
		scalarBar->SetPosition(0.92, 0.01); // ����ͼ����λ��
		scalarBar->SetWidth(0.06); // ����ͼ���Ŀ�ȣ��������Ⱦ���ڵı�����
		scalarBar->SetHeight(0.3); // ����ͼ���ĸ߶ȣ��������Ⱦ���ڵı�����
		scalarBar->SetLabelFormat("%1.2e"); // ���ñ�ǩ��ʽΪ��ѧ��������������λС��

		// ����ͼ��������ı�����
		vtkSmartPointer<vtkTextProperty> titleTextProperty = vtkSmartPointer<vtkTextProperty>::New();
		titleTextProperty->SetFontSize(24); // ���ñ��������С
		titleTextProperty->SetColor(1.0, 1.0, 1.0); // ���ñ�����ɫΪ��ɫ
		titleTextProperty->SetBold(1); // ���ñ���Ϊ����
		titleTextProperty->SetJustificationToCentered(); // ���ñ�����ж���
		scalarBar->SetTitleTextProperty(titleTextProperty);

		// ����ͼ����ǩ���ı�����
		vtkSmartPointer<vtkTextProperty> labelTextProperty = vtkSmartPointer<vtkTextProperty>::New();
		labelTextProperty->SetFontSize(18); // ���ñ�ǩ�����С
		labelTextProperty->SetColor(0, 0, 0); // ���ñ�ǩ��ɫΪ��ɫ
		scalarBar->SetLabelTextProperty(labelTextProperty);

		// ���ͼ������Ⱦ��
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
	QFileDialog dialog(this, tr(""), "", tr("OpenFOAM �ļ� (*.foam);;"));
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
	//���û�пɼ���Ա��ֱ�ӷ���
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
		viewUp[1] = 1.0; // �����������Z�᷽���ϣ�����Y��Ϊ�Ϸ���
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

	// ���´���ƽ��ѡ�����ͱ�ʾ����
	planeRepModelClip = vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();
	planeWidgetModelClip = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
	planeRepModelClip->AddObserver(vtkCommand::ModifiedEvent, this, &MainWindow::updatePlaneRepModelClipValues);
	formModelClip->ui->checkBox->setChecked(true);

	// ����ƽ��ѡ�����ķ�������
	planeRepModelClip->SetPlaceFactor(1.5);

	// ��ȡ������Ա�ı߽�
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

	// ����ƽ��ѡ������ԭ��ͷ�����
	double origin[3];
	origin[0] = (bounds[0] + bounds[1]) / 2.0;
	origin[1] = (bounds[2] + bounds[3]) / 2.0;
	origin[2] = (bounds[4] + bounds[5]) / 2.0;

	double normal[3] = { 1, 1, 0 };

	// ����ƽ��ѡ������ԭ��ͷ�����
	planeRepModelClip->SetOrigin(origin);
	planeRepModelClip->SetNormal(normal);
	planeRepModelClip->PlaceWidget(bounds);

	// ����ƽ��ѡ����
	planeWidgetModelClip->SetRepresentation(planeRepModelClip);
	planeWidgetModelClip->SetInteractor(ui->openGLWidget->renderWindow()->GetInteractor());
	planeWidgetModelClip->On();

	// ��Ⱦ����
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::formModelClip_apply()
{
	if (render->GetActors()->GetNumberOfItems() == 0) {
		return;
	}

	formModelClip->ui->checkBox->setChecked(false);
	formModelClip->ui->checkBox->setChecked(true);

	// ��ȡƽ��ѡ������ԭ��ͷ�����
	double origin[3];
	double normal[3];
	planeRepModelClip->GetOrigin(origin);
	planeRepModelClip->GetNormal(normal);

	// ����һ��ƽ��
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetOrigin(origin);
	plane->SetNormal(normal);

	// ��ȡ��ǰѡ�еĻ����
	QString activeScalar = this->formPostprocessing->ui->comboBox_2->currentText();

	// �洢�зֺ����Ա
	std::vector<vtkSmartPointer<vtkActor>> clippedActors;

	// �������пɼ�����Ա�������з�
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
					// ʹ��ƽ���зֶ��������
					vtkSmartPointer<vtkClipPolyData> clipper = vtkSmartPointer<vtkClipPolyData>::New();
					clipper->SetInputData(polyData);
					clipper->SetClipFunction(plane);
					clipper->SetInsideOut(!formModelClip->ui->checkBox_2->isChecked());
					clipper->Update();

					// ��ȡ�зֺ������
					vtkSmartPointer<vtkPolyData> clippedPolyData = clipper->GetOutput();

					// �����µ�ӳ��������Ա
					vtkSmartPointer<vtkPolyDataMapper> clippedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
					clippedMapper->SetInputData(clippedPolyData);
					clippedMapper->SetLookupTable(colorTransferFunction);
					clippedMapper->SetScalarRange(range);

					vtkSmartPointer<vtkActor> clippedActor = vtkSmartPointer<vtkActor>::New();
					clippedActor->SetMapper(clippedMapper);
					clippedActor->GetProperty()->SetColor(actor_->GetProperty()->GetColor());

					// �洢�зֺ����Ա
					clippedActors.push_back(clippedActor);

					// �Ƴ�ԭʼ��Ա
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
					// ʹ�� vtkClipDataSet ��ԭʼ���ݽ��м���
					vtkSmartPointer<vtkClipDataSet> clipper = vtkSmartPointer<vtkClipDataSet>::New();
					clipper->SetInputData(dataSet);
					clipper->SetClipFunction(plane);
					clipper->SetInsideOut(!formModelClip->ui->checkBox_2->isChecked());
					clipper->Update();

					// ��ȡ���ú�ı���
					vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
					surfaceFilter->SetInputConnection(clipper->GetOutputPort());
					surfaceFilter->Update();

					vtkSmartPointer<vtkPolyData> cutSurface = surfaceFilter->GetOutput();

					// ������Ƿ���Ч
					if (cutSurface->GetNumberOfPoints() > 0)
					{
						// ���û����Ϊ��ǰѡ�е�������
						if (cutSurface->GetPointData()->HasArray(activeScalar.toStdString().c_str()))
						{
							cutSurface->GetPointData()->SetActiveScalars(activeScalar.toStdString().c_str());
						}
						else
						{
							QMessageBox::warning(this, "����", "���ú�����ݲ��������� '" + activeScalar + "'��");
							continue;
						}

						// ��ȡ������Χ����ԭʼ����һ�£�
						double scalarRange[2];
						cutSurface->GetPointData()->GetArray(activeScalar.toStdString().c_str())->GetRange(scalarRange);

						// ����ӳ��������Ա���з��棩
						vtkSmartPointer<vtkPolyDataMapper> cutMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
						cutMapper->SetInputData(cutSurface);
						cutMapper->SetLookupTable(colorTransferFunction);
						cutMapper->SetScalarRange(scalarRange);
						cutMapper->SetScalarModeToUsePointData();

						vtkSmartPointer<vtkActor> cutActor = vtkSmartPointer<vtkActor>::New();
						cutActor->SetMapper(cutMapper);

						// �����з�����Ա����
						cutActor->GetProperty()->SetOpacity(1.0);
						cutActor->GetProperty()->EdgeVisibilityOff();

						// �洢�зֺ����Ա
						clippedActors.push_back(cutActor);
					}

					// �Ƴ�ԭʼ��Ա
					render->RemoveActor(actor_);
				}
			}
		}
		actor = actors->GetNextProp();
	}

	// ���зֺ����Ա��ӵ���Ⱦ����
	for (auto& clippedActor : clippedActors)
	{
		render->AddActor(clippedActor);
	}

	// ��Ⱦ����
	ui->openGLWidget->renderWindow()->Render();
}

void MainWindow::onButtonClicked()
{
	QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
	if (clickedButton) {
		// ��ԭ��һ������İ�ť����ɫ
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
		// ���õ�ǰ����İ�ť����ɫ
		clickedButton->setStyleSheet(
			"QPushButton {"
			"    background-color: rgb(232, 232, 232);"
			"    border: none;"
			"	 text-align: left;"
			"	 padding-left: 50px;"
			"}"
		);
		// ������һ������İ�ť
		lastClickedButton = clickedButton;
	}
}

void MainWindow::onProcessRunOutput()
{
	while (processRun.canReadLine()) {
		QByteArray output = processRun.readLine();
		ui->textBrowser->append(QString::fromLocal8Bit(output));
		ui->textBrowser->repaint();

		// ���������Ϣ
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

		// ���������Ϣ
		parseOutput(QString::fromLocal8Bit(output));
		ui->tab_2->repaint();
	}
}

void MainWindow::parseOutput(const QString& output)
{
	QRegExp regex("Solving for (\\w+), Initial residual = ([\\d\\.eE\\-]+), Final residual = ([\\d\\.eE\\-]+)");
	if (output.startsWith("Time = ")) {
		// �����µ�ʱ�䲽������ QLineSeries
		for (auto it = residuals.begin(); it != residuals.end(); ++it) {
			const QString& variable = it.key();
			double initialResidual = it.value();

			// ����ñ����� QLineSeries �����ڣ��򴴽�һ���µ�
			if (!seriesMap.contains(variable)) {
				QLineSeries* newSeries = new QLineSeries();
				newSeries->setName(variable);
				seriesMap.insert(variable, newSeries);
				chart->addSeries(newSeries);
				chart->createDefaultAxes();
				seriesRangeMap.insert(variable, qMakePair(initialResidual, initialResidual));
			}

			// ���� QLineSeries
			seriesMap[variable]->append(axisMaxX, initialResidual);

			if (initialResidual < axisMinY) {
				axisMinY /= 10;
			}
			if (initialResidual > axisMaxY) {
				axisMinY *= 10;
			}
		}

		// ��յ�ǰʱ�䲽�Ĳв�����
		residuals.clear();

		// ����ʱ�䲽
		currentTimeStep = output.split("=").last().trimmed().toDouble();
		axisMaxX++;
	}

	if (regex.indexIn(output) != -1) {
		QString variable = regex.cap(1);
		double initialResidual = regex.cap(2).toDouble();

		// �洢��ǰʱ�䲽�Ĳв�����
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
		std::cerr << "û���ҵ����õ�ʱ�䲽��" << std::endl;
		return;
	}

	for (int i = 0; i < timeValues->GetNumberOfValues(); ++i) {
		timeSteps.push_back(timeValues->GetValue(i));
	}

	double lastTime = timeSteps.back();
	reader->SetTimeValue(lastTime);

	//��ȡ���patchGroup
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
		std::cerr << "�޷���ȡ�������ݡ�" << std::endl;
		return;
	}

	// �ռ���������ķ�Χ
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
	// ���� OpenFOAM ��ȡ��
	vtkSmartPointer<vtkOpenFOAMReader> openFOAMReader =
		vtkSmartPointer<vtkOpenFOAMReader>::New();
	openFOAMReader->SetFileName(casePath.c_str());
	openFOAMReader->SetCreateCellToPoint(1);
	openFOAMReader->SetSkipZeroTime(1);
	openFOAMReader->SetTimeValue(timeValue);
	openFOAMReader->UpdateInformation();

	// ��ȡ������ȫ������
	int numPatches = openFOAMReader->GetNumberOfPatchArrays();
	openFOAMReader->DisableAllPatchArrays();
	// ����ָ������
	for (int i = 0; i < numPatches; ++i) {
		const char* currentPatchName = openFOAMReader->GetPatchArrayName(i);
		for (const auto& groupPatch : patchGroup) {
			if (groupPatch == currentPatchName) {
				openFOAMReader->SetPatchArrayStatus(currentPatchName, 1);
				break;
			}
		}
	}

	// ����UPDATE_TIME_STEP
	vtkInformation* outInfo = openFOAMReader->GetOutputInformation(0);
	outInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEP(), timeValue);

	// ���¶�ȡ��
	openFOAMReader->Update();

	// ʹ�� vtkCompositeDataGeometryFilter ��ȡ��������
	vtkSmartPointer<vtkCompositeDataGeometryFilter> geometryFilter =
		vtkSmartPointer<vtkCompositeDataGeometryFilter>::New();
	geometryFilter->SetInputConnection(openFOAMReader->GetOutputPort());
	geometryFilter->Update();

	vtkPolyData* polyData = geometryFilter->GetOutput();
	if (!polyData || polyData->GetNumberOfPoints() == 0) {
		std::cerr << "�޷���ȡ�����ļ������ݡ�" << std::endl;
		return nullptr;
	}

	// ���û����
	if (!polyData->GetPointData()->HasArray(fieldName.c_str())) {
		std::cerr << "ָ���������������ڣ�" << fieldName << std::endl;
		return nullptr;
	}
	polyData->GetPointData()->SetActiveScalars(fieldName.c_str());

	// ������ɫ���亯����ʹ�ô����globalRange��
	vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction =
		vtkSmartPointer<vtkColorTransferFunction>::New();
	colorTransferFunction->SetColorSpaceToRGB();
	// �����ɫ�㣨��-��-�죩
	colorTransferFunction->AddRGBPoint(globalRange.first, 0.0, 127.0 / 255.0, 1.0);
	colorTransferFunction->AddRGBPoint(
		(globalRange.first + globalRange.second) / 2.0,
		234.0 / 255.0, 213.0 / 255.0, 201.0 / 255.0);
	colorTransferFunction->AddRGBPoint(globalRange.second, 180.0 / 255.0, 0.0, 0.0);

	// ����ӳ������ʹ���Զ��巶Χ
	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputData(polyData);
	mapper->SetLookupTable(colorTransferFunction);
	mapper->UseLookupTableScalarRangeOn();
	mapper->SetScalarRange(globalRange.first, globalRange.second);
	mapper->ScalarVisibilityOn();

	// ����actor
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOff();
	actor->GetProperty()->SetRepresentationToSurface();

	return actor;
}

void MainWindow::updatePostProcessingPage(const QString& casePath)
{
	//��Ҫ���֮ǰ������Ϣ���貹�䣩
	if (GlobalData::getInstance().getCaseData()->casePath != casePath.toStdString()) {
		GlobalData::getInstance().clearAllData();
		GlobalData::getInstance().getCaseData()->casePath = casePath.toStdString();
	}

	getNephogramPatchData(casePath.toStdString());
	formPostprocessing->ui->comboBox->clear();
	formPostprocessing->ui->comboBox_2->clear();
	formPostprocessing->listViewModel->clear();

	// ����ʱ�䲽��������������
	QStringList timeStepList;
	std::vector<double> timeSteps = GlobalData::getInstance().getCaseData()->timeSteps;
	for (const double& timeStep : timeSteps) timeStepList.append(QString::number(timeStep));
	if (timeStepList.size() == 0) return;
	formPostprocessing->ui->comboBox->addItems(timeStepList);

	// ����������������
	QStringList fieldNameList;
	std::vector<std::string> fieldName = GlobalData::getInstance().getCaseData()->fieldName;
	for (const std::string& field : fieldName) fieldNameList.append(QString::fromStdString(field));
	if (fieldNameList.size() == 0) return;
	formPostprocessing->ui->comboBox_2->addItems(fieldNameList);

	//���²������б�
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

	// �������� item ���ܸ߶�
	int totalHeight = 0;
	for (int i = 0; i < formPostprocessing->listViewModel->rowCount(); ++i) {
		totalHeight += formPostprocessing->ui->listView->sizeHintForRow(i);
	}
	formPostprocessing->ui->listView->setFixedHeight(totalHeight + 2 * formPostprocessing->ui->listView->frameWidth());

	//������Ⱦ����
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

		// ������ɫ���亯��
		vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
		colorTransferFunction->SetColorSpaceToRGB();

		// �����ɫ��
		colorTransferFunction->AddRGBPoint(globalRange.first, 0 / 255.0, 127 / 255.0, 255 / 255.0); // ��ɫ
		colorTransferFunction->AddRGBPoint((globalRange.first + globalRange.second) / 2.0, 234.0 / 255.0, 213.0 / 255.0, 201.0 / 255.0); // ��ɫ
		colorTransferFunction->AddRGBPoint(globalRange.second, 180.0 / 255.0, 0 / 255.0, 0 / 255.0); // ��ɫ

		// ����ͼ��
		vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
		scalarBar->SetLookupTable(colorTransferFunction);
		scalarBar->SetNumberOfLabels(4);
		scalarBar->SetOrientationToVertical();
		scalarBar->SetPosition(0.92, 0.01); // ����ͼ����λ��
		scalarBar->SetWidth(0.06); // ����ͼ���Ŀ�ȣ��������Ⱦ���ڵı�����
		scalarBar->SetHeight(0.3); // ����ͼ���ĸ߶ȣ��������Ⱦ���ڵı�����
		scalarBar->SetLabelFormat("%1.2e"); // ���ñ�ǩ��ʽΪ��ѧ��������������λС��

		// ����ͼ��������ı�����
		vtkSmartPointer<vtkTextProperty> titleTextProperty = vtkSmartPointer<vtkTextProperty>::New();
		titleTextProperty->SetFontSize(24); // ���ñ��������С
		titleTextProperty->SetColor(1.0, 1.0, 1.0); // ���ñ�����ɫΪ��ɫ
		titleTextProperty->SetBold(1); // ���ñ���Ϊ����
		titleTextProperty->SetJustificationToCentered(); // ���ñ�����ж���
		scalarBar->SetTitleTextProperty(titleTextProperty);

		// ����ͼ����ǩ���ı�����
		vtkSmartPointer<vtkTextProperty> labelTextProperty = vtkSmartPointer<vtkTextProperty>::New();
		labelTextProperty->SetFontSize(18); // ���ñ�ǩ�����С
		labelTextProperty->SetColor(0, 0, 0); // ���ñ�ǩ��ɫΪ��ɫ
		scalarBar->SetLabelTextProperty(labelTextProperty);

		// ���ͼ������Ⱦ��
		render->AddActor2D(scalarBar);

		render->ResetCamera();
		renderWindow->Render();
	}

	//�л���������ҳ��
	on_pushButton_17_clicked();
	ui->pushButton_17->setStyleSheet("QPushButton { background-color: rgb(232, 232, 232); border: none; text-align: left; padding-left: 50px; }");
	lastClickedButton->setStyleSheet("QPushButton { background-color: rgb(255, 255, 255); border: none; text-align: left; padding-left: 50px; } QPushButton:hover { background-color: rgb(242, 242, 242); }");
	lastClickedButton = ui->pushButton_17;
}

void MainWindow::updateChart()
{
	chart->axisX()->setTitleText("��������");
	chart->axisX()->setRange(axisMinX, axisMaxX - 1);

	// ȷ�� axisY ����Ϊ��������ϵ
	QLogValueAxis* logAxisY = new QLogValueAxis();
	logAxisY->setTitleText("�в�");
	logAxisY->setBase(10);
	logAxisY->setRange(axisMinY, axisMaxY);
	logAxisY->setLabelFormat("%.1e");
	chart->setAxisY(logAxisY);

	// ���ú�����Ϊ����
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