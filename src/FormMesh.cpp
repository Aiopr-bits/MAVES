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

    // �������� item ���ܸ߶�
    int totalHeight = 0;
    for (int i = 0; i < listViewModel->rowCount(); ++i) {
        totalHeight += ui->listView->sizeHintForRow(i);
    }

    // ���� QListView �ĸ߶�
    ui->listView->setFixedHeight(totalHeight + 2 * ui->listView->frameWidth());
}

void FormMesh::on_pushButton_clicked()
{
    emit meshVisibilityChanged();
}
