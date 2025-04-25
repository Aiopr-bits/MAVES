#include "FormCellZonesTabWidget.h"
#include <QRegularExpressionValidator>

FormCellZonesTabWidget::FormCellZonesTabWidget(const QString& solver, const QString& regionType, const QString& turbulenceType, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormCellZonesTabWidgetClass())
{
	ui->setupUi(this);

	ui->tabWidget->setTabVisible(0, false);
	ui->tabWidget->setTabVisible(1, false);
	ui->tabWidget->setTabVisible(2, false);
	ui->tabWidget->setTabVisible(3, false);

	connect(ui->radioButton_12, &QRadioButton::toggled, this, &FormCellZonesTabWidget::on_RadioButton_Toggled);
	connect(ui->radioButton_13, &QRadioButton::toggled, this, &FormCellZonesTabWidget::on_RadioButton_Toggled);
	connect(ui->comboBox_20, QOverload<int>::of(&QComboBox::activated), this, &FormCellZonesTabWidget::on_comboBox_20_itemActivated);

	ui->radioButton_12->setChecked(true);
	on_comboBox_20_itemActivated(0);

	// 输入限制
	QRegularExpression regex("^-?(\\d+(\\.\\d*)?|\\.\\d+)([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, this);
	const auto lineEdits = this->findChildren<QLineEdit*>();
	for (QLineEdit* lineEdit : lineEdits) {
		lineEdit->setValidator(validator);
	}
}

FormCellZonesTabWidget::~FormCellZonesTabWidget()
{
	delete ui;
}

void FormCellZonesTabWidget::on_comboBox_20_itemActivated(int index)
{
	ui->lineEdit_26->hide();
	ui->pushButton_5->hide();

	if (index == 0) {
		ui->lineEdit_26->show(); 
	}
	else if (index == 1) {
		ui->pushButton_5->show();
	}
}

void FormCellZonesTabWidget::on_RadioButton_Toggled(bool checked)
{
	QRadioButton* senderButton = qobject_cast<QRadioButton*>(sender());
	if (!senderButton || !checked) return; 

	ui->widget_5->hide();
	ui->widget_6->hide();

	if (senderButton == ui->radioButton_12)
	{
		ui->widget_5->show();
	}
	else if (senderButton == ui->radioButton_13)
	{
		ui->widget_6->show();
	}
}

void FormCellZonesTabWidget::resizeEvent(QResizeEvent* event)
{
	int width = ui->tabWidget->width();
	ui->tabWidget->tabBar()->setFixedWidth(ui->tabWidget->width());
}