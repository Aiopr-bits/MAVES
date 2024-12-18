#include "WorkSpaceWindow.h"
#include <QScreen>
#include <QStandardItemModel>

WorkSpaceWindow::WorkSpaceWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WorkSpaceWindowClass())
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);

    //label
    QPalette palette = ui->label_2->palette();
    palette.setColor(QPalette::WindowText, Qt::gray);
    ui->label_2->setPalette(palette);
    ui->label_6->setPalette(palette);

    //treeview
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setColumnCount(1);
    QStandardItem* parentItem = new QStandardItem("工作区");
    model->appendRow(parentItem);
    for (int i = 0; i < 3; ++i) {
        QStandardItem* subItem = new QStandardItem(QString("案例%1").arg(i + 1));
        parentItem->appendRow(subItem);
    }
    ui->treeView->setModel(model);
    ui->treeView->setUniformRowHeights(true);
    ui->treeView->setIndentation(0);
    ui->treeView->setFocusPolicy(Qt::NoFocus);
    ui->treeView->setFrameStyle(QFrame::NoFrame);
    ui->treeView->expandAll();
    ui->treeView->setAnimated(true);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->treeView, &QTreeView::clicked, this, [&](const QModelIndex& index) {
        if (ui->treeView->isExpanded(index))
            ui->treeView->collapse(index);
        else
            ui->treeView->expand(index);
        });
}

WorkSpaceWindow::~WorkSpaceWindow()
{
    delete ui;
}
