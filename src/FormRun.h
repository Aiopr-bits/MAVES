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

	void on_pushButton_clicked();				//��ʼ����
	void on_pushButton_clicked_2();				//ֹͣ����

signals:
	void run();
	void stopRun();

public:
	Ui::FormRunClass *ui;
};
