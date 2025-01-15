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
	ui->lineEdit_4->setValidator(validator);
	ui->lineEdit_5->setValidator(validator);
	ui->lineEdit_6->setValidator(validator);
	ui->lineEdit_7->setValidator(validator);
	ui->lineEdit_8->setValidator(validator);
	ui->lineEdit_9->setValidator(validator);
	ui->lineEdit_10->setValidator(validator);

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

void FormBoundaryConditionsTabWidget::setLayoutVisibility(QLayout* layout, bool visible)
{
	for (int i = 0; i < layout->count(); ++i) {
		QLayoutItem* item = layout->itemAt(i);
		if (item != nullptr) {
			QWidget* widget = item->widget();
			if (widget != nullptr) {
				widget->setVisible(visible);
			}
			else {
				QLayout* subLayout = item->layout();
				if (subLayout != nullptr) {
					setLayoutVisibility(subLayout, visible);
				}
			}
		}
	}
}

void FormBoundaryConditionsTabWidget::updateLayoutVisibility(QGridLayout* layout, bool visible)
{
	for (int i = 0; i < layout->columnCount(); ++i) {
		QLayoutItem* item = layout->itemAtPosition(1, i);
		if (item != nullptr) {
			QWidget* widget = item->widget();
			if (widget != nullptr) {
				widget->setVisible(visible);
			}
			else {
				QLayout* subLayout = item->layout();
				if (subLayout != nullptr) {
					setLayoutVisibility(subLayout, visible);
				}
			}
		}
	}
}

void FormBoundaryConditionsTabWidget::onComboBoxCurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox->currentText();
	updateLayoutVisibility(ui->gridLayout, boundaryConditionType == "fixedValue");
}

void FormBoundaryConditionsTabWidget::onComboBox_2CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_2->currentText();
	bool visible = (boundaryConditionType == "fixedValue" || boundaryConditionType == "heatFluxTemperature");
	updateLayoutVisibility(ui->gridLayout_2, visible);
}

void FormBoundaryConditionsTabWidget::onComboBox_3CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_3->currentText();
	bool visible = (boundaryConditionType == "fixedValue");
	updateLayoutVisibility(ui->gridLayout_3, visible);
}

void FormBoundaryConditionsTabWidget::onComboBox_4CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_4->currentText();
	bool visible = (boundaryConditionType == "fixedValue" || boundaryConditionType == "inletOutlet" || boundaryConditionType == "kqRWallFunction");
	updateLayoutVisibility(ui->gridLayout_4, visible);
}

void FormBoundaryConditionsTabWidget::onComboBox_5CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_5->currentText();
	bool visible = (boundaryConditionType == "calculated" || boundaryConditionType == "nutkWallFunction");
	updateLayoutVisibility(ui->gridLayout_5, visible);
}

void FormBoundaryConditionsTabWidget::onComboBox_6CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_6->currentText();
	bool visible = (boundaryConditionType == "fixedValue" || boundaryConditionType == "inletOutlet" || boundaryConditionType == "omegaWallFunction");
	updateLayoutVisibility(ui->gridLayout_6, visible);
}

void FormBoundaryConditionsTabWidget::onComboBox_7CurrentIndexChanged(int index)
{
	QString boundaryConditionType = ui->comboBox_7->currentText();
	bool visible = (boundaryConditionType == "calculated" || boundaryConditionType == "alphatWallFunction");
	updateLayoutVisibility(ui->gridLayout_7, visible);
}
