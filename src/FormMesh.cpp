#include "FormMesh.h"
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QStandardItem>
#include <QRandomGenerator>
#include <qdebug.h>

FormMesh::FormMesh(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormMeshClass())
    , listViewModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    ui->listView->setModel(listViewModel);
    ui->listView->setMouseTracking(true);
    ui->listView->viewport()->installEventFilter(this);

    CustomCheckBoxDelegate* delegate = new CustomCheckBoxDelegate(this);
    ui->listView->setItemDelegate(delegate);

    connect(ui->pushButton, &QPushButton::clicked, this, &FormMesh::on_pushButton_clicked);
}

FormMesh::~FormMesh()
{
    delete ui;
}

void FormMesh::updateForm()
{
    // ��ȡȫ������ʵ���е� meshPatchActors
    const auto& meshPatchActors = GlobalData::getInstance().getCaseData()->meshPatchActors;

    // ������е���
    listViewModel->clear();

    // ���� meshPatchActors ����ӵ� listView ��
    for (const auto& actor : meshPatchActors)
    {
        QString actorName = QString::fromStdString(actor.first);
        QStandardItem* item = new QStandardItem(actorName);
        item->setCheckable(true);
        if(actorName == "internalMesh") item->setCheckState(Qt::Checked);
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

    on_pushButton_clicked();
}

void FormMesh::on_pushButton_clicked()
{
    emit meshVisibilityChanged();
}

void FormMesh::onItemEntered(const QString& text)
{
	//emit itemEntered(text);
}

void FormMesh::onItemExited(const QString& text)
{
	//emit itemExited(text);
}

bool FormMesh::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->listView->viewport())
    {
        if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint pos = mouseEvent->pos();
            QModelIndex index = ui->listView->indexAt(pos);

            if (index.isValid())
            {
                if (index != lastIndex)
                {
                    if (lastIndex.isValid())
                    {
                        onItemExited(lastIndex.data(Qt::DisplayRole).toString());
                    }
                    onItemEntered(index.data(Qt::DisplayRole).toString());
                    lastIndex = index;
                }
            }
            else
            {
                if (lastIndex.isValid())
                {
                    onItemExited(lastIndex.data(Qt::DisplayRole).toString());
                    lastIndex = QModelIndex();
                }
            }
            return false; // �������������¼�
        }
        else if (event->type() == QEvent::Leave)
        {
            if (lastIndex.isValid())
            {
                onItemExited(lastIndex.data(Qt::DisplayRole).toString());
                lastIndex = QModelIndex();
            }
            return false;
        }
    }
    return QWidget::eventFilter(watched, event);
}
