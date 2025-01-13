#include "FormBoundaryConditionsTabWidget.h"

FormBoundaryConditionsTabWidget::FormBoundaryConditionsTabWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormBoundaryConditionsTabWidgetClass())
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

	connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxCurrentIndexChanged(int)));
	connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_2CurrentIndexChanged(int)));
	connect(ui->comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_3CurrentIndexChanged(int)));
	connect(ui->comboBox_4, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_4CurrentIndexChanged(int)));
	connect(ui->comboBox_5, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_5CurrentIndexChanged(int)));
	connect(ui->comboBox_6, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_6CurrentIndexChanged(int)));
	connect(ui->comboBox_7, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_7CurrentIndexChanged(int)));
}

FormBoundaryConditionsTabWidget::~FormBoundaryConditionsTabWidget()
{
	delete ui;
}

void FormBoundaryConditionsTabWidget::onComboBoxCurrentIndexChanged(int index)
{
	
}

void FormBoundaryConditionsTabWidget::onComboBox_2CurrentIndexChanged(int index)
{
}

void FormBoundaryConditionsTabWidget::onComboBox_3CurrentIndexChanged(int index)
{
}

void FormBoundaryConditionsTabWidget::onComboBox_4CurrentIndexChanged(int index)
{
}

void FormBoundaryConditionsTabWidget::onComboBox_5CurrentIndexChanged(int index)
{
}

void FormBoundaryConditionsTabWidget::onComboBox_6CurrentIndexChanged(int index)
{
}

void FormBoundaryConditionsTabWidget::onComboBox_7CurrentIndexChanged(int index)
{
}


