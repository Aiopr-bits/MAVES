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
	
	if (text == "STP 文件") {
		filePath = QFileDialog::getOpenFileName(this, tr("打开文件"), "",
			tr("STP 文件 (*.stp *.step);;"));
	}
	else if (text == "IGES 文件") {
		filePath = QFileDialog::getOpenFileName(this, tr("打开文件"), "",
			tr("IGES 文件 (*.igs *.iges);;"));
	}
	else if (text == "BREP 文件") {
		filePath = QFileDialog::getOpenFileName(this, tr("打开文件"), "",
			tr("BREP 文件 (*.brep);;"));
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
