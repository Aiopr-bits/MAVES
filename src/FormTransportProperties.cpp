#include "FormTransportProperties.h"

FormTransportProperties::FormTransportProperties(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormTransportPropertiesClass())
{
	ui->setupUi(this);
}

FormTransportProperties::~FormTransportProperties()
{
	delete ui;
}
