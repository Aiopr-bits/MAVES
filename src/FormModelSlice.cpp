#include "FormModelSlice.h"

FormModelSlice::FormModelSlice(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::ModelSliceClass())
{
	ui->setupUi(this);
}

FormModelSlice::~FormModelSlice()
{
	delete ui;
}
