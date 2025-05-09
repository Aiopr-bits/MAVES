#pragma once

#include <QWidget>
#include "ui_MainWindowSetting.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowSettingClass; };
QT_END_NAMESPACE

class MainWindowSetting : public QWidget
{
	Q_OBJECT

public:
	MainWindowSetting(QWidget *parent = nullptr);
	~MainWindowSetting();
	
	void readConfig();
	void writeConfig();

protected:
	void closeEvent(QCloseEvent* event);

private:
	Ui::MainWindowSettingClass *ui;
};
