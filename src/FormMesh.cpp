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
    ui->pushButton->hide();

    ui->treeView->setModel(treeViewModel);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

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
    treeViewModel->clear();

    // ���� meshFaceActors ����ӵ� treeView ��
    for (const auto& actor : *meshFaceActors)
    {
        QStandardItem* item = new QStandardItem(actor.first);
		item->setCheckable(true);
        item->setCheckState(Qt::Checked);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		item->setSizeHint(QSize(0, 40));
		treeViewModel->appendRow(item);
    }
    ui->pushButton->show();
}

void FormMesh::on_pushButton_clicked()
{
    // ��ȡȫ������ʵ��
    const auto& meshFaceActors = GlobalData::getInstance().getCaseData()->meshFaceActors;
    // ���� treeView ������ actor �������ߵĿɼ���
    for (int i = 0; i < treeViewModel->rowCount(); ++i)
    {
        QStandardItem* item = treeViewModel->item(i);
        const auto& actor = meshFaceActors->find(item->text());
        if (actor != meshFaceActors->end())
        {
            bool isVisible = (item->checkState() == Qt::Checked);
            actor->second->SetVisibility(isVisible);
            //actor->second->SetGridVisibility(isVisible); // ���������ߵĿɼ���
        }
    }
}



