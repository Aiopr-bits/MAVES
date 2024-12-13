#include "FormMeshImport.h"

FormMeshImport::FormMeshImport(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormMeshImportClass())
{
	ui->setupUi(this);
}

FormMeshImport::~FormMeshImport()
{
	delete ui;
}
