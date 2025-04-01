#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include <QLineEdit>
#include <QListView>
#include <QMap>
#include <QEvent>
#include <QApplication>
#include <qdebug.h>
#include "ui_CustomItemWidgetMeshBoundaries1.h"
#include "ui_CustomItemWidgetMeshBoundaries2.h"
#include "ui_CustomItemWidgetMeshRegions1.h"
#include "ui_CustomItemWidgetMeshRegions2.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class CustomItemWidgetMeshBoundaries1Class;
    class CustomItemWidgetMeshBoundaries2Class;
    class CustomItemWidgetMeshRegions1Class;
    class CustomItemWidgetMeshRegions2Class;  
};
QT_END_NAMESPACE

class CustomItemWidget : public QWidget {
    Q_OBJECT
public:
    explicit CustomItemWidget(int styleIndex, const QString& text, const QString& text2 = QString(), QWidget* parent = nullptr);
    ~CustomItemWidget();

    bool eventFilter(QObject* obj, QEvent* event) override;
    void initializeUI(int styleIndex, const QString& text, const QString& text2 = "");
    void beginEdit(QLabel* label);
    void finishEdit();

signals:
    void textChanged(const QString& newText);

private:
    Ui::CustomItemWidgetMeshBoundaries1Class* ui_ItemWidgetMeshBoundaries1; 
    Ui::CustomItemWidgetMeshBoundaries2Class* ui_ItemWidgetMeshBoundaries2;
    Ui::CustomItemWidgetMeshRegions1Class* ui_ItemWidgetMeshRegions1;
    Ui::CustomItemWidgetMeshRegions2Class* ui_ItemWidgetMeshRegions2;
    QLineEdit* lineEdit;
    bool editing;
};
