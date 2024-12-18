#include "FormGeometry.h"
#include <QFileDialog>

FormGeometry::FormGeometry(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormGeometryClass())
{
	ui->setupUi(this);
	ui->pushButton_2->setEnabled(false);
	connect(ui->lineEdit, &QLineEdit::textChanged, this, &FormGeometry::on_LineEditText_Changed);

}

FormGeometry::~FormGeometry()
{
	delete ui;
}

void FormGeometry::on_pushButton_clicked()
{
	QString text = ui->comboBox->currentText();
	QString filePath;
	
	if (text == "STP �ļ�") {
		filePath = QFileDialog::getOpenFileName(this, tr("���ļ�"), "",
			tr("STP �ļ� (*.stp *.step);;"));
	}
	else if (text == "IGES �ļ�") {
		filePath = QFileDialog::getOpenFileName(this, tr("���ļ�"), "",
			tr("IGES �ļ� (*.igs *.iges);;"));
	}
	else if (text == "BREP �ļ�") {
		filePath = QFileDialog::getOpenFileName(this, tr("���ļ�"), "",
			tr("BREP �ļ� (*.brep);;"));
	}
	else {
		return;
	}

	ui->lineEdit->setText(filePath);
}

void FormGeometry::on_pushButton_2_clicked()
{
	QString filePath = ui->lineEdit->text();
	emit geometryImported(filePath);
	ui->lineEdit->clear();
}

void FormGeometry::on_LineEditText_Changed(const QString& text)
{
	ui->pushButton_2->setEnabled(!text.isEmpty());
}
