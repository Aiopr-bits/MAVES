#include "CustomItemWidget.h"

CustomItemWidget::CustomItemWidget(int styleIndex, const QString& text, const QString& text2, QWidget* parent)
    : QWidget(parent), ui_ItemWidgetMeshBoundaries1(nullptr), ui_ItemWidgetMeshBoundaries2(nullptr), ui_ItemWidgetMeshRegions1(nullptr), ui_ItemWidgetMeshRegions2(nullptr), lineEdit(nullptr), editing(false)
{
    initializeUI(styleIndex, text, text2);
}

CustomItemWidget::~CustomItemWidget()
{
    delete ui_ItemWidgetMeshBoundaries1;
    delete ui_ItemWidgetMeshBoundaries2;
    delete ui_ItemWidgetMeshRegions1;
    delete ui_ItemWidgetMeshRegions2;
}

void CustomItemWidget::initializeUI(int styleIndex, const QString& text, const QString& text2)
{
    if (styleIndex == 0) {
        ui_ItemWidgetMeshBoundaries1 = new Ui::CustomItemWidgetMeshBoundaries1Class();
        ui_ItemWidgetMeshBoundaries1->setupUi(this);
        ui_ItemWidgetMeshBoundaries1->label->setText(text);

        // 设置 QComboBox 的自定义视图
        QListView* combo1View1 = new QListView(ui_ItemWidgetMeshBoundaries1->comboBox);
        combo1View1->setMinimumWidth(150);
        ui_ItemWidgetMeshBoundaries1->comboBox->setView(combo1View1);

        QListView* combo1View2 = new QListView(ui_ItemWidgetMeshBoundaries1->comboBox_2);
        combo1View2->setMinimumWidth(150);
        ui_ItemWidgetMeshBoundaries1->comboBox_2->setView(combo1View2);

        // 安装事件过滤器
        ui_ItemWidgetMeshBoundaries1->label->installEventFilter(this);
    }
    else if (styleIndex == 1) {
        ui_ItemWidgetMeshBoundaries2 = new Ui::CustomItemWidgetMeshBoundaries2Class();
        ui_ItemWidgetMeshBoundaries2->setupUi(this);
        ui_ItemWidgetMeshBoundaries2->label->setText(text);
		ui_ItemWidgetMeshBoundaries2->label_2->setText(text2);

        // 安装事件过滤器
        ui_ItemWidgetMeshBoundaries2->label->installEventFilter(this);
        ui_ItemWidgetMeshBoundaries2->label_2->installEventFilter(this);
    }
    else if (styleIndex == 2) {
        ui_ItemWidgetMeshRegions1 = new Ui::CustomItemWidgetMeshRegions1Class();
        ui_ItemWidgetMeshRegions1->setupUi(this);
        ui_ItemWidgetMeshRegions1->label->setText(text);

        // 设置 QComboBox 的自定义视图
        QListView* combo1View1 = new QListView(ui_ItemWidgetMeshRegions1->comboBox);
        combo1View1->setMinimumWidth(150);
        ui_ItemWidgetMeshRegions1->comboBox->setView(combo1View1);

        // 安装事件过滤器
        ui_ItemWidgetMeshRegions1->label->installEventFilter(this);
    }
    else if (styleIndex == 3) {
        ui_ItemWidgetMeshRegions2 = new Ui::CustomItemWidgetMeshRegions2Class();
        ui_ItemWidgetMeshRegions2->setupUi(this);
        ui_ItemWidgetMeshRegions2->label->setText(text);

        // 设置 QComboBox 的自定义视图
        QListView* combo1View1 = new QListView(ui_ItemWidgetMeshRegions2->comboBox);
        combo1View1->setMinimumWidth(150);
        ui_ItemWidgetMeshRegions2->comboBox->setView(combo1View1);

        QListView* combo1View2 = new QListView(ui_ItemWidgetMeshRegions2->comboBox_2);
        combo1View2->setMinimumWidth(150);
        ui_ItemWidgetMeshRegions2->comboBox_2->setView(combo1View2);

        // 安装事件过滤器
        ui_ItemWidgetMeshRegions2->label->installEventFilter(this);
    }
    else {
        return;
    }
}

bool CustomItemWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::Enter || event->type() == QEvent::Leave) {
        if (ui_ItemWidgetMeshBoundaries1 && obj == ui_ItemWidgetMeshBoundaries1->label) {
            if (event->type() == QEvent::Enter) {
                qDebug() << "Mouse entered ui_ItemWidgetMeshBoundaries1->label";
            }
            else {
                qDebug() << "Mouse left ui_ItemWidgetMeshBoundaries1->label";
            }
        }
        if (ui_ItemWidgetMeshBoundaries2 && (obj == ui_ItemWidgetMeshBoundaries2->label || obj == ui_ItemWidgetMeshBoundaries2->label_2)) {
            if (event->type() == QEvent::Enter) {
                qDebug() << "Mouse entered" << (obj == ui_ItemWidgetMeshBoundaries2->label ? "ui_ItemWidgetMeshBoundaries2->label" : "ui_ItemWidgetMeshBoundaries2->label_2");
            }
            else {
                qDebug() << "Mouse left" << (obj == ui_ItemWidgetMeshBoundaries2->label ? "ui_ItemWidgetMeshBoundaries2->label" : "ui_ItemWidgetMeshBoundaries2->label_2");
            }
        }
        if (ui_ItemWidgetMeshRegions1 && obj == ui_ItemWidgetMeshRegions1->label) {
            if (event->type() == QEvent::Enter) {
                qDebug() << "Mouse entered ui_ItemWidgetMeshRegions1->label";
            }
            else {
                qDebug() << "Mouse left ui_ItemWidgetMeshRegions1->label";
            }
        }
        if (ui_ItemWidgetMeshRegions2 && obj == ui_ItemWidgetMeshRegions2->label) {
            if (event->type() == QEvent::Enter) {
                qDebug() << "Mouse entered ui_ItemWidgetMeshRegions2->label";
            }
            else {
                qDebug() << "Mouse left ui_ItemWidgetMeshRegions2->label";
            }
        }
    }

    if (ui_ItemWidgetMeshBoundaries1 && obj == ui_ItemWidgetMeshBoundaries1->label && event->type() == QEvent::MouseButtonDblClick) {
        beginEdit(ui_ItemWidgetMeshBoundaries1->label);
        return true;
    }
    if (ui_ItemWidgetMeshBoundaries2 && (obj == ui_ItemWidgetMeshBoundaries2->label || obj == ui_ItemWidgetMeshBoundaries2->label_2) && event->type() == QEvent::MouseButtonDblClick) {
        beginEdit(qobject_cast<QLabel*>(obj));
        return true;
    }
    if (ui_ItemWidgetMeshRegions1 && obj == ui_ItemWidgetMeshRegions1->label && event->type() == QEvent::MouseButtonDblClick) {
        return true;
    }
    if (ui_ItemWidgetMeshRegions2 && obj == ui_ItemWidgetMeshRegions2->label && event->type() == QEvent::MouseButtonDblClick) {
        beginEdit(ui_ItemWidgetMeshRegions2->label);
        return true;
    }

    if (editing && event->type() == QEvent::MouseButtonPress) {
        if (obj != lineEdit) {
            finishEdit();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void CustomItemWidget::beginEdit(QLabel* label)
{
    if (editing) return;
    editing = true;

    QBoxLayout* boxLayout = nullptr;

    if (ui_ItemWidgetMeshBoundaries1) {
        boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshBoundaries1->horizontalLayout);
    }
    else if (ui_ItemWidgetMeshBoundaries2) {
        boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshBoundaries2->horizontalLayout);
    }
	else if (ui_ItemWidgetMeshRegions1) {
		boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshRegions1->horizontalLayout);
	}
	else if (ui_ItemWidgetMeshRegions2) {
		boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshRegions2->horizontalLayout);
	}

    if (!label || !boxLayout) return;

    lineEdit = new QLineEdit(label->text(), this);
    lineEdit->setGeometry(label->geometry()); // 设置 lineEdit 的几何属性与 label 相同
    lineEdit->setSizePolicy(label->sizePolicy()); // 设置 lineEdit 的大小策略与 label 相同
    int labelIndex = boxLayout->indexOf(label);
    boxLayout->removeWidget(label);
    label->hide();
    boxLayout->insertWidget(labelIndex, lineEdit);
    lineEdit->setFocus();
    connect(lineEdit, &QLineEdit::editingFinished, this, &CustomItemWidget::finishEdit);

    qApp->installEventFilter(this);
}

void CustomItemWidget::finishEdit()
{
    editing = false;
    QString newText = lineEdit->text();

    QLabel* label = nullptr;
    QBoxLayout* boxLayout = nullptr;

    if (ui_ItemWidgetMeshBoundaries1) {
        label = ui_ItemWidgetMeshBoundaries1->label;
        boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshBoundaries1->horizontalLayout);
    }
    else if (ui_ItemWidgetMeshBoundaries2) {
        if (ui_ItemWidgetMeshBoundaries2->label->isHidden()) {
            label = ui_ItemWidgetMeshBoundaries2->label;
        }
        else {
            label = ui_ItemWidgetMeshBoundaries2->label_2;
        }
        boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshBoundaries2->horizontalLayout);
    }
	else if (ui_ItemWidgetMeshRegions1) {
		label = ui_ItemWidgetMeshRegions1->label;
		boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshRegions1->horizontalLayout);
	}
	else if (ui_ItemWidgetMeshRegions2) {
		label = ui_ItemWidgetMeshRegions2->label;
		boxLayout = qobject_cast<QBoxLayout*>(ui_ItemWidgetMeshRegions2->horizontalLayout);
	}

    if (!label || !boxLayout) return;

    int lineEditIndex = boxLayout->indexOf(lineEdit); 
    boxLayout->removeWidget(lineEdit);
    lineEdit->hide();
    label->setText(newText);
    boxLayout->insertWidget(lineEditIndex, label); 
    label->show();

    emit textChanged(newText);
    lineEdit->deleteLater();
    lineEdit = nullptr;

    qApp->removeEventFilter(this);
}
