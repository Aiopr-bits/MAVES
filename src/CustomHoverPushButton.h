#pragma once	
#include <QPushButton>
#include <QEvent>
#include <QIcon>
#include <QTimer>

class CustomHoverPushButton : public QPushButton
{
    Q_OBJECT

public:
    CustomHoverPushButton(QWidget* parent = nullptr) : QPushButton(parent) {
        QTimer::singleShot(0, this, [this]() {
            originalIconSize = iconSize();
            });
    }

protected:
    void enterEvent(QEvent* event) override {
        QSize newSize = originalIconSize * 1.1;
        setIconSize(newSize);
        QPushButton::enterEvent(event);
        emit cursorEnter();
    }

    void leaveEvent(QEvent* event) override {
        setIconSize(originalIconSize);
        QPushButton::leaveEvent(event);
        emit cursorLeave();
    }

signals:
    void cursorEnter();
    void cursorLeave();

private:
    QSize originalIconSize;
};

