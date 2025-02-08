#include "FormSolver.h"

FormSolver::FormSolver(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::FormSolverClass())
	, model(new QStandardItemModel(this))
{
	ui->setupUi(this);

	QStringList items;
	items << "buoyantBoussinesqPimpleFoam" << "rhoSimpleFoam";
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
	connect(ui->checkBox, &QCheckBox::stateChanged, this, &FormSolver::handleCheckBoxGroup);
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

	if (application == "rhoSimpleFoam" )
	{
		ui->pushButton->setChecked(true);
		ui->pushButton_2->setChecked(false);
		ui->pushButton_3->setChecked(false);
		ui->pushButton_4->setChecked(true);
		ui->checkBox->setChecked(true);	
		ui->label_8->setText("rhoSimpleFoam");
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
		ui->label_8->setText("buoyantBoussinesqPimpleFoam");
		GlobalData::getInstance().getCaseData()->solverName = "buoyantBoussinesqPimpleFoam";
		emit labelText_8_Changed("buoyantBoussinesqPimpleFoam");
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
	if (application != "rhoSimpleFoam" && application != "buoyantBoussinesqPimpleFoam")
	{
		QMessageBox::warning(this, "错误", "求解器参数配置错误");
		return false;
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
			ui->pushButton_4->setChecked(true);
			ui->pushButton_2->setChecked(false);
			ui->pushButton_3->setChecked(false);
			ui->checkBox->setChecked(true);
		}
		else {
			ui->pushButton_4->setChecked(false);
			ui->checkBox->setChecked(false);
		}
	}
	else if (senderButton == ui->pushButton_2) {
		if (ui->pushButton_2->isChecked()) {
			ui->pushButton_3->setChecked(true);
			ui->pushButton->setChecked(false);
			ui->pushButton_4->setChecked(false);
			ui->checkBox->setChecked(false);
		}
		else {
			ui->pushButton_3->setChecked(false);
		}
	}
	updateListView();
}

void FormSolver::handleButtonGroup2()
{
	QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
	if (senderButton == ui->pushButton_3) {
		if (ui->pushButton_3->isChecked()) {
			ui->pushButton_2->setChecked(true);
			ui->pushButton->setChecked(false);
			ui->pushButton_4->setChecked(false);
			ui->checkBox->setChecked(false);
		}
		else {
			ui->pushButton_2->setChecked(false);
		}
	}
	else if (senderButton == ui->pushButton_4) {
		if (ui->pushButton_4->isChecked()) {
			ui->pushButton->setChecked(true);
			ui->pushButton_2->setChecked(false);
			ui->pushButton_3->setChecked(false);
			ui->checkBox->setChecked(true);
		}
		else {
			ui->pushButton->setChecked(false);
			ui->checkBox->setChecked(false);
		}
	}
	updateListView();
}

void FormSolver::handleCheckBoxGroup(int state)
{
	if (state == Qt::Checked) {
		ui->pushButton->setChecked(true);
		ui->pushButton_4->setChecked(true);
		ui->pushButton_2->setChecked(false);
		ui->pushButton_3->setChecked(false);
	}
	else {
		ui->pushButton->setChecked(false);
		ui->pushButton_4->setChecked(false);
	}
	updateListView();
}

void FormSolver::updateListView()
{
	if (ui->pushButton->isChecked() && ui->pushButton_4->isChecked() && ui->checkBox->isChecked()) {
		for (int i = 0; i < model->rowCount(); ++i) {
			QStandardItem* item = model->item(i);
			if (item->text() == "rhoSimpleFoam") {
				item->setEnabled(true);
			}
			else {
				item->setEnabled(false);
			}
		}
	}
	else if (ui->pushButton_2->isChecked() && ui->pushButton_3->isChecked() && !ui->checkBox->isChecked()) {
		for (int i = 0; i < model->rowCount(); ++i) {
			QStandardItem* item = model->item(i);
			if (item->text() == "buoyantBoussinesqPimpleFoam") {
				item->setEnabled(true);
			}
			else {
				item->setEnabled(false);
			}
		}
	}
	else {
		for (int i = 0; i < model->rowCount(); ++i) {
			QStandardItem* item = model->item(i);
			item->setEnabled(true);
		}
	}
}

void FormSolver::on_pushButton_5_clicked()
{
	QModelIndexList selectedIndexes = ui->listView->selectionModel()->selectedIndexes();
	if (!selectedIndexes.isEmpty()) {
		QString selectedText = selectedIndexes.first().data().toString();
		ui->label_8->setText(selectedText);
		GlobalData::getInstance().getCaseData()->solverName = "selectedText";
		emit labelText_8_Changed(selectedText);
	}
}
