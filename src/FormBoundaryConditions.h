/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

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
#include <QModelIndex>
#include "CustomItemWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormBoundaryConditionsClass; };
QT_END_NAMESPACE

class FormBoundaryConditions : public QWidget
{
	Q_OBJECT

public:
	FormBoundaryConditions(QWidget *parent = nullptr);
	~FormBoundaryConditions();

public slots:
	void initialization(const QString& solver);
	void on_textChanged(CustomItemWidget* widget, QString previousText);				//文本改变
	void on_typeChanged(CustomItemWidget* widget, int previousIndex);					//类型改变

signals:

private:
	Ui::FormBoundaryConditionsClass* ui;



	//以下可能删除
public:
	void onMeshImported();
	void initListView();
	void initSubWidget();
	void importParameter();
	void exportParameter();
	void initBoundaryConditions();
	void parseBoundaryFile(const QString& filePath, const QString& fieldName);

private slots:
    void onListViewItemClicked(const QModelIndex& index);
	void onToggleRegionSecondAnimation();

signals:
	void toggleRegionSecondAnimation();

private:
	//QStandardItemModel* listViewModel;
	////QMap<QString, QMap<QString, QVector<QString>>> boundaryConditions;
	//std::vector<QString> patchName;
	//std::vector<FormBoundaryConditionsTabWidget*> tabWidgetsGroup;
};
