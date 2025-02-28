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

QT_BEGIN_NAMESPACE
namespace Ui { class FormMeshClass; };
QT_END_NAMESPACE

class FormMesh : public QWidget
{
    Q_OBJECT

public:
    FormMesh(QWidget* parent = nullptr);
    ~FormMesh();

public slots:
    void updateForm();
    void on_pushButton_clicked();
    void onItemEntered(const QString& text);
    void onItemExited(const QString& text);

signals:
    void meshVisibilityChanged();
	void itemEntered(const QString& text);
	void itemExited(const QString& text);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

public:
    Ui::FormMeshClass* ui;
    QStandardItemModel* listViewModel;
    QModelIndex lastIndex;
};
