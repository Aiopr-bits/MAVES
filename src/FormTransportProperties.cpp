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

	ui->lineEdit_53->setValidator(validator);

	ui->lineEdit_54->setValidator(validator);
	ui->lineEdit_55->setValidator(validator);
	ui->lineEdit_56->setValidator(validator);
	ui->lineEdit_57->setValidator(validator);

	ui->lineEdit_58->setValidator(validator);
	ui->lineEdit_59->setValidator(validator);
	ui->lineEdit_60->setValidator(validator);
	ui->lineEdit_61->setValidator(validator);

	ui->lineEdit_62->setValidator(validator);
	ui->lineEdit_63->setValidator(validator);
	ui->lineEdit_64->setValidator(validator);
	ui->lineEdit_65->setValidator(validator);
	ui->lineEdit_66->setValidator(validator);

	ui->lineEdit_67->setValidator(validator);
	ui->lineEdit_68->setValidator(validator);
	ui->lineEdit_69->setValidator(validator);
	ui->lineEdit_70->setValidator(validator);

	ui->lineEdit_71->setValidator(validator);
	ui->lineEdit_72->setValidator(validator);
	ui->lineEdit_73->setValidator(validator);
	ui->lineEdit_74->setValidator(validator);

	ui->lineEdit_2->setValidator(validator);
	ui->lineEdit_3->setValidator(validator);
	ui->lineEdit_4->setValidator(validator);
	ui->lineEdit_5->setValidator(validator);
	ui->lineEdit_6->setValidator(validator);
	ui->lineEdit_7->setValidator(validator);

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
