#include "FormOperatingConditions.h"

FormOperatingConditions::FormOperatingConditions(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormOperatingConditionsClass())
{
	ui->setupUi(this);

	// 输入限制
	QRegularExpression regex("^-?(\\d+(\\.\\d*)?|\\.\\d+)([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, this);
	const auto lineEdits = this->findChildren<QLineEdit*>();
	for (QLineEdit* lineEdit : lineEdits) {
		lineEdit->setValidator(validator);
	}

	connect(ui->radioButton_7, &QRadioButton::toggled, this, &FormOperatingConditions::on_RadioButton_7_Toggled);
	connect(ui->radioButton_8, &QRadioButton::toggled, this, &FormOperatingConditions::on_RadioButton_8_Toggled);

	on_RadioButton_7_Toggled(true);
}

FormOperatingConditions::~FormOperatingConditions()
{
	delete ui;
}

void FormOperatingConditions::on_RadioButton_7_Toggled(bool checked)
{
	ui->widget->show();
	ui->widget_2->hide();
}

void FormOperatingConditions::on_RadioButton_8_Toggled(bool checked)
{
	ui->widget_2->show();
	ui->widget->hide();
}