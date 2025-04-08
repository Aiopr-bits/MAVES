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
#include "ui_CustomItemWidgetMeshZones.h"
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
    class CustomItemWidgetMeshBoundaries1Class;                 //type  0
	class CustomItemWidgetMeshBoundaries2Class; 		        //type  1
	class CustomItemWidgetMeshRegions1Class; 		            //type  2
	class CustomItemWidgetMeshRegions2Class;                    //type  3
	class CustomItemWidgetMeshZonesClass; 				        //type  4
};
QT_END_NAMESPACE

class CustomItemWidget : public QWidget {
    Q_OBJECT
public:
    explicit CustomItemWidget(
        int styleIndex,
        QWidget* parent = nullptr, 
        const QString& text = QString(),
        const QString& text2 = QString(),
		const QString& text3 = QString(),
		const QString& text4 = QString()
    );

    ~CustomItemWidget();

    bool eventFilter(QObject* obj, QEvent* event) override;
    void initializeUI(
        int styleIndex, 
        const QString& text, 
        const QString& text2 = QString(),
		const QString& text3 = QString(),
		const QString& text4 = QString()
    );
    void beginEdit(QLabel* label);
    void finishEdit();

signals:
    void textChanged(CustomItemWidget* widget, QString previousText);

public:
    Ui::CustomItemWidgetMeshBoundaries1Class* ui_ItemWidgetMeshBoundaries1; 
    Ui::CustomItemWidgetMeshBoundaries2Class* ui_ItemWidgetMeshBoundaries2;
    Ui::CustomItemWidgetMeshRegions1Class* ui_ItemWidgetMeshRegions1;
    Ui::CustomItemWidgetMeshRegions2Class* ui_ItemWidgetMeshRegions2;
	Ui::CustomItemWidgetMeshZonesClass* ui_ItemWidgetMeshZones;

    QLineEdit* lineEdit;
    bool editing;
    QString text1;
    QString text2;
	QString text3;
	QString text4;
};
