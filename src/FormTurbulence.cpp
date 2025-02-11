#include "FormTurbulence.h"
#include <QRegularExpression>

FormTurbulence::FormTurbulence(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormTurbulenceClass())
{
	ui->setupUi(this);
	ui->gridLayout->setRowStretch(0, 1);
	ui->gridLayout->setRowStretch(3, 1);
	ui->gridLayout->setColumnStretch(0, 1);
	ui->gridLayout->setColumnStretch(2, 1);

	connect(ui->comboBox, &QComboBox::currentTextChanged, this, &FormTurbulence::onComboBoxTextChanged);

	onComboBoxTextChanged("RAS");
}

FormTurbulence::~FormTurbulence()
{
	delete ui;
}

bool FormTurbulence::importParameter()
{
	//获取案例路径
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QFileInfo fileInfo(casePath);
	QString caseDir = fileInfo.path();
	QString turbulencePropertiesPath = caseDir + "/constant/turbulenceProperties";

	// 打开 turbulencePropertiesPath 文件
	QFile turbulencePropertiesFile(turbulencePropertiesPath);
	if (!turbulencePropertiesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("错误"), tr("无法打开 turbulenceProperties 文件"));
		return false;
	}

	// 读取 turbulenceProperties 文件中的 simulationType 字段
	QString simulationType;
	QTextStream in(&turbulencePropertiesFile);
	while (!in.atEnd()) {
		QString line = in.readLine();
		if (line.trimmed().startsWith("simulationType")) {
			QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			if (parts.size() >= 2) {
				simulationType = parts[1].trimmed();
				if (simulationType.endsWith(";")) {
					simulationType.chop(1);
				}
				break;
			}
		}
	}


	if (simulationType.isEmpty()) {
		QMessageBox::warning(this, tr("错误"), tr("未找到 simulationType 字段"));
		return false;
	}

	if (simulationType == "RAS") {
		//切换到 RAS 模型
		for (int i = 0; i < ui->comboBox->count(); ++i) {
			if (ui->comboBox->itemText(i) == "RAS") {
				ui->comboBox->setCurrentIndex(i);
				break;
			}
		}

		// 读取 turbulenceProperties 文件中的RASModel字段
		QString RASModel;
		while (!in.atEnd()) {
			QString line = in.readLine();
			if (line.trimmed().startsWith("RASModel")) {
				QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
				if (parts.size() >= 2) {
					RASModel = parts[1].trimmed();
					if (RASModel.endsWith(";")) {
						RASModel.chop(1);
					}
					break;
				}
			}
		}
		for (int i = 0; i < ui->comboBox_4->count(); ++i) {
			if (ui->comboBox_3->itemText(i) == RASModel) {
				ui->comboBox_3->setCurrentIndex(i);
				break;
			}
		}

		//读取turbulenceProperties 文件中的turbulence字段
		QString turbulence;
		while (!in.atEnd()) {
			QString line = in.readLine();
			if (line.trimmed().startsWith("turbulence")) {
				QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
				if (parts.size() >= 2) {
					turbulence = parts[1].trimmed();
					if (turbulence.endsWith(";")) {
						turbulence.chop(1);
					}
					break;
				}
			}
		}
		ui->checkBox_3->setChecked(turbulence == "on" ? true : false);

		//读取turbulenceProperties 文件中的printCoeffs字段
		QString printCoeffs;
		while (!in.atEnd()) {
			QString line = in.readLine();
			if (line.trimmed().startsWith("printCoeffs")) {
				QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
				if (parts.size() >= 2) {
					printCoeffs = parts[1].trimmed();
					if (printCoeffs.endsWith(";")) {
						printCoeffs.chop(1);
					}
					break;
				}
			}
		}
		ui->checkBox_4->setChecked(printCoeffs == "on" ? true : false);		
		return true;
	}
	else if(simulationType == "LES"){
		for (int i = 0; i < ui->comboBox->count(); ++i) {
			if (ui->comboBox->itemText(i) == "LES") {
				ui->comboBox->setCurrentIndex(i);
				break;
			}
		}

		// 读取 turbulenceProperties 文件中的LESModel字段
		QString LESModel;
		while (!in.atEnd()) {
			QString line = in.readLine();
			if (line.trimmed().startsWith("LESModel")) {
				QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
				if (parts.size() >= 2) {
					LESModel = parts[1].trimmed();
					if (LESModel.endsWith(";")) {
						LESModel.chop(1);
					}
					break;
				}
			}
		}
		for (int i = 0; i < ui->comboBox_4->count(); ++i) {
			if (ui->comboBox_4->itemText(i) == LESModel) {
				ui->comboBox_4->setCurrentIndex(i);
				break;
			}
		}

		//读取turbulenceProperties 文件中的turbulence字段
		QString turbulence;
		while (!in.atEnd()) {
			QString line = in.readLine();
			if (line.trimmed().startsWith("turbulence")) {
				QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
				if (parts.size() >= 2) {
					turbulence = parts[1].trimmed();
					if (turbulence.endsWith(";")) {
						turbulence.chop(1);
					}
					break;
				}
			}
		}
		ui->checkBox_5->setChecked(turbulence == "on" ? true : false);

		//读取turbulenceProperties 文件中的printCoeffs字段
		QString printCoeffs;
		while (!in.atEnd()) {
			QString line = in.readLine();
			if (line.trimmed().startsWith("printCoeffs")) {
				QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
				if (parts.size() >= 2) {
					printCoeffs = parts[1].trimmed();
					if (printCoeffs.endsWith(";")) {
						printCoeffs.chop(1);
					}
					break;
				}
			}
		}
		ui->checkBox_6->setChecked(printCoeffs == "on" ? true : false);
		return true;
	}
	else if (simulationType == "laminar") {
		for (int i = 0; i < ui->comboBox->count(); ++i) {
			if (ui->comboBox->itemText(i) == "laminar") {
				ui->comboBox->setCurrentIndex(i);
				break;
			}
		}
		return true;
	}

	turbulencePropertiesFile.close();
	return false;
}

bool FormTurbulence::exportParameter()
{
	// 获取案例路径
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QFileInfo fileInfo(casePath);
	QString caseDir = fileInfo.path();
	QString turbulencePropertiesPath = caseDir + "/constant/turbulenceProperties";

	// 打开 turbulencePropertiesPath 文件
	QFile turbulencePropertiesFile(turbulencePropertiesPath);
	if (!turbulencePropertiesFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
		QMessageBox::warning(this, tr("错误"), tr("无法打开 turbulenceProperties 文件"));
		return false;
	}

	// 读取文件内容
	QTextStream in(&turbulencePropertiesFile);
	QString content = in.readAll();
	turbulencePropertiesFile.close();

	// 替换 simulationType 字段
	QString simulationType = ui->comboBox->currentText();
	content.replace(QRegExp("simulationType\\s+\\w+;"), "simulationType " + simulationType + ";");

	if (simulationType == "RAS") {
		// 替换 RASModel 字段
		QString RASModel = ui->comboBox_3->currentText();
		content.replace(QRegExp("RASModel\\s+\\w+;"), "RASModel " + RASModel + ";");

		// 替换 turbulence 字段
		QString turbulence = ui->checkBox_3->isChecked() ? "on" : "off";
		content.replace(QRegExp("turbulence\\s+\\w+;"), "turbulence " + turbulence + ";");

		// 替换 printCoeffs 字段
		QString printCoeffs = ui->checkBox_4->isChecked() ? "on" : "off";
		content.replace(QRegExp("printCoeffs\\s+\\w+;"), "printCoeffs " + printCoeffs + ";");
	}
	else if (simulationType == "LES") {
		// 替换 LESModel 字段
		QString LESModel = ui->comboBox_4->currentText();
		content.replace(QRegExp("LESModel\\s+\\w+;"), "LESModel " + LESModel + ";");

		// 替换 turbulence 字段
		QString turbulence = ui->checkBox_5->isChecked() ? "on" : "off";
		content.replace(QRegExp("turbulence\\s+\\w+;"), "turbulence " + turbulence + ";");

		// 替换 printCoeffs 字段
		QString printCoeffs = ui->checkBox_6->isChecked() ? "on" : "off";
		content.replace(QRegExp("printCoeffs\\s+\\w+;"), "printCoeffs " + printCoeffs + ";");
	}

	// 将修改后的内容写回文件
	if (!turbulencePropertiesFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		QMessageBox::warning(this, tr("错误"), tr("无法打开 turbulenceProperties 文件"));
		return false;
	}
	QTextStream out(&turbulencePropertiesFile);
	out << content;
	turbulencePropertiesFile.close();

	return true;
}

void FormTurbulence::onComboBoxTextChanged(const QString& text)
{
	if (text == "RAS") {
		ui->tabWidget->show();
		for (int i = 0; i < ui->tabWidget->count(); ++i) {
			if (ui->tabWidget->tabText(i) == "RAS参数") {
				ui->tabWidget->setTabVisible(i, true);
				ui->tabWidget->setCurrentIndex(i);
			}
			else {
				ui->tabWidget->setTabVisible(i, false);
			}
		}
	}
	else if (text == "LES") {
		ui->tabWidget->show();
		for (int i = 0; i < ui->tabWidget->count(); ++i) {
			if (ui->tabWidget->tabText(i) == "LES参数") {
				ui->tabWidget->setTabVisible(i, true);
				ui->tabWidget->setCurrentIndex(i);
			}
			else {
				ui->tabWidget->setTabVisible(i, false);
			}
		}
	}
	else if (text == "laminar") {
		ui->tabWidget->hide();
	}
}