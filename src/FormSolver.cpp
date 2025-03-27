/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#include "FormSolver.h"

FormSolver::FormSolver(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::FormSolverClass())
	, model(new QStandardItemModel(this))
{
	ui->setupUi(this);

	QStringList items;
	items << "transientIncompressibleSolver" << "steadyCompressibleSolver" << "multiRegionSolver";
	for (const QString& itemText : items) {
		QStandardItem* item = new QStandardItem(itemText);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable); 
		model->appendRow(item);
	}
	ui->listView->setModel(model);

	connect(ui->pushButton, &QPushButton::clicked, this, &FormSolver::handleButtonGroup1);
	connect(ui->pushButton_2, &QPushButton::clicked, this, &FormSolver::handleButtonGroup1);
	connect(ui->pushButton_3, &QPushButton::clicked, this, &FormSolver::handleButtonGroup2);
	connect(ui->pushButton_4, &QPushButton::clicked, this, &FormSolver::handleButtonGroup2);
	connect(ui->checkBox, &QCheckBox::stateChanged, this, &FormSolver::updateListView);
	connect(ui->checkBox_2, &QCheckBox::stateChanged, this, &FormSolver::updateListView);
}

FormSolver::~FormSolver()
{
	delete ui;
}

bool FormSolver::importParameter()
{
	//获取案例路径
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QFileInfo fileInfo(casePath);
	QString caseDir = fileInfo.path();
	QString controlDictPath = caseDir + "/system/controlDict";

	// 打开 controlDict 文件
	QFile controlDictFile(controlDictPath);
	if (!controlDictFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("错误"), tr("无法打开 controlDict 文件"));
		return false;
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
		QMessageBox::warning(this, tr("错误"), tr("未找到 application 字段"));
		return false;
	}

	if (application == "rhoSimpleFoam")
	{
		ui->pushButton->setChecked(true);
		ui->pushButton_2->setChecked(false);
		ui->pushButton_3->setChecked(false);
		ui->pushButton_4->setChecked(true);
		ui->checkBox->setChecked(true);
		ui->checkBox_2->setChecked(true);
		ui->label_8->setText("steadyCompressibleSolver");
		GlobalData::getInstance().getCaseData()->solverName = "rhoSimpleFoam";
		emit labelText_8_Changed("rhoSimpleFoam");
		return true;
	}
	else if (application == "buoyantBoussinesqPimpleFoam")
	{
		ui->pushButton->setChecked(false);
		ui->pushButton_2->setChecked(true);
		ui->pushButton_3->setChecked(true);
		ui->pushButton_4->setChecked(false);
		ui->checkBox->setChecked(false);
		ui->checkBox_2->setChecked(true);
		ui->label_8->setText("transientIncompressibleSolver");
		GlobalData::getInstance().getCaseData()->solverName = "buoyantBoussinesqPimpleFoam";
		emit labelText_8_Changed("buoyantBoussinesqPimpleFoam");
		return true;
	}
	else if (application == "chtMultiRegionFoam")
	{
		ui->pushButton->setChecked(false);
		ui->pushButton_2->setChecked(true);
		ui->pushButton_3->setChecked(false);
		ui->pushButton_4->setChecked(true);
		ui->checkBox->setChecked(true);
		ui->checkBox_2->setChecked(true);
		ui->label_8->setText("multiRegionSolver");
		GlobalData::getInstance().getCaseData()->solverName = "chtMultiRegionFoam";
		emit labelText_8_Changed("chtMultiRegionFoam");
		return true;
	}
	else {
		ui->pushButton->setChecked(false);
		ui->pushButton_2->setChecked(false);
		ui->pushButton_3->setChecked(false);
		ui->pushButton_4->setChecked(false);
		ui->checkBox->setChecked(false);
		ui->label_8->setText("");
		GlobalData::getInstance().getCaseData()->solverName = "";
		QMessageBox::warning(this, tr("错误"), tr("暂不支持该类型案例"));
		return false;
	}
}

bool FormSolver::exportParameter()
{
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QString caseDirPath = QFileInfo(casePath).absolutePath();
	QString controlDictPath = caseDirPath + "/system/controlDict";
	// 打开 controlDict 文件
	QFile controlDictFile(controlDictPath);
	if (!controlDictFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
		QMessageBox::warning(this, "错误", "无法打开文件: " + controlDictPath);
		return false;
	}
	// 读取 controlDict 文件内容
	QTextStream in(&controlDictFile);
	QString content = in.readAll();
	controlDictFile.close();
	// 替换 controlDict 文件中的 application 字段
	QString application = ui->label_8->text();
	if (application != "steadyCompressibleSolver" && application != "transientIncompressibleSolver" && application != "multiRegionSolver")
	{
		QMessageBox::warning(this, "错误", "求解器参数配置错误");
		return false;
	}

	if (application == "steadyCompressibleSolver")
	{
		application = "rhoSimpleFoam";
	}
	else if (application == "transientIncompressibleSolver")
	{
		application = "buoyantBoussinesqPimpleFoam";
	}
	else if (application == "multiRegionSolver")
	{
		application = "chtMultiRegionFoam";
	}

	QRegExp rx("application\\s+\\S+;");
	rx.setMinimal(true);
	content.replace(rx, "application     " + application + ";");

	// 将修改后的内容写入 controlDict 文件
	if (!controlDictFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		QMessageBox::warning(this, "错误", "无法打开文件: " + controlDictPath);
		return false;
	}
	QTextStream out(&controlDictFile);
	out << content;
	controlDictFile.close();
	return true;
}

void FormSolver::handleButtonGroup1()
{
	QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
	if (senderButton == ui->pushButton) {
		if (ui->pushButton->isChecked()) {
			ui->pushButton_2->setChecked(false);
		}
	}
	else if (senderButton == ui->pushButton_2) {
		if (ui->pushButton_2->isChecked()) {
			ui->pushButton->setChecked(false);
		}
	}
	updateListView();
}

void FormSolver::handleButtonGroup2()
{
	QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
	if (senderButton == ui->pushButton_3) {
		if (ui->pushButton_3->isChecked()) {
			ui->pushButton_4->setChecked(false);
		}
	}
	else if (senderButton == ui->pushButton_4) {
		if (ui->pushButton_4->isChecked()) {
			ui->pushButton_3->setChecked(false);
		}
	}
	updateListView();
}

void FormSolver::updateListView()
{
	//所有item均设置为可用
	for (int i = 0; i < model->rowCount(); ++i) {
		QStandardItem* item = model->item(i);
		item->setEnabled(true);
	}

	if (ui->pushButton->isChecked()) {
		for (int i = 0; i < model->rowCount(); ++i) {
			QStandardItem* item = model->item(i);
			if (item->text() == "transientIncompressibleSolver" || item->text() == "multiRegionSolver") {
				item->setEnabled(false);
			}
		}
	}

	if (ui->pushButton_2->isChecked()) {
		for (int i = 0; i < model->rowCount(); ++i) {
			QStandardItem* item = model->item(i);
			if (item->text() == "steadyCompressibleSolver" ) {
				item->setEnabled(false);
			}
		}
	}

	if (ui->pushButton_3->isChecked()) {
		for (int i = 0; i < model->rowCount(); ++i) {
			QStandardItem* item = model->item(i);
			if (item->text() == "steadyCompressibleSolver" || item->text() == "multiRegionSolver") {
				item->setEnabled(false);
			}
		}
	}

	if (ui->pushButton_4->isChecked()) {
		for (int i = 0; i < model->rowCount(); ++i) {
			QStandardItem* item = model->item(i);
			if (item->text() == "transientIncompressibleSolver") {
				item->setEnabled(false);
			}
		}
	}

	if (ui->checkBox->isChecked()) {
		for (int i = 0; i < model->rowCount(); ++i) {
			QStandardItem* item = model->item(i);
			if (item->text() == "steadyCompressibleSolver") {
				item->setEnabled(false);
			}
		}
	}

	if (ui->checkBox_2->isChecked()) {

	}
}

void FormSolver::on_pushButton_5_clicked()
{
	if(GlobalData::getInstance().getCaseData()->meshPatchNamesMap.empty())
	{
		//新建一个DialogInformationPrompt窗口并弹出
		DialogInformationPrompt* dialogInformationPrompt = new DialogInformationPrompt(this,{ "请先导入网格!"}, true);
		dialogInformationPrompt->exec();
		return;
	}

	QModelIndexList selectedIndexes = ui->listView->selectionModel()->selectedIndexes();
	if (!selectedIndexes.isEmpty()) {
		QString selectedText = selectedIndexes.first().data().toString();
		ui->label_8->setText(selectedText);

		if (selectedText == "steadyCompressibleSolver")
		{
			selectedText = "rhoSimpleFoam";
		}
		else if (selectedText == "transientIncompressibleSolver")
		{
			selectedText = "buoyantBoussinesqPimpleFoam";
		}
		else if (selectedText == "multiRegionSolver")
		{
			selectedText = "chtMultiRegionFoam";
		}

		GlobalData::getInstance().getCaseData()->solverName = selectedText.toStdString();
		emit labelText_8_Changed(selectedText);
	}
}
