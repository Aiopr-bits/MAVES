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
#include "DialogInformationPrompt.h"
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class FormTurbulenceClass; };
QT_END_NAMESPACE

class FormTurbulence : public QWidget
{
	Q_OBJECT

public:
	FormTurbulence(QWidget *parent = nullptr);
	~FormTurbulence();

private slots:
	void onRadioButtonToggled();									//湍流模型切换
	void onComboBox_5_IndexChanged(int index);						//雷诺平均-模型切换
	void onComboBox_6_IndexChanged(int index);						//大涡模拟-模型切换
	void onComboBox_7_IndexChanged(int index);						//变化量切换
	void onComboBox_8_IndexChanged(int index);						//过滤器切换
	void on_PushButton_3_Toggled(bool checked);						//雷诺平均-模型		参数面板收起展开切换
	void on_PushButton_6_Toggled(bool checked);						//大涡模拟-模型		参数面板收起展开切换
	void on_PushButton_7_Toggled(bool checked);						//变化量切换		参数面板收起展开切换
	void on_PushButton_8_Toggled(bool checked);						//过滤器切换		参数面板收起展开切换

signals:
	void turbulenceTypeChanged();

public:
	Ui::FormTurbulenceClass *ui;

//以下可能删除
public:
	bool importParameter();
	bool exportParameter();
	void onComboBoxTextChanged(const QString& text);
};
