#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormCellZonesTabWidget.h"
#include <QTabBar>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class FormCellZonesTabWidgetClass; };
QT_END_NAMESPACE

class FormCellZonesTabWidget : public QWidget
{
	Q_OBJECT

public:
	FormCellZonesTabWidget(const QString& solver = "rhoSimpleFoam", const QString& regionType = "fluid", const QString& turbulenceType = "Laminar", QWidget *parent = nullptr);
	~FormCellZonesTabWidget();

public slots:
	void resizeEvent(QResizeEvent* event);
	void on_RadioButton_Toggled(bool checked);
	void on_comboBox_20_itemActivated(int index);

public:
	Ui::FormCellZonesTabWidgetClass *ui;
};
