#include "FormBoundaryConditions.h"

FormBoundaryConditions::FormBoundaryConditions(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormBoundaryConditionsClass())
{
	ui->setupUi(this);
}

FormBoundaryConditions::~FormBoundaryConditions()
{
	delete ui;
}
