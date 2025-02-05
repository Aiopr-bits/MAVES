#include "FormPhysicalPropertyParameter.h"

FormPhysicalPropertyParameter::FormPhysicalPropertyParameter(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormPhysicalPropertyParameterClass())
{
	ui->setupUi(this);
}

FormPhysicalPropertyParameter::~FormPhysicalPropertyParameter()
{
	delete ui;
}
