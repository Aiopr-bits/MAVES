#include "FormTurbulence.h"

FormTurbulence::FormTurbulence(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormTurbulenceClass())
{
	ui->setupUi(this);
}

FormTurbulence::~FormTurbulence()
{
	delete ui;
}
