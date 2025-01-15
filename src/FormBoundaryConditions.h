#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormBoundaryConditions.h"
#include "GlobalData.h"
#include <QStandardItemModel>
#include <QMessageBox.h>
#include "FormBoundaryConditionsTabWidget.h"
#include <vector>
#include <QFileInfo>

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
	void importParameter();
	void exportParameter();
	void initBoundaryConditions();

	void parseBoundaryFile(const QString& filePath, const QString& fieldName);

private slots:
    void onListViewItemClicked(const QModelIndex& index);

private:
	Ui::FormBoundaryConditionsClass *ui;
	QStandardItemModel* listViewModel;
	QMap<QString, QMap<QString, QVector<QString>>> boundaryConditions;
};
