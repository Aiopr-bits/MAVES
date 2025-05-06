#pragma once

#include <QWidget>
#include "ui_FormOperatingConditions.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormOperatingConditionsClass; };
QT_END_NAMESPACE

class FormOperatingConditions : public QWidget
{
	Q_OBJECT

public:
	FormOperatingConditions(QWidget *parent = nullptr);
	~FormOperatingConditions();

private:
	Ui::FormOperatingConditionsClass *ui;
};
