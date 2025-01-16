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
};