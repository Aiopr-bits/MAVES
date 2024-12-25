#include "FormModelClip.h"

FormModelClip::FormModelClip(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormModelClipClass())
{
	ui->setupUi(this);

	connect(ui->checkBox, &QCheckBox::toggled, this, &FormModelClip::checkBoxToggled);
}

FormModelClip::~FormModelClip()
{
	delete ui;
}

