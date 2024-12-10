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

    //// ������ѡ�򲢽������
    //QCheckBox* checkBox = new QCheckBox(this);
    //QWidget* checkBoxWidget = new QWidget(this);
    //QHBoxLayout* layout = new QHBoxLayout(checkBoxWidget);
    //layout->addWidget(checkBox);
    //layout->setAlignment(checkBox, Qt::AlignCenter);
    //layout->setContentsMargins(0, 0, 0, 0);
    //checkBoxWidget->setLayout(layout);

    //// �����в����ø�ѡ��С����
    //ui->tableWidget->insertRow(0);
    //ui->tableWidget->setCellWidget(0, 0, checkBoxWidget);

}

FormMesh::~FormMesh()
{
	delete ui;
}

void FormMesh::updateForm()
{
    // ��ȡȫ������ʵ��
    const auto& meshBoundaryActors = GlobalData::getInstance().getCaseData()->meshBoundaryActors;

    // ������е���
    ui->tableWidget->setRowCount(0);

    // ���� meshBoundaryActors ����ӵ� tableWidget ��
    int row = 0;
    for (const auto& actor : *meshBoundaryActors)
    {
        // ��������
        ui->tableWidget->insertRow(row);

        // ������ѡ������Ϊѡ��״̬
        QCheckBox* checkBox = new QCheckBox(this);
        checkBox->setChecked(true);
        QWidget* checkBoxWidget = new QWidget(this);
        QHBoxLayout* layout = new QHBoxLayout(checkBoxWidget);
        layout->addWidget(checkBox);
        layout->setAlignment(checkBox, Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);
        checkBoxWidget->setLayout(layout);
        ui->tableWidget->setCellWidget(row, 0, checkBoxWidget);

        // ������ǩ�������ı�
        QLabel* label = new QLabel(actor.first, this);
        ui->tableWidget->setCellWidget(row, 1, label);

        ++row;
    }
}

