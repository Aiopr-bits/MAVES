#pragma once
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
