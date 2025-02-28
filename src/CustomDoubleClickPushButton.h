/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#pragma execution_character_set("utf-8")
#include <QPushButton>
#include <QMouseEvent>
#include <QWidget>

class CustomDoubleClickPushButton : public QPushButton {
    Q_OBJECT

public:
    explicit CustomDoubleClickPushButton(QWidget* parent = nullptr) : QPushButton(parent) {}

signals:
    void doubleClicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            emit doubleClicked();
        }
 
        QPushButton::mouseDoubleClickEvent(event);
    }
};
