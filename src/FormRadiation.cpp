#include "FormRadiation.h"
#include <QRegularExpressionValidator>

FormRadiation::FormRadiation(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormRadiationClass())
{
	ui->setupUi(this);

	// 输入限制
	QRegularExpression regex("^-?(\\d+(\\.\\d*)?|\\.\\d+)([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, this);
	const auto lineEdits = this->findChildren<QLineEdit*>();
	for (QLineEdit* lineEdit : lineEdits) {
		lineEdit->setValidator(validator);
	}

	connect(ui->checkBox, &QCheckBox::toggled, this, &FormRadiation::on_CheckBox_Toggled);
	connect(ui->pushButton_3, &QPushButton::toggled, this, &FormRadiation::on_PushButton_3_Toggled);
	connect(ui->pushButton_4, &QPushButton::toggled, this, &FormRadiation::on_PushButton_4_Toggled);
	connect(ui->pushButton_5, &QPushButton::toggled, this, &FormRadiation::on_PushButton_5_Toggled);
	connect(ui->pushButton_6, &QPushButton::toggled, this, &FormRadiation::on_PushButton_6_Toggled);
	connect(ui->pushButton_7, &QPushButton::toggled, this, &FormRadiation::on_PushButton_7_Toggled);
	connect(ui->comboBox_5, QOverload<int>::of(&QComboBox::activated), this, &FormRadiation::onComboBox_5_IndexChanged);
	connect(ui->comboBox_6, QOverload<int>::of(&QComboBox::activated), this, &FormRadiation::onComboBox_6_IndexChanged);
	connect(ui->comboBox_7, QOverload<int>::of(&QComboBox::activated), this, &FormRadiation::onComboBox_7_IndexChanged);
	connect(ui->comboBox_8, QOverload<int>::of(&QComboBox::activated), this, &FormRadiation::onComboBox_8_IndexChanged);
	connect(ui->comboBox_9, QOverload<int>::of(&QComboBox::activated), this, &FormRadiation::onComboBox_9_IndexChanged);

	on_CheckBox_Toggled(true);
	on_PushButton_3_Toggled(false);
	on_PushButton_4_Toggled(false);
	on_PushButton_5_Toggled(false);
	on_PushButton_6_Toggled(false);
	on_PushButton_7_Toggled(false);
	onComboBox_5_IndexChanged(0);
	onComboBox_6_IndexChanged(0);
	onComboBox_7_IndexChanged(0);
	onComboBox_8_IndexChanged(0);
	onComboBox_9_IndexChanged(0);
}

FormRadiation::~FormRadiation()
{
	delete ui;
}

void FormRadiation::on_CheckBox_Toggled(bool checked)
{
	if (checked)
	{
		ui->widget->show();
	}
	else
	{
		ui->widget->hide();
	}
}

void FormRadiation::on_PushButton_3_Toggled(bool checked)
{
	if (checked)
	{
		ui->widget_4->show();
	}
	else
	{
		ui->widget_4->hide();
	}
}

void FormRadiation::on_PushButton_4_Toggled(bool checked)
{
	if (checked)
	{
		ui->widget_12->show();
	}
	else
	{
		ui->widget_12->hide();
	}
}

void FormRadiation::on_PushButton_5_Toggled(bool checked)
{
	if (checked)
	{
		ui->widget_13->show();
	}
	else
	{
		ui->widget_13->hide();
	}
}

void FormRadiation::on_PushButton_6_Toggled(bool checked)
{
	if (checked)
	{
		ui->widget_14->show();
	}
	else
	{
		ui->widget_14->hide();
	}
}

void FormRadiation::on_PushButton_7_Toggled(bool checked)
{
	if (checked)
	{
		ui->widget_15->show();
	}
	else
	{
		ui->widget_15->hide();
	}
}

void FormRadiation::onComboBox_5_IndexChanged(int index)
{
	QString text = ui->comboBox_5->currentText();
	ui->widget_2->hide();
	ui->widget_3->hide();
	ui->widget_11->hide();
	ui->pushButton_3->setChecked(false);
	ui->pushButton_3->setEnabled(false);

	if (text == "none")
	{
		ui->widget_11->show();
	}
	else if (text == "P1")
	{
	}
	else if (text == "Discrete Ordinates")
	{
		ui->widget_2->show();
		ui->widget_11->show();
		ui->pushButton_3->setEnabled(true);
	}
	else if (text == "Surface to Surface")
	{
		ui->widget_3->show();
		ui->widget_11->show();
		ui->pushButton_3->setEnabled(true);
	}
}

void FormRadiation::onComboBox_6_IndexChanged(int index)
{
	QString text = ui->comboBox_6->currentText();

	ui->widget_5->hide();
	ui->pushButton_4->setChecked(false);
	ui->pushButton_4->setEnabled(false);
	if (text == "none")
	{
		
	}
	else if (text == "Constant")
	{
		ui->widget_5->show();
		ui->pushButton_4->setEnabled(true);
	}
}

void FormRadiation::onComboBox_7_IndexChanged(int index)
{
	QString text = ui->comboBox_7->currentText();

	ui->widget_6->hide();
	ui->pushButton_5->setChecked(false);
	ui->pushButton_5->setEnabled(false);
	if (text == "none")
	{
	}
	else if (text == "Constant")
	{
		ui->widget_6->show();
		ui->pushButton_5->setEnabled(true);
	}
}

void FormRadiation::onComboBox_8_IndexChanged(int index)
{
	QString text = ui->comboBox_8->currentText();

	ui->widget_7->hide();
	ui->widget_8->hide();

	if (text == "Constant")
	{
		ui->widget_7->show();
	}
	else if (text == "Sun Tracking")
	{
		ui->widget_8->show();
	}
}

void FormRadiation::onComboBox_9_IndexChanged(int index)
{
	QString text = ui->comboBox_9->currentText();

	ui->widget_9->hide();
	if (text == "Constant")
	{
		ui->widget_9->show();
	}
}