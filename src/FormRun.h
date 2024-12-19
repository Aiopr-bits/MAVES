#pragma once
#include <QWidget>
#include "ui_FormRun.h"
#include <QEvent>
#include <QIcon>

#pragma execution_character_set("utf-8")

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
	void cursorEnter();
	void cursorLeave();
	void on_pushButton_clicked();

signals:
	void run();

private:
	Ui::FormRunClass *ui;
};
