#include "FormTransportProperties.h"
#include <QRegularExpressionValidator>

FormTransportProperties::FormTransportProperties(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormTransportPropertiesClass())
{
	ui->setupUi(this);

	// 输入限制
	QRegularExpression regex("^-?(\\d+(\\.\\d*)?|\\.\\d+)([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, this);
	const auto lineEdits = this->findChildren<QLineEdit*>();
	for (QLineEdit* lineEdit : lineEdits) {
		lineEdit->setValidator(validator);
	}

	connect(ui->comboBox, QOverload<int>::of(&QComboBox::activated), this, &FormTransportProperties::on_comboBox_itemActivated);
	on_comboBox_itemActivated(0);
}

FormTransportProperties::~FormTransportProperties()
{
	delete ui;
}

void FormTransportProperties::on_comboBox_itemActivated(int index)
{
	QString currentText = ui->comboBox->itemText(index);

	ui->widget_2->hide();
	ui->widget_3->hide();
	ui->widget_4->hide();
	ui->widget_5->hide();
	ui->widget_6->hide();
	ui->widget_7->hide();

	if (currentText == "NewTonian")
	{
		ui->widget_2->show();
	}
	else if (currentText == "Power Law")
	{
		ui->widget_3->show();
	}
	else if (currentText == "Cross-Power Law")
	{
		ui->widget_4->show();
	}
	else if (currentText == "Bird-Carreau")
	{
		ui->widget_5->show();
	}
	else if (currentText == "Herschel-Bulkley")
	{
		ui->widget_3->show();
	}
	else if (currentText == "Casson")
	{
		ui->widget_7->show();
	}
}
