#pragma once

#include <QWidget>
#include "ui_FormSolver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormSolverClass; };
QT_END_NAMESPACE

class FormSolver : public QWidget
{
	Q_OBJECT

public:
	FormSolver(QWidget *parent = nullptr);
	~FormSolver();

private:
	Ui::FormSolverClass *ui;
};
