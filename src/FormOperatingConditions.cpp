#include "FormOperatingConditions.h"

FormOperatingConditions::FormOperatingConditions(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormOperatingConditionsClass())
{
	ui->setupUi(this);
}

FormOperatingConditions::~FormOperatingConditions()
{
	delete ui;
}
