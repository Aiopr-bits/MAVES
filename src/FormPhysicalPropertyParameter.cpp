#include "FormPhysicalPropertyParameter.h"
#include "FormSolver.h"

FormPhysicalPropertyParameter::FormPhysicalPropertyParameter(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormPhysicalPropertyParameterClass())
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

FormPhysicalPropertyParameter::~FormPhysicalPropertyParameter()
{
	delete ui;
}

void FormPhysicalPropertyParameter::solverChanged(const QString& newText)
{
	if (newText == "rhoSimpleFoam") {
		ui->widget->show();
		ui->widget_2->hide();
	}
	else if (newText == "buoyantBoussinesqPimpleFoam") {
		ui->widget->hide();
		ui->widget_2->show();
	}
	else {
		ui->widget->hide();
		ui->widget_2->hide();
	}
}