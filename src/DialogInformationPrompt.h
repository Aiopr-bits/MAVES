/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#pragma execution_character_set("utf-8")
#include <QDialog>
#include "ui_DialogInformationPrompt.h"
#include <QMouseEvent>
#include <QTimer> 

QT_BEGIN_NAMESPACE
namespace Ui { class DialogInformationPromptClassbao; };
QT_END_NAMESPACE

class DialogInformationPrompt : public QDialog
{
	Q_OBJECT

public:
	DialogInformationPrompt(QWidget* parent = nullptr, const QString titleText = "信息提示", const QStringList& texts = { "信息提示框" }, bool isShowCloseButton = true, bool isRollText = false);
	~DialogInformationPrompt();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
	void updateLabelText();	
	void on_pushButton_clicked();

public:
	Ui::DialogInformationPromptClass *ui;
	QPoint m_mousePosition; 
	bool m_isMousePressed; 
	QTimer* m_timer;
	QStringList m_texts;	// 文本列表
	int m_textIndex;		// 当前文本索引
};
