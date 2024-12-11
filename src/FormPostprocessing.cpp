#include "FormPostprocessing.h"

FormPostprocessing::FormPostprocessing(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormPostprocessingClass())
{
	ui->setupUi(this);
}

FormPostprocessing::~FormPostprocessing()
{
	delete ui;
}
