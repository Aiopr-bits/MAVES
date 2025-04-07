/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormMesh.h"
#include "GlobalData.h"
#include <QStandardItemModel.h>
#include "CustomCheckBoxDelegate.h"
#include <QPropertyAnimation.h>
#include <QThread.h>
#include <QSequentialAnimationGroup>
#include <vtkOpenFOAMReader.h>
#include <sstream>
#include <vtkCompositeDataSet.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkInformation.h>
#include "CustomItemWidget.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>

// 如果是 C++14，启用 experimental/filesystem
#if __cplusplus < 201703L
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class FormMeshClass; };
QT_END_NAMESPACE

class FormMesh : public QWidget
{
    Q_OBJECT

public:
    FormMesh(QWidget* parent = nullptr);
    ~FormMesh();

    void getRegionTypes(const std::string& casePath);   						        //获取区域类型
    void getPatchTypes(const std::string& casePath); 								    //获取网格patch类型
	void getCellZoneNames(const std::string& casePath); 								//获取cell zone名称

public slots:
    void resizeEvent(QResizeEvent* event);
    void on_tabWidget_currentChanged(int index);
    void updateForm(bool isRender);
    void onSelectionChanged();
	void on_pushButton_clicked();

signals:
	void apply();                                                                        //应用

public:
    Ui::FormMeshClass* ui;
    int previousIndex;

     //以下可能删除
	QListView* createBoundariesListView(std::string regionName,
        std::vector<std::string> patchNames);

    void split(const std::string& s, char delimiter,
        std::vector<std::string>& tokens);

    std::unordered_map<std::string, unordered_map<std::string, std::string>>
        analysismeshPatchNames(const std::vector<std::string>& meshPatchNames);

public slots:
    void onListViewClicked(const QModelIndex& index);
    void onItemEntered(const QString& text);
    void onItemExited(const QString& text);
    void onToggleRegionSecondAnimation();

signals:
    //void apply(std::vector<QListView*> listViewBoundaries);
	void itemEntered(const QString& text);
	void itemExited(const QString& text);
    void toggleRegionSecondAnimation();
	void updateFormFinished();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

public:
    QStandardItemModel* listViewModel;
    QModelIndex lastIndex;
	std::vector<QListView*> listViewBoundaries;
};
