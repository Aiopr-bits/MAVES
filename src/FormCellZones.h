#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormCellZones.h"
#include <QString>
#include "GlobalData.h"
#include "FormCellZonesTabWidget.h"
#include "CustomItemWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormCellZonesClass; };
QT_END_NAMESPACE

class FormCellZones : public QWidget
{
	Q_OBJECT

public:
	FormCellZones(QWidget *parent = nullptr);
	~FormCellZones();

public slots:
	void initialization(const QString& solver = "rhoSimpleFoam", const QString& regionType = "fluid", const QString& turbulenceType = "Laminar");
	void on_ListWidgetItem_Clicked(QListWidgetItem* item);

private:
	Ui::FormCellZonesClass *ui;
	std::vector<FormCellZonesTabWidget*> formCellZonesTabWidgetsList;
};
