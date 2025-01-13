#pragma once	
#pragma execution_character_set("utf-8")
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

    void mousePressEvent(QMouseEvent* event) override {
        QSize newSize = originalIconSize * 0.8;
        setIconSize(newSize);
        QPushButton::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        setIconSize(originalIconSize);
        QPushButton::mouseReleaseEvent(event);
    }

signals:
    void cursorEnter();
    void cursorLeave();

private:
    QSize originalIconSize;
};
