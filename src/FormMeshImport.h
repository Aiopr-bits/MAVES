#pragma once

#include <QWidget>
#include "ui_FormMeshImport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormMeshImportClass; };
QT_END_NAMESPACE

class FormMeshImport : public QWidget
{
	Q_OBJECT

public:
	FormMeshImport(QWidget *parent = nullptr);
	~FormMeshImport();

private:
	Ui::FormMeshImportClass *ui;
};
