#include "FormMesh.h"
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QStandardItem>
#include <QRandomGenerator>

FormMesh::FormMesh(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormMeshClass())
{
    ui->setupUi(this);

    //// 创建复选框并将其居中
    //QCheckBox* checkBox = new QCheckBox(this);
    //QWidget* checkBoxWidget = new QWidget(this);
    //QHBoxLayout* layout = new QHBoxLayout(checkBoxWidget);
    //layout->addWidget(checkBox);
    //layout->setAlignment(checkBox, Qt::AlignCenter);
    //layout->setContentsMargins(0, 0, 0, 0);
    //checkBoxWidget->setLayout(layout);

    //// 插入行并设置复选框小部件
    //ui->tableWidget->insertRow(0);
    //ui->tableWidget->setCellWidget(0, 0, checkBoxWidget);

}

FormMesh::~FormMesh()
{
	delete ui;
}

void FormMesh::updateForm()
{
    // 获取全局数据实例
    const auto& meshBoundaryActors = GlobalData::getInstance().getCaseData()->meshBoundaryActors;

    // 清空现有的行
    ui->tableWidget->setRowCount(0);

    // 遍历 meshBoundaryActors 并添加到 tableWidget 中
    int row = 0;
    for (const auto& actor : *meshBoundaryActors)
    {
        // 插入新行
        ui->tableWidget->insertRow(row);

        // 创建复选框并设置为选中状态
        QCheckBox* checkBox = new QCheckBox(this);
        checkBox->setChecked(true);
        QWidget* checkBoxWidget = new QWidget(this);
        QHBoxLayout* layout = new QHBoxLayout(checkBoxWidget);
        layout->addWidget(checkBox);
        layout->setAlignment(checkBox, Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);
        checkBoxWidget->setLayout(layout);
        ui->tableWidget->setCellWidget(row, 0, checkBoxWidget);

        // 创建标签并设置文本
        QLabel* label = new QLabel(actor.first, this);
        ui->tableWidget->setCellWidget(row, 1, label);

        ++row;
    }
}

