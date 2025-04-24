#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormInitialConditions.h"
#include "FormInitialConditionsTabWidget.h"
#include "GlobalData.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormInitialConditionsClass; };
QT_END_NAMESPACE

class FormInitialConditions : public QWidget
{
	Q_OBJECT

public:
	FormInitialConditions(QWidget* parent = nullptr);
	~FormInitialConditions();

public slots:
	void initialization(const QString& solver = "rhoSimpleFoam", const QString& regionType = "fluid", const QString& turbulenceType = "Laminar");
	void on_ListWidgetItem_Clicked(QListWidgetItem* item);

private:
	Ui::FormInitialConditionsClass *ui;
	std::vector<FormInitialConditionsTabWidget*> formInitialConditionsTabWidgetsList;
};
