/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormBoundaryConditionsTabWidget.h"
#include <QMessageBox>
#include <QPropertyAnimation.h>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class FormBoundaryConditionsTabWidgetClass; };
QT_END_NAMESPACE

class FormBoundaryConditionsTabWidget : public QWidget
{
	Q_OBJECT

public:
	FormBoundaryConditionsTabWidget(
		QWidget* parent = nullptr, 
		const QString& solver = "rhoSimpleFoam",
		const QString& turbulenceType = "Laminar",
		const QString& boundaryType = "Patch",
		const QString& boundaryConditionType = "Pressure Inlet");

	~FormBoundaryConditionsTabWidget();

public slots:
	void initialization(
		const QString& solver = "rhoSimpleFoam",
		const QString& turbulenceType = "Laminar", 
		const QString& boundaryType = "Patch",
		const QString& boundaryConditionType = "Pressure Inlet");

	void on_comboBox_14_itemActivated(int index);
	void on_comboBox_16_itemActivated(int index);
	void on_comboBox_26_itemActivated(int index);
	void on_comboBox_43_itemActivated(int index);

	void resizeEvent(QResizeEvent* event);

public:
	Ui::FormBoundaryConditionsTabWidgetClass* ui;
};
