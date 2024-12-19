#include "FormRun.h"
#include <QRegularExpressionValidator> 

FormRun::FormRun(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormRunClass())
{
	ui->setupUi(this);

	ui->label_6->hide();

	// 设置 lineEdit 只支持整数和小数输入
	QRegularExpression regExp("^-?\\d*\\.?\\d+([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regExp, this);
	ui->lineEdit->setValidator(validator);
	ui->lineEdit_2->setValidator(validator);
	ui->lineEdit_3->setValidator(validator);

	// 设置 lineEdit_4 只支持正整数输入
	QIntValidator* intValidator = new QIntValidator(1, INT_MAX, this);
	ui->lineEdit_4->setValidator(intValidator);

	connect(ui->pushButton, &CustomHoverPushButton::cursorEnter, this, &FormRun::cursorEnter);
	connect(ui->pushButton, &CustomHoverPushButton::cursorLeave, this, &FormRun::cursorLeave);
}

FormRun::~FormRun()
{
	delete ui;
}

void FormRun::cursorEnter()
{
	ui->label_6->show();
}


void FormRun::cursorLeave()
{
	ui->label_6->hide();
}


