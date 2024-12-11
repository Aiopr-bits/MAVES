#pragma once

#include <QWidget>
#include "ui_FormMesh.h"
#include "GlobalData.h"
#include <QStandardItemModel.h>

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

	void on_treeView_itemClicked(const QModelIndex& index);

public slots:
	void updateForm();
	void on_pushButton_clicked();

signals:
	void meshVisibilityChanged();

private:
	Ui::FormMeshClass *ui;
	QStandardItemModel* treeViewModel;
};
