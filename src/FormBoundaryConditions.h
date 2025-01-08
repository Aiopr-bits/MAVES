#pragma once

#include <QWidget>
#include "ui_FormBoundaryConditions.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormBoundaryConditionsClass; };
QT_END_NAMESPACE

class FormBoundaryConditions : public QWidget
{
	Q_OBJECT

public:
	FormBoundaryConditions(QWidget *parent = nullptr);
	~FormBoundaryConditions();

private:
	Ui::FormBoundaryConditionsClass *ui;
};
