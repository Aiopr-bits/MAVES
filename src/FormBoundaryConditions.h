#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormBoundaryConditions.h"
#include "GlobalData.h"
#include <QStandardItemModel>
#include <QMessageBox.h>
#include "FormBoundaryConditionsTabWidget.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class FormBoundaryConditionsClass; };
QT_END_NAMESPACE

class FormBoundaryConditions : public QWidget
{
	Q_OBJECT

public:
	FormBoundaryConditions(QWidget *parent = nullptr);
	~FormBoundaryConditions();
	void onMeshImported();
	void initListView();
	void initTabWidget();

private slots:
    void onListViewItemClicked(const QModelIndex& index);

private:
	vector<FormBoundaryConditionsTabWidget*> tabWidgetList;
	Ui::FormBoundaryConditionsClass *ui;
	QStandardItemModel* listViewModel;
};
