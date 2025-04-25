#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormInitialConditionsTabWidget.h"
#include <QRegularExpressionValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class FormInitialConditionsTabWidgetClass; };
QT_END_NAMESPACE

class FormInitialConditionsTabWidget : public QWidget
{
	Q_OBJECT

public:
	FormInitialConditionsTabWidget(const QString& solver = "rhoSimpleFoam", const QString& regionType = "fluid", const QString& turbulenceType = "Laminar", QWidget* parent = nullptr);
	~FormInitialConditionsTabWidget();

public slots:
	void initialization(const QString& solver, const QString&  regionType, const QString& turbulenceType);
	void resizeEvent(QResizeEvent* event);

private:
	Ui::FormInitialConditionsTabWidgetClass *ui;
};
