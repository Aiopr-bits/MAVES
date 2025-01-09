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
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->pushButton, &QPushButton::clicked, this, &FormMesh::on_pushButton_clicked);
    connect(ui->listView, &QListView::clicked, this, &FormMesh::on_listView_itemClicked);

}

FormMesh::~FormMesh()
{
	delete ui;
}

void FormMesh::on_listView_itemClicked(const QModelIndex& index)
{
    QStandardItem* item = listViewModel->itemFromIndex(index);
    if (item)
    {
        Qt::CheckState newState = (item->checkState() == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
        item->setCheckState(newState);
    }
}

void FormMesh::updateForm()
{
    // ��ȡȫ������ʵ��
    const auto& meshFaceActors = GlobalData::getInstance().getCaseData()->meshFaceActors;

    // ������е���
    listViewModel->clear();

    // ���� meshFaceActors ����ӵ� listView ��
    for (const auto& actor : meshFaceActors)
    {
        QStandardItem* item = new QStandardItem(actor.first);
		item->setCheckable(true);
        item->setCheckState(Qt::Checked);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		item->setSizeHint(QSize(0, 40));
		listViewModel->appendRow(item);
    }
}

void FormMesh::on_pushButton_clicked()
{
	emit meshVisibilityChanged();
}



