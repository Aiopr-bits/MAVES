#include "ModelSlice.h"

ModelSlice::ModelSlice(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::ModelSliceClass())
{
	ui->setupUi(this);
}

ModelSlice::~ModelSlice()
{
	delete ui;
}
