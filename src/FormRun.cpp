#include "FormRun.h"
#include <QRegularExpressionValidator> 
#include <QThread>

FormRun::FormRun(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormRunClass())
{
	ui->setupUi(this);

	ui->label_6->hide();
	ui->label_12->hide();
	ui->pushButton_2->hide();

	// 设置 lineEdit 只支持整数和小数输入
	QRegularExpression regExp("^-?\\d*\\.?\\d+([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regExp, this);
	ui->lineEdit->setValidator(validator);
	ui->lineEdit_2->setValidator(validator);
	ui->lineEdit_3->setValidator(validator);

	// 设置 lineEdit_4 只支持正整数输入
	QIntValidator* intValidator = new QIntValidator(1, INT_MAX, this);
	ui->lineEdit_4->setValidator(intValidator);

	connect(ui->pushButton, &CustomHoverPushButton::cursorEnter, this, &FormRun::cursorEnterPushButton);
	connect(ui->pushButton, &CustomHoverPushButton::cursorLeave, this, &FormRun::cursorLeavePushButton);
	connect(ui->pushButton_2, &CustomHoverPushButton::clicked, this, &FormRun::on_pushButton_clicked_2);
}

FormRun::~FormRun()
{
	delete ui;
}

void FormRun::importParameter()
{
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QString caseDirPath = QFileInfo(casePath).absolutePath();
	QString filePath = caseDirPath + "/system/controlDict";

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
		return;
	}

	QTextStream in(&file);
	QString content = in.readAll();
	file.close();

	QRegularExpression startTimeRegex(R"(startTime\s+(\d*\.?\d+);)");
	QRegularExpression endTimeRegex(R"(endTime\s+(\d*\.?\d+);)");
	QRegularExpression deltaTRegex(R"(deltaT\s+(\d*\.?\d+);)");
	QRegularExpression writeIntervalRegex(R"(writeInterval\s+(\d*\.?\d+);)");

	QRegularExpressionMatch startTimeMatch = startTimeRegex.match(content);
	QRegularExpressionMatch endTimeMatch = endTimeRegex.match(content);
	QRegularExpressionMatch deltaTMatch = deltaTRegex.match(content);
	QRegularExpressionMatch writeIntervalMatch = writeIntervalRegex.match(content);

	if (startTimeMatch.hasMatch()) {
		ui->lineEdit->setText(startTimeMatch.captured(1));
	}
	if (endTimeMatch.hasMatch()) {
		ui->lineEdit_2->setText(endTimeMatch.captured(1));
	}
	if (deltaTMatch.hasMatch()) {
		ui->lineEdit_3->setText(deltaTMatch.captured(1));
	}
	if (writeIntervalMatch.hasMatch()) {
		ui->lineEdit_4->setText(writeIntervalMatch.captured(1));
	}
}

void FormRun::exportParameter()
{
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QString caseDirPath = QFileInfo(casePath).absolutePath();
	QString filePath = caseDirPath + "/system/controlDict";

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
		return;
	}

	QTextStream in(&file);
	QString content = in.readAll();
	file.close();

	// 从界面上读取值
	QString startTime = ui->lineEdit->text();
	QString endTime = ui->lineEdit_2->text();
	QString deltaT = ui->lineEdit_3->text();
	QString writeInterval = ui->lineEdit_4->text();

	// 使用正则表达式替换对应的数值
	content.replace(QRegularExpression(R"(startTime\s+\d*\.?\d+;)"), "startTime       " + startTime + ";");
	content.replace(QRegularExpression(R"(endTime\s+\d*\.?\d+;)"), "endTime         " + endTime + ";");
	content.replace(QRegularExpression(R"(deltaT\s+\d*\.?\d+;)"), "deltaT          " + deltaT + ";");
	content.replace(QRegularExpression(R"(writeInterval\s+\d*\.?\d+;)"), "writeInterval   " + writeInterval + ";");

	// 将修改后的内容写回文件
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
		return;
	}
	QTextStream out(&file);
	out << content;
	file.close();
}

void FormRun::cursorEnterPushButton()
{
	ui->label_6->show();
}


void FormRun::cursorLeavePushButton()
{
	ui->label_6->hide();
}

void FormRun::on_pushButton_clicked()
{
	emit run();
}

void FormRun::on_pushButton_clicked_2()
{
	emit stopRun();
}

