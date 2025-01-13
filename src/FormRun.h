#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormRun.h"
#include <QEvent>
#include <QIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class FormRunClass; };
QT_END_NAMESPACE

class FormRun : public QWidget
{
	Q_OBJECT

public:
	FormRun(QWidget *parent = nullptr);
	~FormRun();

public slots:
	void cursorEnterPushButton();
	void cursorLeavePushButton();

	void on_pushButton_clicked();				//开始计算
	void on_pushButton_clicked_2();				//停止计算

signals:
	void run();
	void stopRun();

public:
	Ui::FormRunClass *ui;
};
