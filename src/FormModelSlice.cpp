#include "FormModelSlice.h"

FormModelSlice::FormModelSlice(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormModelSliceClass())
{
	ui->setupUi(this);
}

FormModelSlice::~FormModelSlice()
{
	delete ui;
}
