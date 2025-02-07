#include "FormTurbulence.h"
#include <QRegularExpression>

FormTurbulence::FormTurbulence(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormTurbulenceClass())
{
	ui->setupUi(this);
	ui->gridLayout->setRowStretch(0, 1);
	ui->gridLayout->setRowStretch(3, 1);
	ui->gridLayout->setColumnStretch(0, 1);
	ui->gridLayout->setColumnStretch(2, 1);

	QRegularExpression regExp("^-?\\d*\\.?\\d+([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regExp, this);
	ui->lineEdit->setValidator(validator);

	connect(ui->comboBox, &QComboBox::currentTextChanged, this, &FormTurbulence::onComboBoxTextChanged);

	onComboBoxTextChanged("RAS");
}

FormTurbulence::~FormTurbulence()
{
	delete ui;
}

void FormTurbulence::onComboBoxTextChanged(const QString& text)
{
	if (text == "RAS") {
		ui->tabWidget->show();
		for (int i = 0; i < ui->tabWidget->count(); ++i) {
			if (ui->tabWidget->tabText(i) == "RAS参数") {
				ui->tabWidget->setTabVisible(i, true);
				ui->tabWidget->setCurrentIndex(i);
			}
			else {
				ui->tabWidget->setTabVisible(i, false);
			}
		}
	}
	else if (text == "LES") {
		ui->tabWidget->show();
		for (int i = 0; i < ui->tabWidget->count(); ++i) {
			if (ui->tabWidget->tabText(i) == "LES参数") {
				ui->tabWidget->setTabVisible(i, true);
				ui->tabWidget->setCurrentIndex(i);
			}
			else {
				ui->tabWidget->setTabVisible(i, false);
			}
		}
	}
	else if (text == "laminar") {
		ui->tabWidget->hide();
	}
}
