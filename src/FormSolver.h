﻿/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormSolver.h"
#include <QStandardItemModel>
#include <QMessageBox>
#include "GlobalData.h"
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include "DialogInformationPrompt.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormSolverClass; };
QT_END_NAMESPACE

class FormSolver : public QWidget
{
	Q_OBJECT

public:
	FormSolver(QWidget *parent = nullptr);
	~FormSolver();

	bool importParameter();
	bool exportParameter();

public slots:
	void on_pushButton_5_clicked();
	void handleButtonGroup1();
	void handleButtonGroup2();
	void updateListView();

signals:
	void labelText_8_Changed(const QString& newText);

public:
	Ui::FormSolverClass *ui;
	QStandardItemModel* model;
};
