#pragma once	
#include <QPushButton>
#include <QEvent>
#include <QIcon>

class CustomHoverPushButton : public QPushButton
{
    Q_OBJECT

public:
    CustomHoverPushButton(QWidget* parent = nullptr) : QPushButton(parent) {
        setIconSize(QSize(100, 100));
    }

protected:
    void enterEvent(QEvent* event) override {
        setIconSize(QSize(110, 110));
        QPushButton::enterEvent(event);
		emit cursorEnter();
    }

    void leaveEvent(QEvent* event) override {
        setIconSize(QSize(100, 100));
        QPushButton::leaveEvent(event);
		emit cursorLeave();
    }

signals:
    void cursorEnter();
	void cursorLeave();
};
