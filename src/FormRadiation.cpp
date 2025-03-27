#include "FormRadiation.h"

FormRadiation::FormRadiation(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormRadiationClass())
{
	ui->setupUi(this);
}

FormRadiation::~FormRadiation()
{
	delete ui;
}
