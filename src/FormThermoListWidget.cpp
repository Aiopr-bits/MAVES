#include "FormThermoListWidget.h"
#include <QRegularExpressionValidator>

FormThermoListWidget::FormThermoListWidget(QWidget *parent, QString widgetType)
	: QMainWindow(parent)
	, ui(new Ui::FormThermoListWidgetClass())
{
	ui->setupUi(this);
	this->widgetType = widgetType;
	initialize(widgetType);

	// 输入限制
	QRegularExpression regex("^-?(\\d+(\\.\\d*)?|\\.\\d+)([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, this);
	const auto lineEdits = this->findChildren<QLineEdit*>();
	for (QLineEdit* lineEdit : lineEdits) {
		lineEdit->setValidator(validator);
	}

	connect(ui->comboBox_6, QOverload<int>::of(&QComboBox::activated), this, &FormThermoListWidget::on_comboBox_6_itemActivated);
	connect(ui->comboBox_7, QOverload<int>::of(&QComboBox::activated), this, &FormThermoListWidget::on_comboBox_7_itemActivated);
	connect(ui->comboBox_8, QOverload<int>::of(&QComboBox::activated), this, &FormThermoListWidget::on_comboBox_8_itemActivated);
}

FormThermoListWidget::~FormThermoListWidget()
{
	delete ui;
}

void FormThermoListWidget::initialize(QString widgetType)
{
	//定义combox组
	equationOfState1 = {
	"Perfect Gas",
	"Boussinesq",
	"Incompressible Perfect Gas",
	"Constant Density",
	"Perfect Fluid",
	"Adiabatic Perfect Fluid",
	"Polynomial",
	"Peng-Robinson",
	};

	equationOfState2 = {
		"Constant Density",
	};

	thermodynamics1 = {
		"Constant",
		"Janaf",
	};

	thermodynamics2 = {
		"Constant",
	};

	thermodynamics3 = {
	"Polynomial",
	};

	thermodynamics4 = {
		"Constant",
		"Polynomial",
		"Janaf",
	};

	thermodynamics5 = {
		"Constant",
		"Polynomial",
	};

	transport1 = {
		"Constant",
		"Sutherland",
	};

	transport2 = {
		"Constant",
	};

	transport3 = {
		"Polynomial",
	};

	transport4 = {
		"Sutherland",
	};

	transport5 = {
		"Constant Isotropic",
		"Constant Anisotropic",
	};

	//初始化comboBox_6
	if (widgetType == "fluid")
	{
		ui->comboBox_6->clear();
		for (const auto& item : equationOfState1)
		{
			ui->comboBox_6->addItem(QString::fromStdString(item));
		}
		ui->comboBox_6->setCurrentIndex(0);
		on_comboBox_6_itemActivated(0);
	}
	else if (widgetType == "solid")
	{
		ui->comboBox_6->clear();
		for (const auto& item : equationOfState2)
		{
			ui->comboBox_6->addItem(QString::fromStdString(item));
		}
		ui->comboBox_6->setCurrentIndex(0);
		on_comboBox_6_itemActivated(0);
	}

	on_spinBox_valueChanged(1);
	on_spinBox_2_valueChanged(3);
	on_spinBox_3_valueChanged(3);
	on_spinBox_4_valueChanged(3);
	on_spinBox_5_valueChanged(5);
}

void FormThermoListWidget::hideAllSubPanel()
{
	//热力学
	ui->widget_17->hide();
	ui->widget_38->hide();
	ui->widget_6->hide();
	ui->widget_9->hide();

	//传输
	ui->widget_21->hide();
	ui->widget_31->hide();
	ui->widget_34->hide();
	ui->widget_7->hide();
	ui->widget_8->hide();
	ui->widget_32->hide();

	//状态方程
	ui->widget_33->hide();
	ui->widget_10->hide();
	ui->widget_11->hide();
	ui->widget_12->hide();
	ui->widget_13->hide();
	ui->widget_14->hide();
	ui->widget_22->hide();
	ui->widget_23->hide();
}

void FormThermoListWidget::on_comboBox_6_itemActivated(int index)
{
	QString comboxEquationOfStateSelectedItem = ui->comboBox_6->currentText();

	if (widgetType == "fluid")
	{
		if (comboxEquationOfStateSelectedItem == "Perfect Gas" ||
			comboxEquationOfStateSelectedItem == "Boussinesq" ||
			comboxEquationOfStateSelectedItem == "Incompressible Perfect Gas")
		{
			ui->comboBox_7->clear();
			for (const auto& item : thermodynamics1)
			{
				ui->comboBox_7->addItem(QString::fromStdString(item));
			}
			ui->comboBox_7->setCurrentIndex(0);
			on_comboBox_7_itemActivated(0);
		}
		else if (comboxEquationOfStateSelectedItem == "Constant Density" ||
			comboxEquationOfStateSelectedItem == "Perfect Fluid" ||
			comboxEquationOfStateSelectedItem == "Adiabatic Perfect Fluid")
		{
			ui->comboBox_7->clear();
			for (const auto& item : thermodynamics2)
			{
				ui->comboBox_7->addItem(QString::fromStdString(item));
			}
			ui->comboBox_7->setCurrentIndex(0);
			on_comboBox_7_itemActivated(0);
		}
		else if (comboxEquationOfStateSelectedItem == "Polynomial")
		{
			ui->comboBox_7->clear();
			for (const auto& item : thermodynamics3)
			{
				ui->comboBox_7->addItem(QString::fromStdString(item));
			}
			ui->comboBox_7->setCurrentIndex(0);
			on_comboBox_7_itemActivated(0);
		}
		else if (comboxEquationOfStateSelectedItem == "Peng-Robinson")
		{
			ui->comboBox_7->clear();
			for (const auto& item : thermodynamics4)
			{
				ui->comboBox_7->addItem(QString::fromStdString(item));
			}
			ui->comboBox_7->setCurrentIndex(0);
			on_comboBox_7_itemActivated(0);
		}
	}
	else if (widgetType == "solid")
	{
		if (comboxEquationOfStateSelectedItem == "Constant Density")
		{
			ui->comboBox_7->clear();
			for (const auto& item : thermodynamics5)
			{
				ui->comboBox_7->addItem(QString::fromStdString(item));
			}
			ui->comboBox_7->setCurrentIndex(0);
			on_comboBox_7_itemActivated(0);
		}
	}
}

void FormThermoListWidget::on_comboBox_7_itemActivated(int index)
{
	QString comboxEquationOfStateSelectedItem = ui->comboBox_6->currentText();
	QString comboxThermodynamicsSelectedItem = ui->comboBox_7->currentText();

	if (widgetType == "fluid")
	{
		if (comboxEquationOfStateSelectedItem == "Perfect Gas" ||
			comboxEquationOfStateSelectedItem == "Boussinesq" ||
			comboxEquationOfStateSelectedItem == "Incompressible Perfect Gas")
		{
			if (comboxThermodynamicsSelectedItem == "Constant")
			{
				ui->comboBox_8->clear();
				for (const auto& item : transport1)
				{
					ui->comboBox_8->addItem(QString::fromStdString(item));
				}
				ui->comboBox_8->setCurrentIndex(0);
				on_comboBox_8_itemActivated(0);
			}
			else if (comboxThermodynamicsSelectedItem == "Janaf")
			{
				ui->comboBox_8->clear();
				for (const auto& item : transport4)
				{
					ui->comboBox_8->addItem(QString::fromStdString(item));
				}
				ui->comboBox_8->setCurrentIndex(0);
				on_comboBox_8_itemActivated(0);
			}
		}
		else if (comboxEquationOfStateSelectedItem == "Constant Density" ||
			comboxEquationOfStateSelectedItem == "Perfect Fluid" ||
			comboxEquationOfStateSelectedItem == "Adiabatic Perfect Fluid")
		{
			if (comboxThermodynamicsSelectedItem == "Constant")
			{
				ui->comboBox_8->clear();
				for (const auto& item : transport2)
				{
					ui->comboBox_8->addItem(QString::fromStdString(item));
				}
				ui->comboBox_8->setCurrentIndex(0);
				on_comboBox_8_itemActivated(0);
			}
		}
		else if (comboxEquationOfStateSelectedItem == "Polynomial")
		{
			if (comboxThermodynamicsSelectedItem == "Polynomial")
			{
				ui->comboBox_8->clear();
				for (const auto& item : transport3)
				{
					ui->comboBox_8->addItem(QString::fromStdString(item));
				}
				ui->comboBox_8->setCurrentIndex(0);
				on_comboBox_8_itemActivated(0);
			}
		}
		else if (comboxEquationOfStateSelectedItem == "Peng-Robinson")
		{
			if (comboxThermodynamicsSelectedItem == "Constant")
			{
				ui->comboBox_8->clear();
				for (const auto& item : transport4)
				{
					ui->comboBox_8->addItem(QString::fromStdString(item));
				}
				ui->comboBox_8->setCurrentIndex(0);
				on_comboBox_8_itemActivated(0);
			}
			else if (comboxThermodynamicsSelectedItem == "Polynomial")
			{
				ui->comboBox_8->clear();
				for (const auto& item : transport3)
				{
					ui->comboBox_8->addItem(QString::fromStdString(item));
				}
				ui->comboBox_8->setCurrentIndex(0);
				on_comboBox_8_itemActivated(0);
			}
			else if (comboxThermodynamicsSelectedItem == "Janaf")
			{
				ui->comboBox_8->clear();
				for (const auto& item : transport3)
				{
					ui->comboBox_8->addItem(QString::fromStdString(item));
				}
				ui->comboBox_8->setCurrentIndex(0);
				on_comboBox_8_itemActivated(0);
			}
		}
	}
	else if (widgetType == "solid")
	{
		if (comboxEquationOfStateSelectedItem == "Constant Density")
		{
			if (comboxThermodynamicsSelectedItem == "Constant")
			{
				ui->comboBox_8->clear();
				for (const auto& item : transport5)
				{
					ui->comboBox_8->addItem(QString::fromStdString(item));
				}
				ui->comboBox_8->setCurrentIndex(0);
				on_comboBox_8_itemActivated(0);
			}
			else if (comboxThermodynamicsSelectedItem == "Polynomial")
			{
				ui->comboBox_8->clear();
				for (const auto& item : transport3)
				{
					ui->comboBox_8->addItem(QString::fromStdString(item));
				}
				ui->comboBox_8->setCurrentIndex(0);
				on_comboBox_8_itemActivated(0);
			}
		}
	}
}

void FormThermoListWidget::on_comboBox_8_itemActivated(int index)
{
	QString comboxEquationOfStateSelectedItem = ui->comboBox_6->currentText();
	QString comboxThermodynamicsSelectedItem = ui->comboBox_7->currentText();
	QString comboxTransportSelectedItem = ui->comboBox_8->currentText();

	if (widgetType == "fluid")
	{
		if (comboxEquationOfStateSelectedItem == "Perfect Gas" )
		{
			if (comboxThermodynamicsSelectedItem == "Constant")
			{
				if (comboxTransportSelectedItem == "Constant")
				{
					hideAllSubPanel();
					ui->widget_6->show();
					ui->widget_8->show();
				}
				else if (comboxTransportSelectedItem == "Sutherland")
				{
					hideAllSubPanel();
					ui->widget_6->show();
					ui->widget_7->show();
				}
			}
			else if (comboxThermodynamicsSelectedItem == "Janaf")
			{
				if (comboxTransportSelectedItem == "Sutherland")
				{
					hideAllSubPanel();
					ui->widget_9->show();
					ui->widget_7->show();
				}
			}
		}
		else if (comboxEquationOfStateSelectedItem == "Boussinesq")
		{
			if (comboxThermodynamicsSelectedItem == "Constant")
			{
				if (comboxTransportSelectedItem == "Constant")
				{
					hideAllSubPanel();
					ui->widget_6->show();
					ui->widget_8->show();
					ui->widget_33->show();
					ui->widget_10->show();
				}
				else if (comboxTransportSelectedItem == "Sutherland")
				{
					hideAllSubPanel();
					ui->widget_6->show();
					ui->widget_7->show();
					ui->widget_33->show();
					ui->widget_10->show();
				}
			}
			else if (comboxThermodynamicsSelectedItem == "Janaf")
			{
				if (comboxTransportSelectedItem == "Sutherland")
				{
					hideAllSubPanel();
					ui->widget_9->show();
					ui->widget_7->show();
					ui->widget_33->show();
					ui->widget_10->show();
				}
			}
		}
		else if (comboxEquationOfStateSelectedItem == "Incompressible Perfect Gas")
		{
			if (comboxThermodynamicsSelectedItem == "Constant")
			{
				if (comboxTransportSelectedItem == "Constant")
				{
					hideAllSubPanel();
					ui->widget_6->show();
					ui->widget_8->show();
					ui->widget_33->show();
					ui->widget_11->show();
				}
				else if (comboxTransportSelectedItem == "Sutherland")
				{
					hideAllSubPanel();
					ui->widget_6->show();
					ui->widget_7->show();
					ui->widget_33->show();
					ui->widget_11->show();
				}
			}
			else if (comboxThermodynamicsSelectedItem == "Janaf")
			{
				if (comboxTransportSelectedItem == "Sutherland")
				{
					hideAllSubPanel();
					ui->widget_9->show();
					ui->widget_7->show();
					ui->widget_33->show();
					ui->widget_11->show();
				}
			}
		}
		else if (comboxEquationOfStateSelectedItem == "Constant Density")
		{
			if (comboxThermodynamicsSelectedItem == "Constant")
			{
				if (comboxTransportSelectedItem == "Constant")
				{
					hideAllSubPanel();
					ui->widget_6->show();
					ui->widget_8->show();
					ui->widget_33->show();
					ui->widget_12->show();
				}
			}
		}
		else if (comboxEquationOfStateSelectedItem == "Perfect Fluid")
		{
			if (comboxThermodynamicsSelectedItem == "Constant")
			{
				if (comboxTransportSelectedItem == "Constant")
				{
					hideAllSubPanel();
					ui->widget_6->show();
					ui->widget_8->show();
					ui->widget_33->show();
					ui->widget_13->show();
				}
			}
		}
		else if (comboxEquationOfStateSelectedItem == "Adiabatic Perfect Fluid")
		{
			if (comboxThermodynamicsSelectedItem == "Constant")
			{
				if (comboxTransportSelectedItem == "Constant")
				{
					hideAllSubPanel();
					ui->widget_6->show();
					ui->widget_8->show();
					ui->widget_33->show();
					ui->widget_14->show();
				}
			}
		}
		else if (comboxEquationOfStateSelectedItem == "Polynomial")
		{
			if (comboxThermodynamicsSelectedItem == "Polynomial")
			{
				if (comboxTransportSelectedItem == "Polynomial")
				{
					hideAllSubPanel();
					ui->widget_17->show();
					ui->widget_21->show();
					ui->widget_33->show();
					ui->widget_22->show();
				}
			}
		}
		else if (comboxEquationOfStateSelectedItem == "Peng-Robinson")
		{
			if (comboxThermodynamicsSelectedItem == "Constant")
			{
				if (comboxTransportSelectedItem == "Sutherland")
				{
					hideAllSubPanel();
					ui->widget_6->show();
					ui->widget_8->show();
					ui->widget_33->show();
					ui->widget_23->show();
				}
			}
			else if(comboxThermodynamicsSelectedItem == "Polynomial")
			{
				if (comboxTransportSelectedItem == "Polynomial")
				{
					hideAllSubPanel();
					ui->widget_17->show();
					ui->widget_21->show();
					ui->widget_33->show();
					ui->widget_23->show();
				}
			}
			else if (comboxThermodynamicsSelectedItem == "Janaf")
			{
				if (comboxTransportSelectedItem == "Polynomial")
				{
					hideAllSubPanel();
					ui->widget_9->show();
					ui->widget_21->show();
					ui->widget_33->show();
					ui->widget_23->show();
				}
			}
		}
	}
	else if (widgetType == "solid")
	{
		if (comboxEquationOfStateSelectedItem == "Constant Density")
		{
			if (comboxThermodynamicsSelectedItem == "Constant")
			{
				if (comboxTransportSelectedItem == "Constant Isotropic")
				{
					hideAllSubPanel();
					ui->widget_6->show();
					ui->widget_31->show();
					ui->widget_33->show();
					ui->widget_12->show();
				}
				else if (comboxTransportSelectedItem == "Constant Anisotropic")
				{
					hideAllSubPanel();
					ui->widget_6->show();
					ui->widget_32->show();
					ui->widget_33->show();
					ui->widget_12->show();
				}
			}
			else if (comboxThermodynamicsSelectedItem == "Polynomial")
			{
				if (comboxTransportSelectedItem == "Polynomial")
				{
					hideAllSubPanel();
					ui->widget_17->show();
					ui->widget_34->show();
					ui->widget_33->show();
					ui->widget_12->show();
				}
			}
		}
	}
}

void FormThermoListWidget::on_spinBox_valueChanged(int value)
{
	std::vector<QLineEdit*> lineEdits = {
		ui->lineEdit_39,
		ui->lineEdit_40,
		ui->lineEdit_41,
		ui->lineEdit_42,
		ui->lineEdit_44,
		ui->lineEdit_46,
		ui->lineEdit_45,
		ui->lineEdit_43,
	};

	for (int i = 0; i < lineEdits.size(); ++i)
	{
		if (i < value)
		{
			lineEdits[i]->setEnabled(true);
		}
		else
		{
			lineEdits[i]->setEnabled(false);
		}
	}
}

void FormThermoListWidget::on_spinBox_2_valueChanged(int value)
{
	std::vector<QLineEdit*> lineEdits = {
		ui->lineEdit_47,
		ui->lineEdit_48,
		ui->lineEdit_49,
		ui->lineEdit_50,
		ui->lineEdit_51,
		ui->lineEdit_52,
		ui->lineEdit_53,
		ui->lineEdit_54,
	};

	for (int i = 0; i < lineEdits.size(); ++i)
	{
		if (i < value)
		{
			lineEdits[i]->setEnabled(true);
		}
		else
		{
			lineEdits[i]->setEnabled(false);
		}
	}
}

void FormThermoListWidget::on_spinBox_3_valueChanged(int value)
{
	std::vector<QLineEdit*> lineEdits = {
		ui->lineEdit_55,
		ui->lineEdit_56,
		ui->lineEdit_57,
		ui->lineEdit_58,
		ui->lineEdit_59,
		ui->lineEdit_60,
		ui->lineEdit_61,
		ui->lineEdit_62,
	};

	for (int i = 0; i < lineEdits.size(); ++i)
	{
		if (i < value)
		{
			lineEdits[i]->setEnabled(true);
		}
		else
		{
			lineEdits[i]->setEnabled(false);
		}
	}
}

void FormThermoListWidget::on_spinBox_4_valueChanged(int value)
{
	std::vector<QLineEdit*> lineEdits = {
		ui->lineEdit_80,
		ui->lineEdit_81,
		ui->lineEdit_82,
		ui->lineEdit_83,
		ui->lineEdit_84,
		ui->lineEdit_85,
		ui->lineEdit_86,
		ui->lineEdit_87,
	};

	for (int i = 0; i < lineEdits.size(); ++i)
	{
		if (i < value)
		{
			lineEdits[i]->setEnabled(true);
		}
		else
		{
			lineEdits[i]->setEnabled(false);
		}
	}
}

void FormThermoListWidget::on_spinBox_5_valueChanged(int value)
{
	std::vector<QLineEdit*> lineEdits = {
		ui->lineEdit_71,
		ui->lineEdit_72,
		ui->lineEdit_73,
		ui->lineEdit_74,
		ui->lineEdit_75,
		ui->lineEdit_76,
		ui->lineEdit_77,
		ui->lineEdit_78,
	};
	for (int i = 0; i < lineEdits.size(); ++i)
	{
		if (i < value)
		{
			lineEdits[i]->setEnabled(true);
		}
		else
		{
			lineEdits[i]->setEnabled(false);
		}
	}
}
