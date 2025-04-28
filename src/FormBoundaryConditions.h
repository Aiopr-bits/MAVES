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
	void initialization(const QString& solver = "rhoSimpleFoam", const QString& turbulenceType = "Laminar");
	void on_ListWidgetItem_Clicked(QListWidgetItem* item);
	void on_textChanged(CustomItemWidget* widget, QString previousText);				//文本改变
	void on_typeChanged(CustomItemWidget* widget, int previousIndex);					//类型改变

private:
	Ui::FormBoundaryConditionsClass* ui;
	std::vector<FormBoundaryConditionsTabWidget*> formBoundaryConditionsTabWidgetsList;
};
