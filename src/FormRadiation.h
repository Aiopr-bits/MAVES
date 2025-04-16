#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormRadiation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormRadiationClass; };
QT_END_NAMESPACE

class FormRadiation : public QWidget
{
	Q_OBJECT

public:
	FormRadiation(QWidget *parent = nullptr);
	~FormRadiation();

public slots:
	void on_CheckBox_Toggled(bool checked);
	void on_PushButton_3_Toggled(bool checked);
	void on_PushButton_4_Toggled(bool checked);
	void on_PushButton_5_Toggled(bool checked);
	void on_PushButton_6_Toggled(bool checked);
	void on_PushButton_7_Toggled(bool checked);
	void onComboBox_5_IndexChanged(int index);
	void onComboBox_6_IndexChanged(int index);
	void onComboBox_7_IndexChanged(int index);
	void onComboBox_8_IndexChanged(int index);
	void onComboBox_9_IndexChanged(int index);

private:
	Ui::FormRadiationClass *ui;
};
