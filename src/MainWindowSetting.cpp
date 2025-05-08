#include "MainWindowSetting.h"

MainWindowSetting::MainWindowSetting(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::MainWindowSettingClass())
{
	ui->setupUi(this);

	this->setFixedSize(this->width(), this->height());
	this->setWindowFlags(this->windowFlags() & ~Qt::WindowMaximizeButtonHint);
	readConfig();
}

MainWindowSetting::~MainWindowSetting()
{
	delete ui;
}

void MainWindowSetting::readConfig()
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
				ui->lineEdit->setText(result);
			}
		}
		else if (line.startsWith("ZhiShiTuPu"))
		{
			int colonIndex = line.indexOf(':');
			if (colonIndex != -1)
			{
				QString result = line.mid(colonIndex + 1).trimmed();
				ui->lineEdit_2->setText(result);
			}
		}
		else if (line.startsWith("TMAX3D"))
		{
			int colonIndex = line.indexOf(':');
			if (colonIndex != -1)
			{
				QString result = line.mid(colonIndex + 1).trimmed();
				ui->lineEdit_3->setText(result);
			}
		}
	}
}

void MainWindowSetting::writeConfig()
{
	QString zongHeGuanLiRuanJian = ui->lineEdit->text();
	QString zhiShiTuPu = ui->lineEdit_2->text();
	QString tmax3d = ui->lineEdit_3->text();

	QString filePath = "../config/MainWindow";

	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
	{
		QMessageBox::critical(this, "错误", "无法打开文件: " + filePath);
		return;
	}

	QTextStream out(&file);
	out << "ZongHeGuanLiRuanJian:" << zongHeGuanLiRuanJian << "\n";
	out << "ZhiShiTuPu:" << zhiShiTuPu << "\n";
	out << "TMAX3D:" << tmax3d << "\n";

	file.close();
}

void MainWindowSetting::closeEvent(QCloseEvent* event)
{
	writeConfig();
	QWidget::closeEvent(event);
}
