/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#include "FormMeshImport.h"
#include <QFileDialog>

FormMeshImport::FormMeshImport(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::FormMeshImportClass())
{
	ui->setupUi(this);
	ui->pushButton->setEnabled(false);	

	connect(ui->lineEdit, &QLineEdit::textChanged, this, &FormMeshImport::on_LineEditText_Changed);
}

FormMeshImport::~FormMeshImport()
{
	delete ui;
}

void FormMeshImport::on_pushButton_clicked()
{
	QString filePath = ui->lineEdit->text();
	emit meshImported(filePath);
	ui->lineEdit->clear();
}

void FormMeshImport::on_pushButton_2_clicked()
{
	QString text = ui->comboBox->currentText();
	QString filePath;

	if (text == "OpenFOAM 文件") {
		filePath = QFileDialog::getOpenFileName(this, tr("打开文件"), "",
			tr("OpenFOAM 文件 (*.foam);;"));
	}
	else {
		return;
	}

	ui->lineEdit->setText(filePath);
}

void FormMeshImport::on_LineEditText_Changed(const QString& text)
{
	ui->pushButton->setEnabled(!text.isEmpty());
}
