#include "FormCellZones.h"

FormCellZones::FormCellZones(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormCellZonesClass())
{
	ui->setupUi(this);
}

FormCellZones::~FormCellZones()
{
	delete ui;
}
