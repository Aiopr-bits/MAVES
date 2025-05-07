/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QMainWindow>
#include "ui_CurvePlotting.h"
#include "ThreeDimensionalComputation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CurvePlottingClass; };
QT_END_NAMESPACE

class CurvePlotting : public QMainWindow
{
    Q_OBJECT

public:
    CurvePlotting(QWidget *parent = nullptr);
    ~CurvePlotting();

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_PushButton_3_DoubleClicked();

private:
    Ui::CurvePlottingClass *ui;
    ThreeDimensionalComputation threeDimensionalComputation;
};
