#pragma once
#pragma execution_character_set("utf-8")
#include <QDialog>
#include "ui_DialogResultMerge.h"
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class DialogResultMergeClass; };
QT_END_NAMESPACE

class DialogResultMerge : public QDialog
{
	Q_OBJECT

public:
	DialogResultMerge(QWidget *parent = nullptr);
	~DialogResultMerge();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
	void on_pushButton_clicked();

signals:
	void interruptMerge();

private:
	Ui::DialogResultMergeClass *ui;
	QPoint m_mousePosition; 
	bool m_isMousePressed; 
};
