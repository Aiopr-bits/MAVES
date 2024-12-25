#include "FormModelClip.h"

FormModelClip::FormModelClip(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormModelClipClass())
{
	ui->setupUi(this);
}

FormModelClip::~FormModelClip()
{
	delete ui;
}
