#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormMesh.h"
#include "GlobalData.h"
#include <QStandardItemModel.h>
#include "CustomCheckBoxDelegate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormMeshClass; };
QT_END_NAMESPACE

class FormMesh : public QWidget
{
	Q_OBJECT

public:
	FormMesh(QWidget *parent = nullptr);
	~FormMesh();

public slots:
	void updateForm();
	void on_pushButton_clicked();

signals:
	void meshVisibilityChanged();

public:
	Ui::FormMeshClass *ui;
	QStandardItemModel* listViewModel;
};
