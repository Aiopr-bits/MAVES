#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormBoundaryConditionsTabWidget.h"
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class FormBoundaryConditionsTabWidgetClass; };
QT_END_NAMESPACE

class FormBoundaryConditionsTabWidget : public QWidget
{
	Q_OBJECT

public:
	FormBoundaryConditionsTabWidget(QWidget *parent = nullptr);
	~FormBoundaryConditionsTabWidget();

public slots:
	void onComboBoxCurrentIndexChanged(int index);
	void onComboBox_2CurrentIndexChanged(int index);
	void onComboBox_3CurrentIndexChanged(int index);
	void onComboBox_4CurrentIndexChanged(int index);
	void onComboBox_5CurrentIndexChanged(int index);
	void onComboBox_6CurrentIndexChanged(int index);
	void onComboBox_7CurrentIndexChanged(int index);

private:
	Ui::FormBoundaryConditionsTabWidgetClass *ui;
};