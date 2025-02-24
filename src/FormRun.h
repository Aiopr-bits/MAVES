#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormRun.h"
#include <QEvent>
#include <QIcon>
#include <QFileInfo>
#include "GlobalData.h"
#include <QMessageBox>
#include <QTextStream>
#include <QTabWidget>
#include <QPropertyAnimation>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class FormRunClass; };
QT_END_NAMESPACE

class FormRun : public QWidget
{
	Q_OBJECT

public:
	FormRun(QWidget *parent = nullptr);
	~FormRun();
	void importParameter();
	void exportParameter();

public slots:
	void cursorEnterPushButton();
	void cursorLeavePushButton();
	void on_pushButton_clicked();					//开始计算
	void on_pushButton_clicked_2();					//停止计算
	void resizeEvent(QResizeEvent* event);			
	void on_tabWidget_currentChanged(int index);	
	void on_radioButton_2_toggled(bool checked);

signals:
	void run();
	void stopRun();

public:
	Ui::FormRunClass *ui;
	int previousIndex;
};
