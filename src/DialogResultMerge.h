/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#pragma execution_character_set("utf-8")
#include <QDialog>
#include "ui_DialogResultMerge.h"
#include <QMouseEvent>
#include <QTimer> 

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
	void updateLabelText();						

signals:
	void interrupt();

public:
	Ui::DialogResultMergeClass *ui;
	QPoint m_mousePosition; 
	bool m_isMousePressed; 
	QTimer* m_timer;
	int m_textIndex; // 当前文本索引
};
