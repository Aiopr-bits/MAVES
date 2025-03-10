 /*--------------------------------*- C++ -*----------------------------------*\
 | MAVES：Multi-source Adaptability Verification and Evaluation Software       |
 | Author: Zhiyong Zeng                                                        |
 | Date: December 01, 2024                                                     |
 | Email: mailzengzhiyong@gamil.com                                            |
 \*---------------------------------------------------------------------------*/

#pragma once
#include <QStyledItemDelegate>
#include <QMouseEvent>
#include <QApplication>
#include <QPainter>

class CustomCheckBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CustomCheckBoxDelegate(QObject* parent = nullptr) {}

    bool editorEvent(QEvent* event, QAbstractItemModel* model,
        const QStyleOptionViewItem& option, const QModelIndex& index) override {
        if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

            // 获取 CheckBox 的区域
            QStyleOptionViewItem viewOption(option);
            QRect checkRect = QApplication::style()->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &viewOption, nullptr);

            // 将坐标转换为相对于项的坐标
            QPoint point = mouseEvent->pos() - option.rect.topLeft();

            if (!checkRect.contains(point))
            {
                // 点击在 CheckBox 之外，手动切换状态
                Qt::CheckState state = static_cast<Qt::CheckState>(index.data(Qt::CheckStateRole).toInt());
                state = (state == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
                model->setData(index, state, Qt::CheckStateRole);
                return true;
            }
        }

        // 对于其他事件，使用默认处理
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

    void paint(QPainter* painter,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const
    {
        // 先调用基类进行默认绘制（包含文本和复选框等）
        QStyledItemDelegate::paint(painter, option, index);

        // 获取item文本，判断使用不同图标
        QString text = index.data(Qt::DisplayRole).toString();
        QIcon icon = (text == "internalMesh")
            ? QIcon("..\\res\\internalMesh.png")
            : QIcon("..\\res\\patch.png");

        // 在右侧绘制图标
        QPixmap pixmap = icon.pixmap(QSize(16, 16));
        int x = option.rect.right() - pixmap.width() - 8;  // 向内缩进一点
        int y = option.rect.center().y() - pixmap.height() / 2;
        painter->drawPixmap(x, y, pixmap);
    }
};