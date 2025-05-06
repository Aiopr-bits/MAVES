#pragma once

#include <QWidget>
#include "ui_FormOperatingConditions.h"
#include <QRegularExpressionValidator>
#include <QLineEdit>
#include <QRadioButton>

QT_BEGIN_NAMESPACE
namespace Ui { class FormOperatingConditionsClass; };
QT_END_NAMESPACE

class FormOperatingConditions : public QWidget
{
	Q_OBJECT

public:
	FormOperatingConditions(QWidget *parent = nullptr);
	~FormOperatingConditions();

private slots:
	void on_RadioButton_7_Toggled(bool checked);
	void on_RadioButton_8_Toggled(bool checked);

private:
	Ui::FormOperatingConditionsClass *ui;
};
