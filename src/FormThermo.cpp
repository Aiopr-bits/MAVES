/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#include "FormThermo.h"
#include "FormSolver.h"

FormThermo::FormThermo(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormThermoClass())
{
	ui->setupUi(this);

	QRegularExpression regExp("^-?\\d*\\.?\\d+([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regExp, this);
	ui->lineEdit->setValidator(validator);
	ui->lineEdit_2->setValidator(validator);
	ui->lineEdit_3->setValidator(validator);
	ui->lineEdit_4->setValidator(validator);
	ui->lineEdit_5->setValidator(validator);
	ui->lineEdit_6->setValidator(validator);
	ui->lineEdit_7->setValidator(validator);
	ui->lineEdit_8->setValidator(validator);
	ui->lineEdit_9->setValidator(validator);
	ui->lineEdit_10->setValidator(validator);

	ui->widget->hide();
	ui->widget_2->hide();
}

FormThermo::~FormThermo()
{
	delete ui;
}

bool FormThermo::importParameter()
{
	//获取案例路径
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QFileInfo fileInfo(casePath);
	QString caseDir = fileInfo.path();
	std::string solverName = GlobalData::getInstance().getCaseData()->solverName;

	if (solverName == "steadyCompressibleSolver") {
		QString thermophysicalPropertiesPath = caseDir + "/constant/thermophysicalProperties";
		QFile file(thermophysicalPropertiesPath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QMessageBox::warning(this, tr("错误"), tr("无法打开 thermophysicalProperties 文件"));
			return false;
		}

		QTextStream in(&file);
		QStringList fields = { "type", "mixture", "transport", "thermo", "equationOfState", "specie", "energy" };
		QMap<QString, QComboBox*> fieldToComboBox = {
			{ "type", ui->comboBox },
			{ "mixture", ui->comboBox_2 },
			{ "transport", ui->comboBox_3 },
			{ "thermo", ui->comboBox_4 },
			{ "equationOfState", ui->comboBox_5 },
			{ "specie", ui->comboBox_6 },
			{ "energy", ui->comboBox_7 }
		};

		QMap<QString, QLineEdit*> fieldToLineEdit = {
			{ "molWeight", ui->lineEdit },
			{ "Cp", ui->lineEdit_2 },
			{ "Hf", ui->lineEdit_3 },
			{ "mu", ui->lineEdit_4 },
			{ "Pr", ui->lineEdit_5 }
		};

		while (!in.atEnd()) {
			QString line = in.readLine();
			for (const QString& field : fields) {
				if (line.trimmed().startsWith(field)) {
					QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
					if (parts.size() >= 2) {
						QString value = parts[1].trimmed();
						if (value.endsWith(";")) {
							value.chop(1);
						}
						QComboBox* comboBox = fieldToComboBox[field];
						for (int i = 0; i < comboBox->count(); ++i) {
							if (comboBox->itemText(i) == value) {
								comboBox->setCurrentIndex(i);
								break;
							}
						}
					}
				}
			}

			for (const QString& field : fieldToLineEdit.keys()) {
				if (line.trimmed().startsWith(field)) {
					QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
					if (parts.size() >= 2) {
						QString value = parts[1].trimmed();
						if (value.endsWith(";")) {
							value.chop(1);
						}
						QLineEdit* lineEdit = fieldToLineEdit[field];
						lineEdit->setText(value);
					}
				}
			}
		}

		file.close();
		return true;
	}
	else if (solverName == "transientIncompressibleSolver") {
		QString transportPropertiesPath = caseDir + "/constant/transportProperties";
		QFile file(transportPropertiesPath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QMessageBox::warning(this, tr("错误"), tr("无法打开 transportProperties 文件"));
			return false;
		}

		QTextStream in(&file);
		QStringList fields = { "transportModel" };
		QMap<QString, QComboBox*> fieldToComboBox = {
			{ "transportModel", ui->comboBox_8 }
		};

		QMap<QString, QLineEdit*> fieldToLineEdit = {
			{ "nu", ui->lineEdit_6 },
			{ "beta", ui->lineEdit_7 },
			{ "TRef", ui->lineEdit_8 },
			{ "Pr", ui->lineEdit_9 },
			{ "Prt", ui->lineEdit_10 }
		};

		while (!in.atEnd()) {
			QString line = in.readLine();
			for (const QString& field : fields) {
				if (line.trimmed().startsWith(field)) {
					QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
					if (parts.size() >= 2) {
						QString value = parts[1].trimmed();
						if (value.endsWith(";")) {
							value.chop(1);
						}
						QComboBox* comboBox = fieldToComboBox[field];
						for (int i = 0; i < comboBox->count(); ++i) {
							if (comboBox->itemText(i) == value) {
								comboBox->setCurrentIndex(i);
								break;
							}
						}
					}
				}
			}

			for (const QString& field : fieldToLineEdit.keys()) {
				if (line.trimmed().startsWith(field)) {
					QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
					if (parts.size() >= 2) {
						QString value = parts[1].trimmed();
						if (value.endsWith(";")) {
							value.chop(1);
						}
						QLineEdit* lineEdit = fieldToLineEdit[field];
						lineEdit->setText(value);
					}
				}
			}
		}

		file.close();
		return true;
	}

	return false;
}

bool FormThermo::exportParameter()
{
	//获取案例路径
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QFileInfo fileInfo(casePath);
	QString caseDir = fileInfo.path();
	std::string solverName = GlobalData::getInstance().getCaseData()->solverName;

	if (solverName == "steadyCompressibleSolver") {
		QString thermophysicalPropertiesPath = caseDir + "/constant/thermophysicalProperties";
		QFile file(thermophysicalPropertiesPath);
		if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
			QMessageBox::warning(this, tr("错误"), tr("无法打开 thermophysicalProperties 文件"));
			return false;
		}

		QTextStream in(&file);
		QString content = in.readAll();
		file.close();

		QMap<QString, QString> fieldToValue = {
			{ "type", ui->comboBox->currentText() },
			{ "mixture", ui->comboBox_2->currentText() },
			{ "transport", ui->comboBox_3->currentText() },
			{ "thermo", ui->comboBox_4->currentText() },
			{ "equationOfState", ui->comboBox_5->currentText() },
			{ "specie", ui->comboBox_6->currentText() },
			{ "energy", ui->comboBox_7->currentText() },
			{ "molWeight", ui->lineEdit->text() },
			{ "Cp", ui->lineEdit_2->text() },
			{ "Hf", ui->lineEdit_3->text() },
			{ "mu", ui->lineEdit_4->text() },
			{ "Pr", ui->lineEdit_5->text() }
		};

		for (auto it = fieldToValue.begin(); it != fieldToValue.end(); ++it) {
			QRegExp rx(it.key() + "\\s+\\S+;");
			rx.setMinimal(true);
			content.replace(rx, it.key() + " " + it.value() + ";");
		}

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
			QMessageBox::warning(this, tr("错误"), tr("无法打开 thermophysicalProperties 文件"));
			return false;
		}
		QTextStream out(&file);
		out << content;
		file.close();
		return true;
	}
	else if (solverName == "transientIncompressibleSolver") {
		QString transportPropertiesPath = caseDir + "/constant/transportProperties";
		QFile file(transportPropertiesPath);
		if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
			QMessageBox::warning(this, tr("错误"), tr("无法打开 transportProperties 文件"));
			return false;
		}

		QTextStream in(&file);
		QString content = in.readAll();
		file.close();

		QMap<QString, QString> fieldToValue = {
			{ "transportModel", ui->comboBox_8->currentText() },
			{ "nu", ui->lineEdit_6->text() },
			{ "beta", ui->lineEdit_7->text() },
			{ "TRef", ui->lineEdit_8->text() },
			{ "Pr", ui->lineEdit_9->text() },
			{ "Prt", ui->lineEdit_10->text() }
		};

		for (auto it = fieldToValue.begin(); it != fieldToValue.end(); ++it) {
			QRegExp rx(it.key() + "\\s+\\S+;");
			rx.setMinimal(true);
			content.replace(rx, it.key() + " " + it.value() + ";");
		}

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
			QMessageBox::warning(this, tr("错误"), tr("无法打开 transportProperties 文件"));
			return false;
		}
		QTextStream out(&file);
		out << content;
		file.close();
		return true;
	}

	return false;
}

void FormThermo::solverChanged(const QString& newText)
{
	if (newText == "rhoSimpleFoam") {
		ui->widget->show();
		ui->widget_2->hide();
	}
	else if (newText == "buoyantBoussinesqPimpleFoam") {
		ui->widget->hide();
		ui->widget_2->show();
	}
	else if (newText == "chtMultiRegionFoam") {
		
	}
	else {
		ui->widget->show();
		ui->widget_2->hide();
	}
}