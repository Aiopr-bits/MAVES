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

	if (text == "OpenFOAM �ļ�") {
		filePath = QFileDialog::getOpenFileName(this, tr("���ļ�"), "",
			tr("OpenFOAM �ļ� (*.foam);;"));
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
