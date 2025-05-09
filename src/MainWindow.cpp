/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);

	//窗口设置到屏幕中间
	int screenWidth = QApplication::desktop()->screen()->width();
	int screenHeight = QApplication::desktop()->screen()->height();
	int windowWidth = this->width();
	int windowHeight = this->height();
	int x = (screenWidth - windowWidth) / 2;
	int y = (screenHeight - windowHeight) / 2;

	//禁用窗口拉伸
	this->setFixedSize(windowWidth, windowHeight);

	// 设置背景图
	this->setStyleSheet(
		"QMainWindow {"
		"    background-image: url(../res/BackgroundImage1);"
		"    background-position: center;"
		"}"
	);

	//让界面上的所有的widget都设置为半透明
	for (int i = 0; i < ui->centralWidget->children().size(); ++i)
	{
		QWidget* widget = qobject_cast<QWidget*>(ui->centralWidget->children().at(i));
		if (widget && widget != ui->centralWidget)
		{
			QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
			effect->setOpacity(0.9);
			widget->setGraphicsEffect(effect);
		}
	}
	QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
	effect->setOpacity(0.8);
	ui->pushButton_4->setGraphicsEffect(effect);

	connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_PushButton_clicked);
	connect(ui->pushButton_2, &QPushButton::toggled, this, &MainWindow::on_PushButton_2_Toggled);
	connect(ui->pushButton_6, &QPushButton::toggled, this, &MainWindow::on_PushButton_6_Toggled);
	connect(ui->pushButton_9, &QPushButton::clicked, this, &MainWindow::on_PushButton_9_clicked);
	connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::on_PushButton_3_clicked);
	connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::on_PushButton_8_clicked);
	connect(ui->pushButton_7, &QPushButton::clicked, this, &MainWindow::on_PushButton_7_clicked);
	connect(ui->pushButton_10, &QPushButton::clicked, this, &MainWindow::on_PushButton_10_clicked);
	connect(ui->pushButton_11, &QPushButton::clicked, this, &MainWindow::on_PushButton_11_clicked);
	connect(ui->pushButton_12, &QPushButton::clicked, this, &MainWindow::on_PushButton_12_clicked);
	connect(ui->pushButton_13, &QPushButton::clicked, this, &MainWindow::on_PushButton_13_clicked);
	connect(ui->pushButton_14, &QPushButton::clicked, this, &MainWindow::on_PushButton_14_clicked);
	connect(ui->pushButton_15, &QPushButton::clicked, this, &MainWindow::on_PushButton_15_clicked);
	connect(ui->pushButton_16, &QPushButton::clicked, this, &MainWindow::on_PushButton_16_clicked);
	connect(ui->pushButton_17, &QPushButton::clicked, this, &MainWindow::on_PushButton_17_clicked);
	connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_PushButton_4_clicked);

	on_PushButton_2_Toggled(false);
	on_PushButton_6_Toggled(false);

	ui->pushButton->installEventFilter(this); 
	ui->pushButton_2->installEventFilter(this);
	ui->pushButton_6->installEventFilter(this);
	ui->pushButton_9->installEventFilter(this);
	ui->pushButton_3->installEventFilter(this);
	ui->pushButton_8->installEventFilter(this);
	ui->pushButton_7->installEventFilter(this);
	ui->pushButton_10->installEventFilter(this);
	ui->pushButton_11->installEventFilter(this);
	ui->pushButton_12->installEventFilter(this);
	ui->pushButton_13->installEventFilter(this);
	ui->pushButton_14->installEventFilter(this);
	ui->pushButton_15->installEventFilter(this);
	ui->pushButton_16->installEventFilter(this);
	ui->pushButton_17->installEventFilter(this);
	ui->pushButton_4->installEventFilter(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::subroutine(QString path)
{
	if (path.startsWith("http"))
	{
		QUrl url(path);
		QDesktopServices::openUrl(url);
	}
	else if (path.endsWith(".exe"))
	{
		QFileInfo fileInfo(path);
		QString workingDirectory = fileInfo.absolutePath();
		QProcess* process = new QProcess(this);
		process->setWorkingDirectory(workingDirectory);
		process->start("\"" + path + "\"");
	}
}

void MainWindow::on_PushButton_clicked()
{
	QString filePath = "../config/MainWindow";

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, "错误", "无法打开文件: " + filePath);
		return;
	}

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString line = in.readLine();

		if (line.startsWith("ZongHeGuanLiRuanJian"))
		{
			int colonIndex = line.indexOf(':');
			if (colonIndex != -1)
			{
				QString result = line.mid(colonIndex + 1).trimmed();
				subroutine(result);
				return;
			}
		}
	}

	QMessageBox::warning(this, "提示", "请先配置“综合管理软件”路径");
}


void MainWindow::on_PushButton_2_Toggled(bool checked)
{
	ui->widget_4->setVisible(checked);
}

void MainWindow::on_PushButton_6_Toggled(bool checked)
{
	ui->widget_5->setVisible(checked);
}

void MainWindow::on_PushButton_9_clicked()
{
}

void MainWindow::on_PushButton_3_clicked()
{
}

void MainWindow::on_PushButton_8_clicked()
{
}

void MainWindow::on_PushButton_7_clicked()
{
}

void MainWindow::on_PushButton_10_clicked()
{
	QString filePath = "../config/MainWindow";

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, "错误", "无法打开文件: " + filePath);
		return;
	}

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString line = in.readLine();

		if (line.startsWith("ZhiShiTuPu"))
		{
			int colonIndex = line.indexOf(':');
			if (colonIndex != -1)
			{
				QString result = line.mid(colonIndex + 1).trimmed();
				subroutine(result);
				return;
			}
		}
	}

	QMessageBox::warning(this, "提示", "请先配置“知识图谱”路径");
}

void MainWindow::on_PushButton_11_clicked()
{
	QString filePath = "../config/MainWindow";

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, "错误", "无法打开文件: " + filePath);
		return;
	}

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString line = in.readLine();

		if (line.startsWith("TMAX3D"))
		{
			int colonIndex = line.indexOf(':');
			if (colonIndex != -1)
			{
				QString result = line.mid(colonIndex + 1).trimmed();
				subroutine(result);
				return;
			}
		}
	}

	QMessageBox::warning(this, "提示", "请先配置“TMAX3D”路径");
}

void MainWindow::on_PushButton_12_clicked()
{
	CurvePlotting* curvePlotting = new CurvePlotting();
	curvePlotting->show();
}

void MainWindow::on_PushButton_13_clicked()
{
	QString program = "../Subroutine/SY_1D_Heatload/SY_1D_Q.exe";
	QFileInfo fileInfo(program);
	QString workingDirectory = fileInfo.absolutePath();
	QProcess* process = new QProcess(this);
	process->setWorkingDirectory(workingDirectory);
	process->start(program);
}


void MainWindow::on_PushButton_14_clicked()
{
	ThreeDimensionalComputation* threeDimensionalComputation = new ThreeDimensionalComputation();
	threeDimensionalComputation->show();
}

void MainWindow::on_PushButton_15_clicked()
{
	QString program = "../Subroutine/SY_3dHeatLoad/SY_GUI_3dHeatLoad.exe";
	QFileInfo fileInfo(program);
	QString workingDirectory = fileInfo.absolutePath();
	QProcess* process = new QProcess(this);
	process->setWorkingDirectory(workingDirectory);
	process->start(program);
}

void MainWindow::on_PushButton_16_clicked()
{
	QString program = "../Subroutine/SY_3dHeatLoad/SY_GUI_3dHeatLoad.exe";
	QFileInfo fileInfo(program);
	QString workingDirectory = fileInfo.absolutePath();
	QProcess* process = new QProcess(this);
	process->setWorkingDirectory(workingDirectory);
	process->start(program);
}

void MainWindow::on_PushButton_17_clicked()
{
	ThreeDimensionalComputation* threeDimensionalComputation = new ThreeDimensionalComputation();
	threeDimensionalComputation->show();
}

void MainWindow::on_PushButton_4_clicked()
{
	MainWindowSetting* mainWindowSetting = new MainWindowSetting(); 

	// 设置窗口为模态对话框
	mainWindowSetting->setWindowModality(Qt::ApplicationModal);

	// 将窗口移动到当前窗口的中心
	int x = this->x() + (this->width() - mainWindowSetting->width()) / 2;
	int y = this->y() + (this->height() - mainWindowSetting->height()) / 2;
	mainWindowSetting->move(x, y);

	// 显示窗口
	mainWindowSetting->show();
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
	// 用于保存每个按钮的初始大小，防止反复进入离开时累积误差
	static QMap<QPushButton*, QSize> originalSizes;

	QPushButton* button = qobject_cast<QPushButton*>(watched);
	if (!button)
		return QMainWindow::eventFilter(watched, event);

	// 如果还没记录过该按钮的原始大小，则进行记录
	if (!originalSizes.contains(button))
		originalSizes[button] = button->size();

	// 读取按钮的原始大小
	QSize oriSize = originalSizes[button];

	if (event->type() == QEvent::Enter)
	{
		// 放大后的目标大小
		QSize endSize(int(oriSize.width() * 1.1),
			int(oriSize.height() * 1.1));

		// 同时动画修改 minimumSize & maximumSize
		QPropertyAnimation* animMin = new QPropertyAnimation(button, "minimumSize");
		animMin->setDuration(200);
		animMin->setStartValue(oriSize);
		animMin->setEndValue(endSize);
		animMin->setEasingCurve(QEasingCurve::OutQuad);
		animMin->start(QAbstractAnimation::DeleteWhenStopped);

		QPropertyAnimation* animMax = new QPropertyAnimation(button, "maximumSize");
		animMax->setDuration(200);
		animMax->setStartValue(oriSize);
		animMax->setEndValue(endSize);
		animMax->setEasingCurve(QEasingCurve::OutQuad);
		animMax->start(QAbstractAnimation::DeleteWhenStopped);

		return true;
	}
	else if (event->type() == QEvent::Leave)
	{
		// 缩回至原始大小
		QSize startSize = button->size();
		QSize endSize = oriSize; // 恢复到原始大小

		QPropertyAnimation* animMin = new QPropertyAnimation(button, "minimumSize");
		animMin->setDuration(200);
		animMin->setStartValue(startSize);
		animMin->setEndValue(endSize);
		animMin->setEasingCurve(QEasingCurve::OutQuad);
		animMin->start(QAbstractAnimation::DeleteWhenStopped);

		QPropertyAnimation* animMax = new QPropertyAnimation(button, "maximumSize");
		animMax->setDuration(200);
		animMax->setStartValue(startSize);
		animMax->setEndValue(endSize);
		animMax->setEasingCurve(QEasingCurve::OutQuad);
		animMax->start(QAbstractAnimation::DeleteWhenStopped);

		return true;
	}

	return QMainWindow::eventFilter(watched, event);
}


