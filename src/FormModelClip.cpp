#include "FormModelClip.h"
#include <QRegularExpressionValidator> 

FormModelClip::FormModelClip(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormModelClipClass())
{
	ui->setupUi(this);

	// 设置 lineEdit 只支持整数和小数输入
	QRegularExpression regExp("^-?\\d*\\.?\\d+([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regExp, this);
	ui->lineEdit->setValidator(validator);
	ui->lineEdit_2->setValidator(validator);
	ui->lineEdit_3->setValidator(validator);
	ui->lineEdit_4->setValidator(validator);
	ui->lineEdit_5->setValidator(validator);
	ui->lineEdit_6->setValidator(validator);

	connect(ui->checkBox, &QCheckBox::toggled, this, &FormModelClip::checkBoxToggled);
	connect(ui->lineEdit, &QLineEdit::textChanged, this, &FormModelClip::onLineEditChanged);
	connect(ui->lineEdit_2, &QLineEdit::textChanged, this, &FormModelClip::onLineEditChanged);
	connect(ui->lineEdit_3, &QLineEdit::textChanged, this, &FormModelClip::onLineEditChanged);
	connect(ui->lineEdit_4, &QLineEdit::textChanged, this, &FormModelClip::onLineEditChanged);
	connect(ui->lineEdit_5, &QLineEdit::textChanged, this, &FormModelClip::onLineEditChanged);
	connect(ui->lineEdit_6, &QLineEdit::textChanged, this, &FormModelClip::onLineEditChanged);
}

FormModelClip::~FormModelClip()
{
	delete ui;
}

void FormModelClip::onLineEditChanged()
{
	emit lineEditsChanged();
}

