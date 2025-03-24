/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#include "DialogInformationPrompt.h"
#include "QPainterPath"
#include <qmessagebox.h>
#include <QGraphicsDropShadowEffect>

DialogInformationPrompt::DialogInformationPrompt(QWidget *parent)
	: QDialog(parent)
    , m_textIndex(0)
	, ui(new Ui::DialogInformationPromptClass())
{
	ui->setupUi(this);
	this->resize(300, 270);

    //实例阴影shadow
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor("#444444"));
    shadow->setBlurRadius(30);
    ui->frame->setGraphicsEffect(shadow);
    QVBoxLayout* layout = new QVBoxLayout(ui->frame);
    layout->setContentsMargins(24, 24, 24, 24); 

    //设置窗体透明,无边框
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // 初始化定时器
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &DialogInformationPrompt::updateLabelText);
    m_timer->start(500);
}

DialogInformationPrompt::~DialogInformationPrompt()
{
	delete ui;
}

void DialogInformationPrompt::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isMousePressed = true;
        m_mousePosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void DialogInformationPrompt::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isMousePressed && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - m_mousePosition);
        event->accept();
    }
}

void DialogInformationPrompt::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isMousePressed = false;
        event->accept();
    }
}

void DialogInformationPrompt::updateLabelText()
{
    // 定义要循环的字符串数组
    static const QStringList texts = {
        "数据加载中      ",
        "数据加载中 .    ",
        "数据加载中 . .  ",
        "数据加载中 . . ."
    };

    ui->label_2->setText(texts[m_textIndex]);
    m_textIndex = (m_textIndex + 1) % texts.size();
}