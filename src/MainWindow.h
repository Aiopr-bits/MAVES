/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include "ui_MainWindow.h"
#include <QDesktopWidget>
#include <QGraphicsOpacityEffect>
#include "ThreeDimensionalComputation.h"
#include <QPropertyAnimation>
#include "CurvePlotting.h"
#include <QDesktopServices>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void subroutine(QString path);								//调用子程序

public slots:
	void on_PushButton_clicked();								//综合管理软件
	void on_PushButton_2_Toggled(bool checked);					//传热机理模块
	void on_PushButton_6_Toggled(bool checked);					//CFD仿真预计模块
	void on_PushButton_9_clicked();								//FEA仿真预计模块
	void on_PushButton_3_clicked();								//机器学习预计模块
	void on_PushButton_8_clicked();								//深度学习预计模块
	void on_PushButton_7_clicked();								//回归算法预计模块
	void on_PushButton_10_clicked();							//知识图谱
	void on_PushButton_11_clicked();							//TMAX3D
	void on_PushButton_12_clicked();							//曲线绘制
	void on_PushButton_13_clicked();							//传热机理模块 - 一维热载荷
	void on_PushButton_14_clicked();							//传热机理模块 - 三维计算模块
	void on_PushButton_15_clicked();							//传热机理模块 - 内外流场插值
	void on_PushButton_16_clicked();							//CFD仿真预计模块 - 内外流场插值
	void on_PushButton_17_clicked();							//CFD仿真预计模块 - 三维计算模块
	void on_PushButton_4_clicked();								//设置

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	Ui::MainWindowClass *ui;
};
