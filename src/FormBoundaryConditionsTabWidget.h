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
	FormBoundaryConditionsTabWidget(QWidget *parent = nullptr);
	~FormBoundaryConditionsTabWidget();
	void setLayoutVisibility(QLayout* layout, bool visible);
	void updateLayoutVisibility(QGridLayout* layout, bool visible);

public slots:
	void onComboBoxCurrentIndexChanged(int index);
	void onComboBox_2CurrentIndexChanged(int index);
	void onComboBox_3CurrentIndexChanged(int index);
	void onComboBox_4CurrentIndexChanged(int index);
	void onComboBox_5CurrentIndexChanged(int index);
	void onComboBox_6CurrentIndexChanged(int index);
	void onComboBox_7CurrentIndexChanged(int index);
	void resizeEvent(QResizeEvent* event);
	void on_tabWidget_currentChanged(int index);

public:
	Ui::FormBoundaryConditionsTabWidgetClass *ui;
	int previousIndex;
};
