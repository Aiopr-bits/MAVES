#include "DialogResultMerge.h"
#include "QPainterPath"
#include <qmessagebox.h>
#include <QGraphicsDropShadowEffect>

DialogResultMerge::DialogResultMerge(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DialogResultMergeClass())
{
	ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);
	this->resize(250, 220);

    connect(ui->pushButton, &QPushButton::clicked, this, &DialogResultMerge::on_pushButton_clicked);
}

DialogResultMerge::~DialogResultMerge()
{
	delete ui;
}

void DialogResultMerge::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isMousePressed = true;
        m_mousePosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void DialogResultMerge::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isMousePressed && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - m_mousePosition);
        event->accept();
    }
}

void DialogResultMerge::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isMousePressed = false;
        event->accept();
    }
}

void DialogResultMerge::on_pushButton_clicked()
{
	emit interruptMerge();
	this->hide();
}