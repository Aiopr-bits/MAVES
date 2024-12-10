#pragma once

#include <QWidget>
#include "ui_FormMesh.h"
#include "GlobalData.h"

#pragma execution_character_set("utf-8")

QT_BEGIN_NAMESPACE
namespace Ui { class FormMeshClass; };
QT_END_NAMESPACE

class FormMesh : public QWidget
{
	Q_OBJECT

public:
	FormMesh(QWidget *parent = nullptr);
	~FormMesh();

	void updateForm();

private:
	Ui::FormMeshClass *ui;
};
