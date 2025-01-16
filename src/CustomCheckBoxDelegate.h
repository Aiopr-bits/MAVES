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

            // ��ȡ CheckBox ������
            QStyleOptionViewItem viewOption(option);
            QRect checkRect = QApplication::style()->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &viewOption, nullptr);

            // ������ת��Ϊ������������
            QPoint point = mouseEvent->pos() - option.rect.topLeft();

            if (!checkRect.contains(point))
            {
                // ����� CheckBox ֮�⣬�ֶ��л�״̬
                Qt::CheckState state = static_cast<Qt::CheckState>(index.data(Qt::CheckStateRole).toInt());
                state = (state == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
                model->setData(index, state, Qt::CheckStateRole);
                return true;
            }
        }

        // ���������¼���ʹ��Ĭ�ϴ���
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }
};