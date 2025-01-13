#include "FormBoundaryConditionsTabWidget.h"

FormBoundaryConditionsTabWidget::FormBoundaryConditionsTabWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormBoundaryConditionsTabWidgetClass())
{
	ui->setupUi(this);

	QRegularExpression regExp("^-?\\d*\\.?\\d+([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regExp, this);
	ui->lineEdit->setValidator(validator);
	ui->lineEdit_2->setValidator(validator);
	ui->lineEdit_3->setValidator(validator);
	ui->lineEdit_4->setValidator(validator);
	ui->lineEdit_5->setValidator(validator);
	ui->lineEdit_6->setValidator(validator);
	ui->lineEdit_7->setValidator(validator);

	connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxCurrentIndexChanged(int)));
	connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_2CurrentIndexChanged(int)));
	connect(ui->comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_3CurrentIndexChanged(int)));
	connect(ui->comboBox_4, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_4CurrentIndexChanged(int)));
	connect(ui->comboBox_5, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_5CurrentIndexChanged(int)));
	connect(ui->comboBox_6, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_6CurrentIndexChanged(int)));
	connect(ui->comboBox_7, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_7CurrentIndexChanged(int)));

	onComboBoxCurrentIndexChanged(0);
	onComboBox_2CurrentIndexChanged(0);
	onComboBox_3CurrentIndexChanged(0);
	onComboBox_4CurrentIndexChanged(0);
	onComboBox_5CurrentIndexChanged(0);
	onComboBox_6CurrentIndexChanged(0);
	onComboBox_7CurrentIndexChanged(0);
}

FormBoundaryConditionsTabWidget::~FormBoundaryConditionsTabWidget()
{
	delete ui;
}

void FormBoundaryConditionsTabWidget::onComboBoxCurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox->currentText();
	if (boundaryConditionType == "zeroGradient") {
		for (int i = 0; i < ui->horizontalLayout->count(); ++i) {
			QWidget* widget = ui->horizontalLayout->itemAt(i)->widget();
			if (widget) {
				widget->hide();
			}
		}
		ui->verticalSpacer->sizeHint().setHeight(0);
	}
	else if (boundaryConditionType == "fixedValue") {
		for (int i = 0; i < ui->horizontalLayout->count(); ++i) {
			QWidget* widget = ui->horizontalLayout->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer->sizeHint().setHeight(10);
	}
}

void FormBoundaryConditionsTabWidget::onComboBox_2CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_2->currentText();
	if (boundaryConditionType == "zeroGradient") {
		for (int i = 0; i < ui->horizontalLayout_6->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_6->itemAt(i)->widget();
			if (widget) {
				widget->hide();
			}
		}
		ui->verticalSpacer_2->sizeHint().setHeight(0);
	}
	else if (boundaryConditionType == "fixedValue") {
		for (int i = 0; i < ui->horizontalLayout_6->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_6->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer_2->sizeHint().setHeight(10);
	}
	else if (boundaryConditionType == "heatFluxTemperature") {
		for (int i = 0; i < ui->horizontalLayout_6->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_6->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer_2->sizeHint().setHeight(10);
	}
}

void FormBoundaryConditionsTabWidget::onComboBox_3CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_3->currentText();
	if (boundaryConditionType == "zeroGradient") {
		for (int i = 0; i < ui->horizontalLayout_9->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_9->itemAt(i)->widget();
			if (widget) {
				widget->hide();
			}
		}
		ui->verticalSpacer_3->sizeHint().setHeight(0);
	}
	else if (boundaryConditionType == "fixedValue") {
		for (int i = 0; i < ui->horizontalLayout_9->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_9->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer_3->sizeHint().setHeight(10);
	}
	else if (boundaryConditionType == "noSlip") {
		for (int i = 0; i < ui->horizontalLayout_9->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_9->itemAt(i)->widget();
			if (widget) {
				widget->hide();
			}
		}
		ui->verticalSpacer_3->sizeHint().setHeight(0);
	}
}

void FormBoundaryConditionsTabWidget::onComboBox_4CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_4->currentText();
	if (boundaryConditionType == "fixedValue") {
		for (int i = 0; i < ui->horizontalLayout_12->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_12->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer_4->sizeHint().setHeight(10);
	}
	else if (boundaryConditionType == "inletOutlet") {
		for (int i = 0; i < ui->horizontalLayout_12->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_12->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer_4->sizeHint().setHeight(0);
	}
	else if (boundaryConditionType == "kqRWallFunction") {
		for (int i = 0; i < ui->horizontalLayout_12->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_12->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer_4->sizeHint().setHeight(0);
	}
}

void FormBoundaryConditionsTabWidget::onComboBox_5CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_5->currentText();
	if (boundaryConditionType == "calculated") {
		for (int i = 0; i < ui->horizontalLayout_15->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_15->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer_5->sizeHint().setHeight(10);
	}
	else if (boundaryConditionType == "nutkWallFunction") {
		for (int i = 0; i < ui->horizontalLayout_15->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_15->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer_5->sizeHint().setHeight(10);
	}
}

void FormBoundaryConditionsTabWidget::onComboBox_6CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_6->currentText();
	if (boundaryConditionType == "fixedValue") {
		for (int i = 0; i < ui->horizontalLayout_18->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_18->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer_6->sizeHint().setHeight(10);
	}
	else if (boundaryConditionType == "inletOutlet") {
		for (int i = 0; i < ui->horizontalLayout_18->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_18->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer_6->sizeHint().setHeight(0);
	}
	else if (boundaryConditionType == "omegaWallFunction") {
		for (int i = 0; i < ui->horizontalLayout_18->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_18->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer_6->sizeHint().setHeight(0);
	}
}

void FormBoundaryConditionsTabWidget::onComboBox_7CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_7->currentText();
	if (boundaryConditionType == "calculated") {
		for (int i = 0; i < ui->horizontalLayout_21->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_21->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer_7->sizeHint().setHeight(10);
	}
	else if (boundaryConditionType == "alphatWallFunction") {
		for (int i = 0; i < ui->horizontalLayout_21->count(); ++i) {
			QWidget* widget = ui->horizontalLayout_21->itemAt(i)->widget();
			if (widget) {
				widget->show();
			}
		}
		ui->verticalSpacer_7->sizeHint().setHeight(0);
	}
}


