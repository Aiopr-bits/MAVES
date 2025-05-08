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

	on_PushButton_2_Toggled(false);
	on_PushButton_6_Toggled(false);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_PushButton_clicked()
{
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
}

void MainWindow::on_PushButton_11_clicked()
{
}

void MainWindow::on_PushButton_12_clicked()
{
}

void MainWindow::on_PushButton_13_clicked()
{
}

void MainWindow::on_PushButton_14_clicked()
{
	ThreeDimensionalComputation* threeDimensionalComputation = new ThreeDimensionalComputation();
	threeDimensionalComputation->show();
}

void MainWindow::on_PushButton_15_clicked()
{
}

void MainWindow::on_PushButton_16_clicked()
{
}

void MainWindow::on_PushButton_17_clicked()
{
	ThreeDimensionalComputation* threeDimensionalComputation = new ThreeDimensionalComputation();
	threeDimensionalComputation->show();
}
