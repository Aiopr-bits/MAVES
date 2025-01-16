#include "FormMesh.h"
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QStandardItem>
#include <QRandomGenerator>

FormMesh::FormMesh(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormMeshClass())
    , listViewModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    ui->listView->setModel(listViewModel);

    ui->listView->setItemDelegate(new CustomCheckBoxDelegate(this));
    connect(ui->pushButton, &QPushButton::clicked, this, &FormMesh::on_pushButton_clicked);
}

FormMesh::~FormMesh()
{
    delete ui;
}

void FormMesh::updateForm()
{
    // 获取全局数据实例
    const auto& meshFaceActors = GlobalData::getInstance().getCaseData()->meshFaceActors;

    // 清空现有的行
    listViewModel->clear();

    // 遍历 meshFaceActors 并添加到 listView 中
    for (const auto& actor : meshFaceActors)
    {
        QStandardItem* item = new QStandardItem(actor.first);
        item->setCheckable(true);
        item->setCheckState(Qt::Checked);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setSizeHint(QSize(0, 40));
        listViewModel->appendRow(item);
    }

    // 计算所有 item 的总高度
    int totalHeight = 0;
    for (int i = 0; i < listViewModel->rowCount(); ++i) {
        totalHeight += ui->listView->sizeHintForRow(i);
    }

    // 设置 QListView 的高度
    ui->listView->setFixedHeight(totalHeight + 2 * ui->listView->frameWidth());
}

void FormMesh::on_pushButton_clicked()
{
    emit meshVisibilityChanged();
}
