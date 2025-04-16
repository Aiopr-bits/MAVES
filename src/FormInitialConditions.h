#pragma once

#include <QWidget>
#include "ui_FormInitialConditions.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormInitialConditionsClass; };
QT_END_NAMESPACE

class FormInitialConditions : public QWidget
{
	Q_OBJECT

public:
	FormInitialConditions(QWidget *parent = nullptr);
	~FormInitialConditions();

private:
	Ui::FormInitialConditionsClass *ui;
};
