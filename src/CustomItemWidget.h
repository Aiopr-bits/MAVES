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
#include "ui_CustomItemWidgetBoundaryConditions.h"
#include "ui_CustomItemWidgetCellZones.h"
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
    //Param1 : label的text
    //Param2 : label的鼠标悬浮text          
    //Param3 : 没用
    //Param4 : 没用
    class CustomItemWidgetMeshBoundaries1Class;                 //type  0 

    //Param1 : label的text
    //Param2 : label的鼠标悬浮text          
    //Param3 : label_2的text
    //Param4 : label_2的鼠标悬浮text
	class CustomItemWidgetMeshBoundaries2Class; 		        //type  1

    //Param1 : label的text
    //Param2 : 没用        
    //Param3 : 没用
    //Param4 : 没用
	class CustomItemWidgetMeshRegions1Class; 		            //type  2

    //Param1 : label的text
    //Param2 : 没用        
    //Param3 : 没用
    //Param4 : 没用
	class CustomItemWidgetMeshRegions2Class;                    //type  3

    //Param1 : label的text
    //Param2 : 没用        
    //Param3 : 没用
    //Param4 : 没用
	class CustomItemWidgetMeshZonesClass; 				        //type  4

    //Param1 : label的text
    //Param2 : 边界类型        
    //Param3 : label的鼠标悬浮text  
    //Param4 : 没用
	class CustomItemWidgetBoundaryConditionsClass;              //type  5

    //Param1 : label的text
    //Param2 : 是否显示左侧两个按钮，0隐藏，其他数字显示  
    //Param3 : 没用
    //Param4 : 没用
	class CustomItemWidgetCellZonesClass;			            //type  6
};
QT_END_NAMESPACE

class CustomItemWidget : public QWidget {
    Q_OBJECT
public:
    explicit CustomItemWidget(
        int styleIndex,
        QWidget* parent = nullptr, 
        const QString& Param1 = QString(),
        const QString& Param2 = QString(),
		const QString& Param3 = QString(),
		const QString& Param4 = QString()
    );

    ~CustomItemWidget();

    bool eventFilter(QObject* obj, QEvent* event) override;
    void initializeUI(
        int styleIndex, 
        const QString& text, 
        const QString& Param2 = QString(),
		const QString& Param3 = QString(),
		const QString& Param4 = QString()
    );
    void beginEdit(QLabel* label);
    void finishEdit();

signals:
    void textChanged(CustomItemWidget* widget, QString previousText);
	void typeChanged(CustomItemWidget* widget, int previousIndex);
	void optionChanged(CustomItemWidget* widget, int previousIndex);

public:
    Ui::CustomItemWidgetMeshBoundaries1Class* ui_ItemWidgetMeshBoundaries1; 
    Ui::CustomItemWidgetMeshBoundaries2Class* ui_ItemWidgetMeshBoundaries2;
    Ui::CustomItemWidgetMeshRegions1Class* ui_ItemWidgetMeshRegions1;
    Ui::CustomItemWidgetMeshRegions2Class* ui_ItemWidgetMeshRegions2;
	Ui::CustomItemWidgetMeshZonesClass* ui_ItemWidgetMeshZones;
	Ui::CustomItemWidgetBoundaryConditionsClass* ui_ItemWidgetBoundaryConditions;
	Ui::CustomItemWidgetCellZonesClass* ui_ItemWidgetCellZones;

    QLineEdit* lineEdit;
    bool editing;
    QString Param1;
    QString Param2;
	QString Param3;
	QString Param4;

    int previousTypeIndex;
	int previousOptionIndex;
};
