/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormThermo.h"
#include "FormSolver.h"
#include "GlobalData.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormThermoClass; };
QT_END_NAMESPACE

class FormThermo : public QWidget
{
	Q_OBJECT

public:
	FormThermo(QWidget *parent = nullptr);
	~FormThermo();
	
	bool importParameter();
	bool exportParameter();

public slots:
	void solverChanged(const QString& newText);

private:
	Ui::FormThermoClass *ui;
};
