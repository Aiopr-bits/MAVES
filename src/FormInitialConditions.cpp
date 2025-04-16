#include "FormInitialConditions.h"

FormInitialConditions::FormInitialConditions(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormInitialConditionsClass())
{
	ui->setupUi(this);
}

FormInitialConditions::~FormInitialConditions()
{
	delete ui;
}
