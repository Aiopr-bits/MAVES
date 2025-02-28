/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormTurbulence.h"
#include "GlobalData.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class FormTurbulenceClass; };
QT_END_NAMESPACE

class FormTurbulence : public QWidget
{
	Q_OBJECT

public:
	FormTurbulence(QWidget *parent = nullptr);
	~FormTurbulence();

	bool importParameter();
	bool exportParameter();
	void onComboBoxTextChanged(const QString& text);

private:
	Ui::FormTurbulenceClass *ui;
};
