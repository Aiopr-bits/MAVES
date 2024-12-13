#include "FormRun.h"

FormRun::FormRun(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormRunClass())
{
	ui->setupUi(this);
	ui->label_6->hide();

	connect(ui->pushButton, &CustomPushButton::cursorEnter, this, &FormRun::cursorEnter);
	connect(ui->pushButton, &CustomPushButton::cursorLeave, this, &FormRun::cursorLeave);
}

FormRun::~FormRun()
{
	delete ui;
}

void FormRun::cursorEnter()
{
	ui->label_6->show();
}


void FormRun::cursorLeave()
{
	ui->label_6->hide();
}


