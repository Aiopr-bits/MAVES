/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#include "WorkSpaceWindow.h"
#include <QScreen>
#include <QStandardItemModel>
#include <QMessageBox>

WorkSpaceWindow::WorkSpaceWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WorkSpaceWindowClass())
	, mainWindow(this)
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

    connect(ui->pushButton_3, &CustomDoubleClickPushButton::doubleClicked, this, &WorkSpaceWindow::on_PushButton_3_DoubleClicked);
}

WorkSpaceWindow::~WorkSpaceWindow()
{
    delete ui;
}

void WorkSpaceWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    QSize buttonSize = ui->pushButton_3->size();
    QIcon icon = ui->pushButton_3->icon();
    QSize iconSize(buttonSize.width() * 0.8, buttonSize.height() * 0.8); 
    icon.actualSize(iconSize);
    ui->pushButton_3->setIconSize(iconSize);
}

void WorkSpaceWindow::on_PushButton_3_DoubleClicked()
{
	mainWindow.show();
	this->hide(); 
}