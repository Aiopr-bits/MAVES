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

	void on_comboBox_14_itemActivated(int index);						//p
	void on_comboBox_16_itemActivated(int index);						//p-ρgh
	void on_comboBox_26_itemActivated(int index);						//U
	void on_comboBox_43_itemActivated(int index);						//T
	void on_comboBox_50_itemActivated(int index);						//k
	void on_comboBox_56_itemActivated(int index);						//k_t
	void on_comboBox_62_itemActivated(int index);						//k_l
	void on_comboBox_68_itemActivated(int index);						//ε
	void on_comboBox_74_itemActivated(int index);						//v_SA
	void on_comboBox_75_itemActivated(int index);						//ω
	void on_comboBox_81_itemActivated(int index);						//v_t
	void on_comboBox_82_itemActivated(int index);						//α_t
	void resizeEvent(QResizeEvent* event);
	void toggleWidgets(QWidget* w1, QWidget* w2, bool showFirst);
	void on_anyComboBox_itemActivated(int index);

public:
	Ui::FormBoundaryConditionsTabWidgetClass* ui;
};
