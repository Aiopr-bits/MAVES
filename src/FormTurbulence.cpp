/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#include "FormTurbulence.h"
#include <QRegularExpression>

FormTurbulence::FormTurbulence(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormTurbulenceClass())
{
	ui->setupUi(this);

	// 连接信号和槽
	connect(ui->comboBox_5, QOverload<int>::of(&QComboBox::activated), this, &FormTurbulence::onComboBox_5_IndexChanged);
	connect(ui->comboBox_6, QOverload<int>::of(&QComboBox::activated), this, &FormTurbulence::onComboBox_6_IndexChanged);
	connect(ui->comboBox_7, QOverload<int>::of(&QComboBox::activated), this, &FormTurbulence::onComboBox_7_IndexChanged);
	connect(ui->comboBox_8, QOverload<int>::of(&QComboBox::activated), this, &FormTurbulence::onComboBox_8_IndexChanged);
	connect(ui->pushButton_3, &QPushButton::toggled, this, &FormTurbulence::on_PushButton_3_Toggled);
	connect(ui->pushButton_6, &QPushButton::toggled, this, &FormTurbulence::on_PushButton_6_Toggled);
	connect(ui->pushButton_7, &QPushButton::toggled, this, &FormTurbulence::on_PushButton_7_Toggled);
	connect(ui->pushButton_8, &QPushButton::toggled, this, &FormTurbulence::on_PushButton_8_Toggled);
	connect(ui->radioButton_7, &QRadioButton::toggled, this, &FormTurbulence::onRadioButtonToggled);
	connect(ui->radioButton_8, &QRadioButton::toggled, this, &FormTurbulence::onRadioButtonToggled);
	connect(ui->radioButton_9, &QRadioButton::toggled, this, &FormTurbulence::onRadioButtonToggled);

	//选中第一个单选按钮
	onComboBox_5_IndexChanged(0);
	onComboBox_6_IndexChanged(0);
	onComboBox_7_IndexChanged(0);
	onComboBox_8_IndexChanged(0);
	ui->radioButton_7->setChecked(true);

	//ui->gridLayout->setRowStretch(0, 1);
	//ui->gridLayout->setRowStretch(3, 1);
	//ui->gridLayout->setColumnStretch(0, 1);
	//ui->gridLayout->setColumnStretch(2, 1);

	//connect(ui->comboBox, &QComboBox::currentTextChanged, this, &FormTurbulence::onComboBoxTextChanged);

	//onComboBoxTextChanged("RAS");
}

FormTurbulence::~FormTurbulence()
{
	delete ui;
}

void FormTurbulence::onComboBox_5_IndexChanged(int index)
{
	QString text = ui->comboBox_5->currentText();

	if (text == "k-ε")
	{
		ui->widget_17->hide();
		ui->widget_18->hide();
		ui->widget_19->hide();
		ui->widget_20->hide();
		ui->widget_22->hide();
		ui->widget_32->hide();
		ui->widget_33->hide();
		ui->widget_34->hide();
		ui->widget_17->show();

		ui->widget_46->hide();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "RNG k-ε")
	{
		ui->widget_17->hide();
		ui->widget_18->hide();
		ui->widget_19->hide();
		ui->widget_20->hide();
		ui->widget_22->hide();
		ui->widget_32->hide();
		ui->widget_33->hide();
		ui->widget_34->hide();
		ui->widget_18->show();

		ui->widget_46->hide();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "Realizable k-ε")
	{
		ui->widget_17->hide();
		ui->widget_18->hide();
		ui->widget_19->hide();
		ui->widget_20->hide();
		ui->widget_22->hide();
		ui->widget_32->hide();
		ui->widget_33->hide();
		ui->widget_34->hide();
		ui->widget_19->show();

		ui->widget_46->hide();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);

	}
	else if (text == "Spalart-Allmaras")
	{
		ui->widget_17->hide();
		ui->widget_18->hide();
		ui->widget_19->hide();
		ui->widget_20->hide();
		ui->widget_22->hide();
		ui->widget_32->hide();
		ui->widget_33->hide();
		ui->widget_34->hide();
		ui->widget_20->show();

		ui->widget_46->hide();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "k-ω")
	{
		ui->widget_17->hide();
		ui->widget_18->hide();
		ui->widget_19->hide();
		ui->widget_20->hide();
		ui->widget_22->hide();
		ui->widget_32->hide();
		ui->widget_33->hide();
		ui->widget_34->hide();
		ui->widget_22->show();	 

		ui->widget_46->hide();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "k-ω SST")
	{
		ui->widget_17->hide();
		ui->widget_18->hide();
		ui->widget_19->hide();
		ui->widget_20->hide();
		ui->widget_22->hide();
		ui->widget_32->hide();
		ui->widget_33->hide();
		ui->widget_34->hide();
		ui->widget_32->show();

		ui->widget_46->hide();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "k-ω SST SAS")
	{
		ui->widget_17->hide();
		ui->widget_18->hide();
		ui->widget_19->hide();
		ui->widget_20->hide();
		ui->widget_22->hide();
		ui->widget_32->hide();
		ui->widget_33->hide();
		ui->widget_34->hide();
		ui->widget_33->show();

		ui->widget_46->show();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "Buoyant k-ε")
	{
		ui->widget_17->hide();
		ui->widget_18->hide();
		ui->widget_19->hide();
		ui->widget_20->hide();
		ui->widget_22->hide();
		ui->widget_32->hide();
		ui->widget_33->hide();
		ui->widget_34->hide();
		ui->widget_34->show();

		ui->widget_46->hide();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
}

void FormTurbulence::onComboBox_6_IndexChanged(int index)
{
	QString text = ui->comboBox_6->currentText();

	if (text == "k-ω SST DDES")
	{
		ui->widget_36->hide();
		ui->widget_37->hide();
		ui->widget_38->hide();
		ui->widget_39->hide();
		ui->widget_40->hide();
		ui->widget_41->hide();
		ui->widget_42->hide();
		ui->widget_43->hide();
		ui->widget_44->hide();
		ui->widget_45->hide();
		ui->widget_36->show();

		ui->widget_46->show();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "k-ω SST IDDES")
	{
		ui->widget_36->hide();
		ui->widget_37->hide();
		ui->widget_38->hide();
		ui->widget_39->hide();
		ui->widget_40->hide();
		ui->widget_41->hide();
		ui->widget_42->hide();
		ui->widget_43->hide();
		ui->widget_44->hide();
		ui->widget_45->hide();
		ui->widget_37->show();

		ui->widget_46->show();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "k-ω SST DES")
	{
		ui->widget_36->hide();
		ui->widget_37->hide();
		ui->widget_38->hide();
		ui->widget_39->hide();
		ui->widget_40->hide();
		ui->widget_41->hide();
		ui->widget_42->hide();
		ui->widget_43->hide();
		ui->widget_44->hide();
		ui->widget_45->hide();
		ui->widget_38->show();

		ui->widget_46->show();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "Spalart-Allmaras DES")
	{
		ui->widget_36->hide();
		ui->widget_37->hide();
		ui->widget_38->hide();
		ui->widget_39->hide();
		ui->widget_40->hide();
		ui->widget_41->hide();
		ui->widget_42->hide();
		ui->widget_43->hide();
		ui->widget_44->hide();
		ui->widget_45->hide();
		ui->widget_39->show();

		ui->widget_46->show();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "Spalart-Allmaras DDES")
	{
		ui->widget_36->hide();
		ui->widget_37->hide();
		ui->widget_38->hide();
		ui->widget_39->hide();
		ui->widget_40->hide();
		ui->widget_41->hide();
		ui->widget_42->hide();
		ui->widget_43->hide();
		ui->widget_44->hide();
		ui->widget_45->hide();
		ui->widget_40->show();

		ui->widget_46->show();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "Spalart-Allmaras IDDES")
	{
		ui->widget_36->hide();
		ui->widget_37->hide();
		ui->widget_38->hide();
		ui->widget_39->hide();
		ui->widget_40->hide();
		ui->widget_41->hide();
		ui->widget_42->hide();
		ui->widget_43->hide();
		ui->widget_44->hide();
		ui->widget_45->hide();
		ui->widget_41->show();

		ui->widget_46->show();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "Smagorinsky")
	{
		ui->widget_36->hide();
		ui->widget_37->hide();
		ui->widget_38->hide();
		ui->widget_39->hide();
		ui->widget_40->hide();
		ui->widget_41->hide();
		ui->widget_42->hide();
		ui->widget_43->hide();
		ui->widget_44->hide();
		ui->widget_45->hide();
		ui->widget_42->show();

		ui->widget_46->show();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "WALE")
	{
		ui->widget_36->hide();
		ui->widget_37->hide();
		ui->widget_38->hide();
		ui->widget_39->hide();
		ui->widget_40->hide();
		ui->widget_41->hide();
		ui->widget_42->hide();
		ui->widget_43->hide();
		ui->widget_44->hide();
		ui->widget_45->hide();
		ui->widget_43->show();

		ui->widget_46->show();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "k Equation")
	{
		ui->widget_36->hide();
		ui->widget_37->hide();
		ui->widget_38->hide();
		ui->widget_39->hide();
		ui->widget_40->hide();
		ui->widget_41->hide();
		ui->widget_42->hide();
		ui->widget_43->hide();
		ui->widget_44->hide();
		ui->widget_45->hide();
		ui->widget_44->show();

		ui->widget_46->show();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->hide();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
	else if (text == "Dynamic k Equation")
	{
		ui->widget_36->hide();
		ui->widget_37->hide();
		ui->widget_38->hide();
		ui->widget_39->hide();
		ui->widget_40->hide();
		ui->widget_41->hide();
		ui->widget_42->hide();
		ui->widget_43->hide();
		ui->widget_44->hide();
		ui->widget_45->hide();
		ui->widget_45->show();

		ui->widget_46->show();
		ui->pushButton_7->setChecked(false);
		on_PushButton_7_Toggled(false);

		ui->widget_48->show();
		ui->pushButton_8->setChecked(false);
		on_PushButton_8_Toggled(false);
	}
}

void FormTurbulence::onComboBox_7_IndexChanged(int index)
{
	QString text = ui->comboBox_7->currentText();
	
	if (text == "Cube Root Volume")
	{
		ui->widget_47->show();
	}
	else if (text == "Max Delta XYZ")
	{
		ui->widget_47->show();
	}
}

void FormTurbulence::onComboBox_8_IndexChanged(int index)
{
	QString text = ui->comboBox_8->currentText();

	if (text == "Simple")
	{
		ui->widget_49->hide();
		ui->pushButton_8->setEnabled(false);
	}
	else if (text == "Laplace")
	{
		ui->widget_49->show();
		ui->pushButton_8->setEnabled(true);
	}
	else if (text == "Anisotropic")
	{
		ui->widget_49->show();
		ui->pushButton_8->setEnabled(true);
	}
	
}

void FormTurbulence::on_PushButton_3_Toggled(bool checked)
{
	if (checked) 
	{
		ui->widget_50->show();
	}
	else 
	{
		ui->widget_50->hide();
	}
}

void FormTurbulence::on_PushButton_6_Toggled(bool checked)
{
	if (checked) 
	{
		ui->widget_51->show();
	}
	else
	{
		ui->widget_51->hide();
	}
}

void FormTurbulence::on_PushButton_7_Toggled(bool checked)
{
	if (checked) 
	{
		ui->widget_47->show();
	}
	else
	{
		ui->widget_47->hide();
	}
}

void FormTurbulence::on_PushButton_8_Toggled(bool checked)
{
	if (checked) 
	{
		ui->widget_49->show();
	}
	else
	{
		ui->widget_49->hide();
	}
}

void FormTurbulence::onRadioButtonToggled()
{
	QRadioButton* senderButton = qobject_cast<QRadioButton*>(sender());
	if (senderButton && senderButton->isChecked()) {
		if (senderButton == ui->radioButton_7) {
			ui->widget->hide();
			ui->widget_46->hide();
			ui->widget_48->hide();

			ui->pushButton_3->setChecked(false);
			on_PushButton_3_Toggled(false);
			ui->pushButton_7->setChecked(false);
			on_PushButton_7_Toggled(false);
			ui->pushButton_8->setChecked(false);
			on_PushButton_8_Toggled(false);
		}
		else if (senderButton == ui->radioButton_8) {
			ui->widget->show();
			ui->widget_16->show();
			ui->widget_35->hide();
			ui->widget_46->hide();
			ui->widget_48->hide();

			ui->pushButton_3->setChecked(false);
			on_PushButton_3_Toggled(false);
			ui->pushButton_7->setChecked(false);
			on_PushButton_7_Toggled(false);
			ui->pushButton_8->setChecked(false);
			on_PushButton_8_Toggled(false);
		}
		else if (senderButton == ui->radioButton_9) {
			ui->widget->show();
			ui->widget_16->hide();
			ui->widget_35->show();
			ui->widget_46->hide();
			ui->widget_48->hide();

			ui->pushButton_6->setChecked(false);
			on_PushButton_6_Toggled(false);
			ui->pushButton_7->setChecked(false);
			on_PushButton_7_Toggled(false);
			ui->pushButton_8->setChecked(false);
			on_PushButton_8_Toggled(false);
		}
	}
}

bool FormTurbulence::importParameter()
{
	////获取案例路径
	//QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	//QFileInfo fileInfo(casePath);
	//QString caseDir = fileInfo.path();
	//QString turbulencePropertiesPath = caseDir + "/constant/turbulenceProperties";

	//// 打开 turbulencePropertiesPath 文件
	//QFile turbulencePropertiesFile(turbulencePropertiesPath);
	//if (!turbulencePropertiesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
	//	DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "无法打开 turbulenceProperties 文件" });
	//	dialogInformationPrompt->exec();
	//	return false;
	//}

	//// 读取 turbulenceProperties 文件中的 simulationType 字段
	//QString simulationType;
	//QTextStream in(&turbulencePropertiesFile);
	//while (!in.atEnd()) {
	//	QString line = in.readLine();
	//	if (line.trimmed().startsWith("simulationType")) {
	//		QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	//		if (parts.size() >= 2) {
	//			simulationType = parts[1].trimmed();
	//			if (simulationType.endsWith(";")) {
	//				simulationType.chop(1);
	//			}
	//			break;
	//		}
	//	}
	//}


	//if (simulationType.isEmpty()) {
	//	DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "未找到 simulationType 字段 "});
	//	dialogInformationPrompt->exec();
	//	return false;
	//}

	//if (simulationType == "RAS") {
	//	//切换到 RAS 模型
	//	for (int i = 0; i < ui->comboBox->count(); ++i) {
	//		if (ui->comboBox->itemText(i) == "RAS") {
	//			ui->comboBox->setCurrentIndex(i);
	//			break;
	//		}
	//	}

	//	// 读取 turbulenceProperties 文件中的RASModel字段
	//	QString RASModel;
	//	while (!in.atEnd()) {
	//		QString line = in.readLine();
	//		if (line.trimmed().startsWith("RASModel")) {
	//			QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	//			if (parts.size() >= 2) {
	//				RASModel = parts[1].trimmed();
	//				if (RASModel.endsWith(";")) {
	//					RASModel.chop(1);
	//				}
	//				break;
	//			}
	//		}
	//	}
	//	for (int i = 0; i < ui->comboBox_4->count(); ++i) {
	//		if (ui->comboBox_3->itemText(i) == RASModel) {
	//			ui->comboBox_3->setCurrentIndex(i);
	//			break;
	//		}
	//	}

	//	//读取turbulenceProperties 文件中的turbulence字段
	//	QString turbulence;
	//	while (!in.atEnd()) {
	//		QString line = in.readLine();
	//		if (line.trimmed().startsWith("turbulence")) {
	//			QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	//			if (parts.size() >= 2) {
	//				turbulence = parts[1].trimmed();
	//				if (turbulence.endsWith(";")) {
	//					turbulence.chop(1);
	//				}
	//				break;
	//			}
	//		}
	//	}
	//	ui->checkBox_3->setChecked(turbulence == "on" ? true : false);

	//	//读取turbulenceProperties 文件中的printCoeffs字段
	//	QString printCoeffs;
	//	while (!in.atEnd()) {
	//		QString line = in.readLine();
	//		if (line.trimmed().startsWith("printCoeffs")) {
	//			QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	//			if (parts.size() >= 2) {
	//				printCoeffs = parts[1].trimmed();
	//				if (printCoeffs.endsWith(";")) {
	//					printCoeffs.chop(1);
	//				}
	//				break;
	//			}
	//		}
	//	}
	//	ui->checkBox_4->setChecked(printCoeffs == "on" ? true : false);		
	//	return true;
	//}
	//else if(simulationType == "LES"){
	//	for (int i = 0; i < ui->comboBox->count(); ++i) {
	//		if (ui->comboBox->itemText(i) == "LES") {
	//			ui->comboBox->setCurrentIndex(i);
	//			break;
	//		}
	//	}

	//	// 读取 turbulenceProperties 文件中的LESModel字段
	//	QString LESModel;
	//	while (!in.atEnd()) {
	//		QString line = in.readLine();
	//		if (line.trimmed().startsWith("LESModel")) {
	//			QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	//			if (parts.size() >= 2) {
	//				LESModel = parts[1].trimmed();
	//				if (LESModel.endsWith(";")) {
	//					LESModel.chop(1);
	//				}
	//				break;
	//			}
	//		}
	//	}
	//	for (int i = 0; i < ui->comboBox_4->count(); ++i) {
	//		if (ui->comboBox_4->itemText(i) == LESModel) {
	//			ui->comboBox_4->setCurrentIndex(i);
	//			break;
	//		}
	//	}

	//	//读取turbulenceProperties 文件中的turbulence字段
	//	QString turbulence;
	//	while (!in.atEnd()) {
	//		QString line = in.readLine();
	//		if (line.trimmed().startsWith("turbulence")) {
	//			QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	//			if (parts.size() >= 2) {
	//				turbulence = parts[1].trimmed();
	//				if (turbulence.endsWith(";")) {
	//					turbulence.chop(1);
	//				}
	//				break;
	//			}
	//		}
	//	}
	//	ui->checkBox_5->setChecked(turbulence == "on" ? true : false);

	//	//读取turbulenceProperties 文件中的printCoeffs字段
	//	QString printCoeffs;
	//	while (!in.atEnd()) {
	//		QString line = in.readLine();
	//		if (line.trimmed().startsWith("printCoeffs")) {
	//			QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	//			if (parts.size() >= 2) {
	//				printCoeffs = parts[1].trimmed();
	//				if (printCoeffs.endsWith(";")) {
	//					printCoeffs.chop(1);
	//				}
	//				break;
	//			}
	//		}
	//	}
	//	ui->checkBox_6->setChecked(printCoeffs == "on" ? true : false);
	//	return true;
	//}
	//else if (simulationType == "laminar") {
	//	for (int i = 0; i < ui->comboBox->count(); ++i) {
	//		if (ui->comboBox->itemText(i) == "laminar") {
	//			ui->comboBox->setCurrentIndex(i);
	//			break;
	//		}
	//	}
	//	return true;
	//}

	//turbulencePropertiesFile.close();
	return false;
}

bool FormTurbulence::exportParameter()
{
	//// 获取案例路径
	//QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	//QFileInfo fileInfo(casePath);
	//QString caseDir = fileInfo.path();
	//QString turbulencePropertiesPath = caseDir + "/constant/turbulenceProperties";

	//// 打开 turbulencePropertiesPath 文件
	//QFile turbulencePropertiesFile(turbulencePropertiesPath);
	//if (!turbulencePropertiesFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
	//	DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "无法打开 turbulenceProperties 文件" });
	//	dialogInformationPrompt->exec();
	//	return false;
	//}

	//// 读取文件内容
	//QTextStream in(&turbulencePropertiesFile);
	//QString content = in.readAll();
	//turbulencePropertiesFile.close();

	//// 替换 simulationType 字段
	//QString simulationType = ui->comboBox->currentText();
	//content.replace(QRegExp("simulationType\\s+\\w+;"), "simulationType " + simulationType + ";");

	//if (simulationType == "RAS") {
	//	// 替换 RASModel 字段
	//	QString RASModel = ui->comboBox_3->currentText();
	//	content.replace(QRegExp("RASModel\\s+\\w+;"), "RASModel " + RASModel + ";");

	//	// 替换 turbulence 字段
	//	QString turbulence = ui->checkBox_3->isChecked() ? "on" : "off";
	//	content.replace(QRegExp("turbulence\\s+\\w+;"), "turbulence " + turbulence + ";");

	//	// 替换 printCoeffs 字段
	//	QString printCoeffs = ui->checkBox_4->isChecked() ? "on" : "off";
	//	content.replace(QRegExp("printCoeffs\\s+\\w+;"), "printCoeffs " + printCoeffs + ";");
	//}
	//else if (simulationType == "LES") {
	//	// 替换 LESModel 字段
	//	QString LESModel = ui->comboBox_4->currentText();
	//	content.replace(QRegExp("LESModel\\s+\\w+;"), "LESModel " + LESModel + ";");

	//	// 替换 turbulence 字段
	//	QString turbulence = ui->checkBox_5->isChecked() ? "on" : "off";
	//	content.replace(QRegExp("turbulence\\s+\\w+;"), "turbulence " + turbulence + ";");

	//	// 替换 printCoeffs 字段
	//	QString printCoeffs = ui->checkBox_6->isChecked() ? "on" : "off";
	//	content.replace(QRegExp("printCoeffs\\s+\\w+;"), "printCoeffs " + printCoeffs + ";");
	//}

	//// 将修改后的内容写回文件
	//if (!turbulencePropertiesFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
	//	DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this, "错误", { "无法打开 turbulenceProperties 文件" });
	//	dialogInformationPrompt->exec();
	//	return false;
	//}
	//QTextStream out(&turbulencePropertiesFile);
	//out << content;
	//turbulencePropertiesFile.close();

	return true;
}

void FormTurbulence::onComboBoxTextChanged(const QString& text)
{
	//if (text == "RAS") {
	//	ui->tabWidget->show();
	//	for (int i = 0; i < ui->tabWidget->count(); ++i) {
	//		if (ui->tabWidget->tabText(i) == "RAS参数") {
	//			ui->tabWidget->setTabVisible(i, true);
	//			ui->tabWidget->setCurrentIndex(i);
	//		}
	//		else {
	//			ui->tabWidget->setTabVisible(i, false);
	//		}
	//	}
	//}
	//else if (text == "LES") {
	//	ui->tabWidget->show();
	//	for (int i = 0; i < ui->tabWidget->count(); ++i) {
	//		if (ui->tabWidget->tabText(i) == "LES参数") {
	//			ui->tabWidget->setTabVisible(i, true);
	//			ui->tabWidget->setCurrentIndex(i);
	//		}
	//		else {
	//			ui->tabWidget->setTabVisible(i, false);
	//		}
	//	}
	//}
	//else if (text == "laminar") {
	//	ui->tabWidget->hide();
	//}
}