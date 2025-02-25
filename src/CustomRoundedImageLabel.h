#pragma once
#pragma execution_character_set("utf-8")
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPainterPath>

class CustomRoundedImageLabel : public QLabel {
    Q_OBJECT

public:
    explicit CustomRoundedImageLabel(QWidget* parent = nullptr) : QLabel(parent) {}

    void setPixmap(const QPixmap& pixmap) {
        m_pixmap = pixmap;
        update();
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

        QPainterPath path;
        path.addRoundedRect(rect(), 20, 20); 
        painter.setClipPath(path);

        if (!m_pixmap.isNull()) {
            painter.drawPixmap(rect(), m_pixmap);
        }

        QLabel::paintEvent(event);
    }

private:
    QPixmap m_pixmap;
};

