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
	, formThermoListWidgets()
{
	ui->setupUi(this);

	ui->widget->hide();
	connect(ui->listWidget_3, &QListWidget::itemClicked, this, &FormThermo::on_ListWidgetItem_Clicked);



	//QRegularExpression regExp("^-?\\d*\\.?\\d+([eE][-+]?\\d+)?$");
	//QRegularExpressionValidator* validator = new QRegularExpressionValidator(regExp, this);
	//ui->lineEdit->setValidator(validator);
	//ui->lineEdit_2->setValidator(validator);
	//ui->lineEdit_3->setValidator(validator);
	//ui->lineEdit_4->setValidator(validator);
	//ui->lineEdit_5->setValidator(validator);
	//ui->lineEdit_6->setValidator(validator);
	//ui->lineEdit_7->setValidator(validator);
	//ui->lineEdit_8->setValidator(validator);
	//ui->lineEdit_9->setValidator(validator);
	//ui->lineEdit_10->setValidator(validator);

	//ui->widget->hide();
	//ui->widget_2->hide();
}

FormThermo::~FormThermo()
{
	delete ui;
}

bool FormThermo::importParameter()
{
	////获取案例路径
	//QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	//QFileInfo fileInfo(casePath);
	//QString caseDir = fileInfo.path();
	//std::string solverName = GlobalData::getInstance().getCaseData()->solverName;

	//if (solverName == "steadyCompressibleSolver") {
	//	QString thermophysicalPropertiesPath = caseDir + "/constant/thermophysicalProperties";
	//	QFile file(thermophysicalPropertiesPath);
	//	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	//		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "无法打开 thermophysicalProperties 文件" });
	//		dialogInformationPrompt->exec();
	//		return false;
	//	}

	//	QTextStream in(&file);
	//	QStringList fields = { "type", "mixture", "transport", "thermo", "equationOfState", "specie", "energy" };
	//	QMap<QString, QComboBox*> fieldToComboBox = {
	//		{ "type", ui->comboBox },
	//		{ "mixture", ui->comboBox_2 },
	//		{ "transport", ui->comboBox_3 },
	//		{ "thermo", ui->comboBox_4 },
	//		{ "equationOfState", ui->comboBox_5 },
	//		{ "specie", ui->comboBox_6 },
	//		{ "energy", ui->comboBox_7 }
	//	};

	//	QMap<QString, QLineEdit*> fieldToLineEdit = {
	//		{ "molWeight", ui->lineEdit },
	//		{ "Cp", ui->lineEdit_2 },
	//		{ "Hf", ui->lineEdit_3 },
	//		{ "mu", ui->lineEdit_4 },
	//		{ "Pr", ui->lineEdit_5 }
	//	};

	//	while (!in.atEnd()) {
	//		QString line = in.readLine();
	//		for (const QString& field : fields) {
	//			if (line.trimmed().startsWith(field)) {
	//				QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	//				if (parts.size() >= 2) {
	//					QString value = parts[1].trimmed();
	//					if (value.endsWith(";")) {
	//						value.chop(1);
	//					}
	//					QComboBox* comboBox = fieldToComboBox[field];
	//					for (int i = 0; i < comboBox->count(); ++i) {
	//						if (comboBox->itemText(i) == value) {
	//							comboBox->setCurrentIndex(i);
	//							break;
	//						}
	//					}
	//				}
	//			}
	//		}

	//		for (const QString& field : fieldToLineEdit.keys()) {
	//			if (line.trimmed().startsWith(field)) {
	//				QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	//				if (parts.size() >= 2) {
	//					QString value = parts[1].trimmed();
	//					if (value.endsWith(";")) {
	//						value.chop(1);
	//					}
	//					QLineEdit* lineEdit = fieldToLineEdit[field];
	//					lineEdit->setText(value);
	//				}
	//			}
	//		}
	//	}

	//	file.close();
	//	return true;
	//}
	//else if (solverName == "transientIncompressibleSolver") {
	//	QString transportPropertiesPath = caseDir + "/constant/transportProperties";
	//	QFile file(transportPropertiesPath);
	//	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	//		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "无法打开 transportProperties 文件" });
	//		dialogInformationPrompt->exec();
	//		return false;
	//	}

	//	QTextStream in(&file);
	//	QStringList fields = { "transportModel" };
	//	QMap<QString, QComboBox*> fieldToComboBox = {
	//		{ "transportModel", ui->comboBox_8 }
	//	};

	//	QMap<QString, QLineEdit*> fieldToLineEdit = {
	//		{ "nu", ui->lineEdit_6 },
	//		{ "beta", ui->lineEdit_7 },
	//		{ "TRef", ui->lineEdit_8 },
	//		{ "Pr", ui->lineEdit_9 },
	//		{ "Prt", ui->lineEdit_10 }
	//	};

	//	while (!in.atEnd()) {
	//		QString line = in.readLine();
	//		for (const QString& field : fields) {
	//			if (line.trimmed().startsWith(field)) {
	//				QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	//				if (parts.size() >= 2) {
	//					QString value = parts[1].trimmed();
	//					if (value.endsWith(";")) {
	//						value.chop(1);
	//					}
	//					QComboBox* comboBox = fieldToComboBox[field];
	//					for (int i = 0; i < comboBox->count(); ++i) {
	//						if (comboBox->itemText(i) == value) {
	//							comboBox->setCurrentIndex(i);
	//							break;
	//						}
	//					}
	//				}
	//			}
	//		}

	//		for (const QString& field : fieldToLineEdit.keys()) {
	//			if (line.trimmed().startsWith(field)) {
	//				QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	//				if (parts.size() >= 2) {
	//					QString value = parts[1].trimmed();
	//					if (value.endsWith(";")) {
	//						value.chop(1);
	//					}
	//					QLineEdit* lineEdit = fieldToLineEdit[field];
	//					lineEdit->setText(value);
	//				}
	//			}
	//		}
	//	}

	//	file.close();
	//	return true;
	//}

	return false;
}

bool FormThermo::exportParameter()
{
	////获取案例路径
	//QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	//QFileInfo fileInfo(casePath);
	//QString caseDir = fileInfo.path();
	//std::string solverName = GlobalData::getInstance().getCaseData()->solverName;

	//if (solverName == "steadyCompressibleSolver") {
	//	QString thermophysicalPropertiesPath = caseDir + "/constant/thermophysicalProperties";
	//	QFile file(thermophysicalPropertiesPath);
	//	if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
	//		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "无法打开 thermophysicalProperties 文件" });
	//		dialogInformationPrompt->exec();
	//		return false;
	//	}

	//	QTextStream in(&file);
	//	QString content = in.readAll();
	//	file.close();

	//	QMap<QString, QString> fieldToValue = {
	//		{ "type", ui->comboBox->currentText() },
	//		{ "mixture", ui->comboBox_2->currentText() },
	//		{ "transport", ui->comboBox_3->currentText() },
	//		{ "thermo", ui->comboBox_4->currentText() },
	//		{ "equationOfState", ui->comboBox_5->currentText() },
	//		{ "specie", ui->comboBox_6->currentText() },
	//		{ "energy", ui->comboBox_7->currentText() },
	//		{ "molWeight", ui->lineEdit->text() },
	//		{ "Cp", ui->lineEdit_2->text() },
	//		{ "Hf", ui->lineEdit_3->text() },
	//		{ "mu", ui->lineEdit_4->text() },
	//		{ "Pr", ui->lineEdit_5->text() }
	//	};

	//	for (auto it = fieldToValue.begin(); it != fieldToValue.end(); ++it) {
	//		QRegExp rx(it.key() + "\\s+\\S+;");
	//		rx.setMinimal(true);
	//		content.replace(rx, it.key() + " " + it.value() + ";");
	//	}

	//	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
	//		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "无法打开 thermophysicalProperties 文件" });
	//		dialogInformationPrompt->exec();
	//		return false;
	//	}
	//	QTextStream out(&file);
	//	out << content;
	//	file.close();
	//	return true;
	//}
	//else if (solverName == "transientIncompressibleSolver") {
	//	QString transportPropertiesPath = caseDir + "/constant/transportProperties";
	//	QFile file(transportPropertiesPath);
	//	if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
	//		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "无法打开 transportProperties 文件" });
	//		dialogInformationPrompt->exec();
	//		return false;
	//	}

	//	QTextStream in(&file);
	//	QString content = in.readAll();
	//	file.close();

	//	QMap<QString, QString> fieldToValue = {
	//		{ "transportModel", ui->comboBox_8->currentText() },
	//		{ "nu", ui->lineEdit_6->text() },
	//		{ "beta", ui->lineEdit_7->text() },
	//		{ "TRef", ui->lineEdit_8->text() },
	//		{ "Pr", ui->lineEdit_9->text() },
	//		{ "Prt", ui->lineEdit_10->text() }
	//	};

	//	for (auto it = fieldToValue.begin(); it != fieldToValue.end(); ++it) {
	//		QRegExp rx(it.key() + "\\s+\\S+;");
	//		rx.setMinimal(true);
	//		content.replace(rx, it.key() + " " + it.value() + ";");
	//	}

	//	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
	//		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "无法打开 transportProperties 文件" });
	//		dialogInformationPrompt->exec();
	//		return false;
	//	}
	//	QTextStream out(&file);
	//	out << content;
	//	file.close();
	//	return true;
	//}

	return false;
}

void FormThermo::on_ListWidgetItem_Clicked(QListWidgetItem* item)
{
	// 获取当前点击项的索引
	int index = ui->listWidget_3->row(item);

	// 先隐藏所有控件
	for (auto widget : formThermoListWidgets) {
		widget->hide();
	}

	// 再显示被点击索引对应的控件
	if (index >= 0 && index < formThermoListWidgets.size()) {
		formThermoListWidgets[index]->show();
	}
}

void FormThermo::initialization(const QString& solver)
{
	// 清理之前的控件
	for (FormThermoListWidget* formThermoListWidget : formThermoListWidgets) {
		delete formThermoListWidget;
	}
	formThermoListWidgets.clear();
	ui->listWidget_3->clear();

	if (solver == "rhoSimpleFoam") {
		ui->widget->hide();

		FormThermoListWidget* formThermoListWidget = new FormThermoListWidget(this, "fluid");
		int insertPosition = std::max(0, ui->verticalLayout_2->count() - 1); 
		ui->verticalLayout_2->insertWidget(insertPosition, formThermoListWidget);
		formThermoListWidgets.push_back(formThermoListWidget);
	}
	else if (solver == "buoyantBoussinesqPimpleFoam") {
		ui->widget->hide();

		FormThermoListWidget* formThermoListWidget = new FormThermoListWidget(this, "fluid");
		int insertPosition = std::max(0, ui->verticalLayout_2->count() - 1); 
		ui->verticalLayout_2->insertWidget(insertPosition, formThermoListWidget);
		formThermoListWidgets.push_back(formThermoListWidget);
	}
	else if (solver == "chtMultiRegionFoam") {
		ui->widget->show();

		ui->label_4->show();
		ui->listWidget_3->show();

		// 获取子域名称添加到 ui->widget 中
		std::unordered_map<std::string, std::string> regionsType = GlobalData::getInstance().getCaseData()->regionsType;
		for (const auto& region : regionsType) {
			QString regionName = QString::fromStdString(region.first);
			QIcon icon;
			FormThermoListWidget* formThermoListWidget = nullptr;

			if (region.second == "solid") {
				icon.addFile("../res/Solid.png");
				formThermoListWidget = new FormThermoListWidget(this, "solid");
			}
			else if (region.second == "fluid") {
				icon.addFile("../res/Fluid.png");
				formThermoListWidget = new FormThermoListWidget(this, "fluid");
			}

			if (formThermoListWidget) {
				int insertPosition = std::max(0, ui->verticalLayout_2->count() - 1);
				ui->verticalLayout_2->insertWidget(insertPosition, formThermoListWidget);
				formThermoListWidget->hide();  // 默认隐藏
				formThermoListWidgets.push_back(formThermoListWidget);

				QListWidgetItem* item = new QListWidgetItem(icon, regionName);
				ui->listWidget_3->addItem(item);
			}
		}

		int totalHeight = 0;
		for (int i = 0; i < std::min(ui->listWidget_3->count(), 5); ++i) {
			totalHeight += ui->listWidget_3->sizeHintForRow(i);
		}
		ui->listWidget_3->setFixedHeight(totalHeight);
		int labelHeight = ui->label_4->sizeHint().height();
		ui->widget->setFixedHeight(totalHeight + labelHeight);
	}
}
