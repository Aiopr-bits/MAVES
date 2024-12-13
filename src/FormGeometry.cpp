#include "FormGeometry.h"

FormGeometry::FormGeometry(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormGeometryClass())
{
	ui->setupUi(this);
}

FormGeometry::~FormGeometry()
{
	delete ui;
}
