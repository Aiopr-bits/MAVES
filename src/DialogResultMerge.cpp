#include "DialogResultMerge.h"
#include "QPainterPath"
#include <qmessagebox.h>
#include <QGraphicsDropShadowEffect>

DialogResultMerge::DialogResultMerge(QWidget *parent)
	: QDialog(parent)
    , m_textIndex(0)
	, ui(new Ui::DialogResultMergeClass())
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
    connect(m_timer, &QTimer::timeout, this, &DialogResultMerge::updateLabelText);
    m_timer->start(500);

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

void DialogResultMerge::updateLabelText()
{
    // 定义要循环的字符串数组
    static const QStringList texts = {
        "结果合并中      ",
        "结果合并中 .    ",
        "结果合并中 . .  ",
        "结果合并中 . . ."
    };

    // 更新标签文本
    ui->label_2->setText(texts[m_textIndex]);

    // 更新索引
    m_textIndex = (m_textIndex + 1) % texts.size();
}

void DialogResultMerge::on_pushButton_clicked()
{
	emit interruptMerge();
	this->hide();
}