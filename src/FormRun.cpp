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

