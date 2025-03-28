﻿/*--------------------------------*- C++ -*----------------------------------*\
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

QT_BEGIN_NAMESPACE
namespace Ui { class FormMeshClass; };
QT_END_NAMESPACE

class FormMesh : public QWidget
{
    Q_OBJECT

public:
    FormMesh(QWidget* parent = nullptr);
    ~FormMesh();

	QListView* createBoundariesListView(std::string regionName,
        std::vector<std::string> patchNames);

    void split(const std::string& s, char delimiter,
        std::vector<std::string>& tokens);

    std::unordered_map<std::string, std::vector<std::string>> 
        analysismeshPatchNames(const std::vector<std::string>& meshPatchNames);

    void getMeshPatchData(const std::string& casePath); 								//获取网格patch数据

public slots:
    void updateForm(bool isRender);
    void onListViewClicked(const QModelIndex& index);
    void on_pushButton_clicked();
    void onItemEntered(const QString& text);
    void onItemExited(const QString& text);
    void onToggleRegionSecondAnimation();

signals:
    void apply(std::vector<QListView*> listViewBoundaries);
	void itemEntered(const QString& text);
	void itemExited(const QString& text);
    void toggleRegionSecondAnimation();
	void updateFormFinished();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

public:
    Ui::FormMeshClass* ui;
    QStandardItemModel* listViewModel;
    QModelIndex lastIndex;
	std::vector<QListView*> listViewBoundaries;
};
