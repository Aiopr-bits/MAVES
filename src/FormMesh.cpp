#include "FormMesh.h"
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QStandardItem>
#include <QRandomGenerator>

FormMesh::FormMesh(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormMeshClass())
    , treeViewModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    ui->treeView->setModel(treeViewModel);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->pushButton, &QPushButton::clicked, this, &FormMesh::on_pushButton_clicked);
    connect(ui->treeView, &QTreeView::clicked, this, &FormMesh::on_treeView_itemClicked);

}

FormMesh::~FormMesh()
{
	delete ui;
}

void FormMesh::on_treeView_itemClicked(const QModelIndex& index)
{
    QStandardItem* item = treeViewModel->itemFromIndex(index);
    if (item)
    {
        Qt::CheckState newState = (item->checkState() == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
        item->setCheckState(newState);
    }
}

void FormMesh::updateForm()
{
    // 获取全局数据实例
    const auto& meshFaceActors = GlobalData::getInstance().getCaseData()->meshFaceActors;

    // 清空现有的行
    treeViewModel->clear();

    // 遍历 meshFaceActors 并添加到 treeView 中
    for (const auto& actor : meshFaceActors)
    {
        QStandardItem* item = new QStandardItem(actor.first);
		item->setCheckable(true);
        item->setCheckState(Qt::Checked);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		item->setSizeHint(QSize(0, 40));
		treeViewModel->appendRow(item);
    }
}

void FormMesh::on_pushButton_clicked()
{
	emit meshVisibilityChanged();
}



