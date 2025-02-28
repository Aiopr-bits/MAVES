/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormModelClip.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormModelClipClass; };
QT_END_NAMESPACE

class FormModelClip : public QWidget
{
	Q_OBJECT

public:
	FormModelClip(QWidget *parent = nullptr);
	~FormModelClip();

public slots:
	void onLineEditChanged();
	void on_pushButton_2_clicked();							//X正向
	void on_pushButton_3_clicked();							//Y正向
	void on_pushButton_4_clicked();							//Z正向
	void on_pushButton_5_clicked();							//相机方向
	void on_pushButton_6_clicked();							//对齐视角
	void on_pushButton_7_clicked();							//重置平面
	void on_pushButton_clicked();							//应用

signals:
	void checkBoxToggled(bool checked);
	void lineEditsChanged();
	void xPositive();										//X正向
	void yPositive();										//Y正向
	void zPositive();										//Z正向
	void cameraDirection();									//相机方向
	void alignView();										//对齐视角
	void resetPlane();										//重置平面
	void apply();											//应用




public:
	Ui::FormModelClipClass *ui;
};
