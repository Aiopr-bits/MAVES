#pragma once
#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
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
	formBoundaryConditions = new FormBoundaryConditions(this);
	formRun = new FormRun(this);
	formPostprocessing = new FormPostprocessing(this);
	formModelClip = new FormModelClip(this);
	ui->gridLayout_3->addWidget(formGeometry, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formMesh, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formMeshImport, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formBoundaryConditions, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formRun, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formPostprocessing, 0, 0, 1, 1);
	ui->gridLayout_3->addWidget(formModelClip, 0, 0, 1, 1);
	formGeometry->hide();
	formMesh->hide();
	formMeshImport->hide();
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
	connect(this, &MainWindow::meshImported, this, &MainWindow::onMeshImported);																//���������
	connect(playTimer, &QTimer::timeout, this, &MainWindow::onPlayTimerTimeout);																//����
	connect(reverseTimer, &QTimer::timeout, this, &MainWindow::onReverseTimerTimeout);															//����
	connect(loopPlayTimer, &QTimer::timeout, this, &MainWindow::onLoopPlayTimerTimeout);														//ѭ������
	connect(&processRun, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessRunOutput);											//������������
	connect(&processFoamToVTK, &QProcess::readyReadStandardOutput, this, &MainWindow::onprocessFoamToVTKOutput);								//foamToVTK�������
	connect(&process, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessOutput);													//�������
	connect(&process, &QProcess::readyReadStandardError, this, &MainWindow::onProcessError);													//���̴���
	connect(&processRun, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onProcessRunFinished);				//��������̽���
	connect(&processFoamToVTK, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onProcessFoamToVTKFinished);	//foamToVTK���̽���
	connect(chartUpdateTimer, &QTimer::timeout, this, &MainWindow::updateChart); 																//���²в�ͼ
	planeRepModelClip->AddObserver(vtkCommand::ModifiedEvent, this, &MainWindow::updatePlaneRepModelClipValues); 				 				//����ģ���з�ƽ��ѡ������ֵ

	//����������¼�����
	connect(formGeometry, &FormGeometry::geometryImported, this, &MainWindow::formGeometry_import);												//���뼸��
	connect(formMeshImport, &FormMeshImport::meshImported, this, &MainWindow::formMeshImport_import);											//��������
	connect(formMesh, &FormMesh::meshVisibilityChanged, this, &MainWindow::formMesh_apply);														//����Ӧ��
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
	connect(formPostprocessing, &FormPostprocessing::reversePause, this, &MainWindow::formPostprocessing_reversePause);							//���򲥷���ͣ
	connect(formPostprocessing, &FormPostprocessing::loopPlayPause, this, &MainWindow::formPostprocessing_loopPlayPause);						//ѭ��������ͣ
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
	QFileDialog dialog(this, tr("���غ�������"), "", tr("OpenFOAM �ļ� (*.foam);;"));
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setViewMode(QFileDialog::Detail);

	dialog.setWindowModality(Qt::WindowModal);
	dialog.setWindowFlags(dialog.windowFlags() | Qt::WindowStaysOnTopHint);

	if (dialog.exec() == QDialog::Accepted) {
		caseFilePath = dialog.selectedFiles().first();
		if (caseFilePath.isEmpty()) return;

		GlobalData::getInstance().clearAllData();

		ui->textBrowser->append("Load case��" + caseFilePath);
		//�������������Ϣ(�貹��)
		GlobalData::getInstance().getCaseData()->casePath = caseFilePath.toStdString();

		//����ҳ����ʾ(�貹��)
		updatePostProcessingPage(caseFilePath);

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

void MainWindow::formMeshImport_import(const QString& filePath)
{
	QFileInfo fileInfo(filePath);
	if (!fileInfo.exists())
	{
		QMessageBox::warning(this, tr("����"), tr("�ļ�������"));
		return;
	}

	QString type = fileInfo.suffix().toLower();
	GlobalData::getInstance().clearAllData();
	render->RemoveAllViewProps();

	if (type == "foam")
	{
		QString casePath = fileInfo.path();
		QString folderName = casePath.split("/").last();
		QString vtpPath = casePath + "/VTK/" + folderName + "_0/boundary/";

		//���VTK�����ļ�������ת��
		QDir vtpDir(vtpPath);
		if (!vtpDir.exists())
		{
			std::string command = "foamToVTK -time 0 -case " + casePath.toStdString();

			process.setProgram("cmd.exe");
			process.setArguments(QStringList() << "/C" << QString::fromStdString(command));
			process.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments* args) {
				args->flags |= CREATE_NO_WINDOW;
				});
			process.start();
			process.waitForFinished();

			// �ȴ��ļ�д�����
			QString vtmFilePath = casePath + "/VTK/" + folderName + "_0.vtm";
			QFile vtmFile(vtmFilePath);
			while (!vtmFile.exists()) {
				QThread::msleep(100);
			}

			// ����������õ��ļ��Ƿ����
			QFile file(vtmFilePath);
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				qWarning() << "�޷����ļ�:" << vtmFilePath;
				return;
			}

			QXmlStreamReader xml(&file);
			bool allFilesExist = true;
			while (!xml.atEnd() && !xml.hasError()) {
				QXmlStreamReader::TokenType token = xml.readNext();
				if (token == QXmlStreamReader::StartElement) {
					if (xml.name() == "DataSet") {
						QString filePath = casePath + "/VTK/" + xml.attributes().value("file").toString();
						if (!QFile::exists(filePath)) {
							allFilesExist = false;
							break;
						}
					}
				}
			}
			file.close();

			// ��������ļ������ڣ������
			if (!allFilesExist) {
				qWarning() << "�����ļ������ڣ��ȴ���...";
				QThread::msleep(100);
			}
		}

		// ��ȡvtpPath�ļ����µ�����.vtp�ļ�·��
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

		//������ɹ�֮���Զ���ת������ҳ��
		on_pushButton_2_clicked();
		ui->pushButton_2->setStyleSheet("QPushButton { background-color: rgb(232, 232, 232); border: none; text-align: left; padding-left: 50px; }");
		lastClickedButton->setStyleSheet("QPushButton { background-color: rgb(255, 255, 255); border: none; text-align: left; padding-left: 50px; } QPushButton:hover { background-color: rgb(242, 242, 242); }");
		lastClickedButton = ui->pushButton_2;

		emit meshImported();
	}
}

void MainWindow::formMesh_apply()
{
	render->RemoveAllViewProps();
	const auto& meshFaceActors = GlobalData::getInstance().getCaseData()->meshFaceActors;
	const auto& meshEdgeActors = GlobalData::getInstance().getCaseData()->meshEdgeActors;

	// ���� treeView ������ actor
	for (int i = 0; i < formMesh->listViewModel->rowCount(); ++i)
	{
		QStandardItem* item = formMesh->listViewModel->item(i);
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
	if (GlobalData::getInstance().getCaseData()->casePath.empty())
	{
		QMessageBox::warning(this, tr("����"), tr("���ȵ�������"));
		return;
	}
	//������������е����ò�������У���Ƿ����Ҫ��(�貹��)



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

	// ������ִ������
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

	//ɾ��ԭ����VTK�ļ���
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
	QString caseFilePath = QString::fromStdString(GlobalData::getInstance().getCaseData()->casePath);
	updatePostProcessingPage(caseFilePath);
}

void MainWindow::formRun_stopRun()
{
	QThread::msleep(500);
	formRun->ui->pushButton_2->hide();
	formRun->ui->label_12->hide();
	formRun->ui->pushButton->show();

	if (processRun.state() == QProcess::Running) {
		processRun.kill();
	}
}

std::tuple<vtkSmartPointer<vtkActor>, vtkSmartPointer<vtkColorTransferFunction>, std::array<double, 2>> createActorFromFile(const QString& filePath, const QString& variableName)
{
	vtkSmartPointer<vtkAlgorithm> reader;

	// �����ļ���չ��ѡ����ʵĶ�ȡ��
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

	// �����ļ��������¶�ȡ��
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

	// ��ȡ���ݼ�
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

	// ����Ƿ����ָ����������
	if (!dataSet->GetPointData()->HasArray(variableName.toStdString().c_str())) {
		return std::make_tuple(nullptr, nullptr, std::array<double, 2>{0.0, 1.0});
	}

	// ������ɫ���亯��
	vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	colorTransferFunction->SetColorSpaceToRGB();

	// ������ɫӳ�䷶Χ
	double range[2];
	dataSet->GetPointData()->GetArray(variableName.toStdString().c_str())->GetRange(range);

	// �����ɫ��
	colorTransferFunction->AddRGBPoint(range[0], 0 / 255.0, 127 / 255.0, 255 / 255.0); // ��ɫ
	colorTransferFunction->AddRGBPoint((range[0] + range[1]) / 2.0, 234.0 / 255.0, 213.0 / 255.0, 201.0 / 255.0); // ��ɫ
	colorTransferFunction->AddRGBPoint(range[1], 180.0 / 255.0, 0 / 255.0, 0 / 255.0); // ��ɫ

	// ���ñ�������Ϊָ����������
	dataSet->GetPointData()->SetScalars(dataSet->GetPointData()->GetArray(variableName.toStdString().c_str()));

	// ������ӳ����
	vtkSmartPointer<vtkDataSetMapper> surfaceMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	surfaceMapper->SetInputData(dataSet);
	surfaceMapper->SetLookupTable(colorTransferFunction);
	surfaceMapper->SetScalarRange(range);

	// ��������Ա
	vtkSmartPointer<vtkActor> surfaceActor = vtkSmartPointer<vtkActor>::New();
	surfaceActor->SetMapper(surfaceMapper);

	return std::make_tuple(surfaceActor, colorTransferFunction, std::array<double, 2>{range[0], range[1]});
}

void createLengendFromFile()
{
	QFileInfo fileInfo(GlobalData::getInstance().getCaseData()->casePath.c_str());
	QString caseDirPath = fileInfo.absolutePath();

	const QStringList& filePaths = GlobalData::getInstance().getCaseData()->timeFilePairs.last().second;
	QMap<QString, vtkSmartPointer<vtkScalarBarActor>> scalarBars;

	for (const QString& variableName : GlobalData::getInstance().getCaseData()->fieldNames) {
		double globalRange[2] = { std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest() };

		for (const QString& filePath : filePaths) {
			if (!filePath.endsWith(".vtp", Qt::CaseInsensitive)) {
				continue;
			}

			vtkSmartPointer<vtkAlgorithm> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
			vtkXMLPolyDataReader::SafeDownCast(reader)->SetFileName((caseDirPath + "/VTK/" + filePath).toStdString().c_str());
			reader->Update();

			vtkSmartPointer<vtkPolyData> polyData = vtkPolyData::SafeDownCast(reader->GetOutputDataObject(0));
			if (!polyData || !polyData->GetPointData()->HasArray(variableName.toStdString().c_str())) {
				continue;
			}

			double range[2];
			polyData->GetPointData()->GetArray(variableName.toStdString().c_str())->GetRange(range);

			if (range[0] < globalRange[0]) {
				globalRange[0] = range[0];
			}
			if (range[1] > globalRange[1]) {
				globalRange[1] = range[1];
			}
		}

		// ������ɫ���亯��
		vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
		colorTransferFunction->SetColorSpaceToRGB();

		// �����ɫ��
		colorTransferFunction->AddRGBPoint(globalRange[0], 0 / 255.0, 127 / 255.0, 255 / 255.0); // ��ɫ
		colorTransferFunction->AddRGBPoint((globalRange[0] + globalRange[1]) / 2.0, 234.0 / 255.0, 213.0 / 255.0, 201.0 / 255.0); // ��ɫ
		colorTransferFunction->AddRGBPoint(globalRange[1], 180.0 / 255.0, 0 / 255.0, 0 / 255.0); // ��ɫ

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

		// ��ͼ����ӵ�������
		scalarBars.insert(variableName, scalarBar);
	}

	GlobalData::getInstance().getCaseData()->scalarBars = scalarBars;
}

void MainWindow::formPostprocessing_apply()
{
	double time = formPostprocessing->ui->comboBox->currentText().toDouble();
	QString variableName = formPostprocessing->ui->comboBox_2->currentText();

	// ��ȡ QTreeView ��ģ��
	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(formPostprocessing->ui->listView->model());
	if (!model) {
		return;
	}

	// ����ģ���е�������
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

	// ���ͼ��
	for (auto it = GlobalData::getInstance().getCaseData()->scalarBars.begin(); it != GlobalData::getInstance().getCaseData()->scalarBars.end(); ++it) {
		if (variableName == it.key()) {
			render->AddActor2D(it.value());
		}
	}

	// ��Ⱦ
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

		// ���������Ϣ������ͼ��
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

		// ���������Ϣ������ͼ��
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

		// ���������Ϣ������ͼ��
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

void MainWindow::updatePostProcessingPage(const QString& casePath)
{
	QFileInfo fileInfo(casePath);
	QString caseDirPath = fileInfo.absolutePath();
	QString caseDirName = fileInfo.dir().dirName();
	if (!QDir(caseDirPath + "/VTK").exists()) {
		formPostprocessing->ui->comboBox->clear();
		formPostprocessing->ui->comboBox_2->clear();
		formPostprocessing->listViewModel->clear();
		render->RemoveAllViewProps();
		renderWindow->Render();
		return;
	}

	// ����seriesϵ���ļ�����ȡʱ����ļ�·��
	QString seriesPath = caseDirPath + "/VTK/" + caseDirName + ".vtm.series";
	QFile seriesFile(seriesPath);
	if (!seriesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("����"), tr("�޷���ϵ���ļ�"));
		return;
	}

	QByteArray seriesData = seriesFile.readAll();
	seriesFile.close();

	QJsonDocument jsonDoc = QJsonDocument::fromJson(seriesData);
	if (jsonDoc.isNull() || !jsonDoc.isObject()) {
		QMessageBox::warning(this, tr("����"), tr("seriesϵ���ļ���ʽ����"));
		return;
	}

	QJsonObject jsonObj = jsonDoc.object();
	QJsonArray filesArray = jsonObj["files"].toArray();

	QList<QPair<double, QStringList>> timeFilePairs; // ���ڴ洢ʱ����ļ�·��������

	for (const QJsonValue& value : filesArray) {
		QJsonObject fileObj = value.toObject();
		double time = fileObj["time"].toDouble();
		QString name = fileObj["name"].toString();

		if (time != 0) {
			QString vtmFilePath = caseDirPath + "/VTK/" + name;
			QFile vtmFile(vtmFilePath);
			if (!vtmFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QMessageBox::warning(this, tr("����"), tr("�޷���VTM�ļ�: ") + vtmFilePath);
				continue;
			}

			QByteArray vtmData = vtmFile.readAll();
			vtmFile.close();

			QStringList filePaths;
			QXmlStreamReader xmlReader(vtmData);
			while (!xmlReader.atEnd() && !xmlReader.hasError()) {
				QXmlStreamReader::TokenType token = xmlReader.readNext();
				if (token == QXmlStreamReader::StartElement) {
					if (xmlReader.name() == "DataSet") {
						QXmlStreamAttributes attributes = xmlReader.attributes();
						QString filePath = attributes.value("file").toString();
						filePaths.append(filePath);
					}
				}
			}

			if (xmlReader.hasError()) {
				QMessageBox::warning(this, tr("����"), tr("����VTM�ļ�ʱ����: ") + xmlReader.errorString());
			}

			timeFilePairs.append(qMakePair(time, filePaths));
		}
	}

	if (timeFilePairs.isEmpty()) {
		formPostprocessing->ui->comboBox->clear();
		formPostprocessing->ui->comboBox_2->clear();
		formPostprocessing->listViewModel->clear();
		render->RemoveAllViewProps();
		renderWindow->Render();
		return;
	}

	//��ȡ�����б�
	QString lastFilePath = caseDirPath + "/VTK/" + timeFilePairs.last().second.last();
	QFile lastFile(lastFilePath);
	if (!lastFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("����"), tr("�޷����ļ�: ") + lastFilePath);
		return;
	}

	QByteArray lastFileData = lastFile.readAll();
	lastFile.close();

	vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
	reader->SetFileName(lastFilePath.toStdString().c_str());
	reader->Update();

	vtkSmartPointer<vtkPolyData> polyData = reader->GetOutput();
	vtkSmartPointer<vtkPointData> pointData = polyData->GetPointData();

	QStringList fieldNames;
	for (int i = 0; i < pointData->GetNumberOfArrays(); ++i) {
		fieldNames.append(pointData->GetArrayName(i));
	}

	//���½���ؼ�
	formPostprocessing->ui->comboBox->clear();
	for (const auto& pair : timeFilePairs) {
		formPostprocessing->ui->comboBox->addItem(QString::number(pair.first));
	}

	formPostprocessing->ui->comboBox_2->clear();
	formPostprocessing->ui->comboBox_2->addItems(fieldNames);

	QStringList lastFileBaseNames;
	for (const QString& filePath : timeFilePairs.last().second) {
		QFileInfo fileInfo(filePath);
		lastFileBaseNames.append(fileInfo.baseName());
	}

	formPostprocessing->listViewModel->clear();
	foreach(const QString & baseName, lastFileBaseNames) {
		QStandardItem* item = new QStandardItem(baseName);
		item->setCheckable(true);
		if (baseName != "internal") item->setCheckState(Qt::Checked);
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

	GlobalData::getInstance().getCaseData()->timeFilePairs = timeFilePairs;
	GlobalData::getInstance().getCaseData()->fieldNames = fieldNames;

	//������ά��ʾ����
	double time = formPostprocessing->ui->comboBox->currentText().toDouble();
	QString variableName = formPostprocessing->ui->comboBox_2->currentText();
	render->RemoveAllViewProps();
	for (const QString& baseName : lastFileBaseNames) {
		if (baseName != "internal") {
			QString filePath = caseDirPath + "/VTK/" + caseDirName + "_" + QString::number(time) + "/boundary/" + baseName + ".vtp";
			auto result = createActorFromFile(filePath, variableName);
			vtkSmartPointer<vtkActor> actor = std::get<0>(result);
			if (actor) {
				render->AddActor(actor);
			}
		}
	}

	createLengendFromFile();

	// ���ͼ��
	for (auto it = GlobalData::getInstance().getCaseData()->scalarBars.begin(); it != GlobalData::getInstance().getCaseData()->scalarBars.end(); ++it) {
		if (variableName == it.key()) {
			render->AddActor2D(it.value());
		}
	}

	render->ResetCamera();
	renderWindow->Render();

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

void MainWindow::onProcessError()
{
	//while (process.canReadLine()) {
	//	QByteArray error = process.readLine();
	//	ui->textBrowser->append(QString::fromLocal8Bit(error));
	//	ui->textBrowser->repaint(); 
	//}
}

void MainWindow::onMeshImported()
{
	QMessageBox::information(this, "��ʾ", "������ɹ�");

	//(������ɾ��)
	GlobalData::getInstance().getCaseData()->casePath = formMeshImport->ui->lineEdit->text().toStdString();

	formBoundaryConditions->onMeshImported();
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