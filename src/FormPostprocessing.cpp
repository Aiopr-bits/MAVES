#include "FormPostprocessing.h"
#include <QSvgRenderer>
#include <QPixmap>
#include <QPainter>
#include <QDir>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkLookupTable.h>
#include <QThread>

FormPostprocessing::FormPostprocessing(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::FormPostprocessingClass())
{
	ui->setupUi(this);

	treeViewModel = new QStandardItemModel(this);
	ui->treeView->setModel(treeViewModel);
	ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

	// 创建并设置 pushButtonLoopPlayTimerPause
	pushButtonLoopPlayTimerPause = new CustomHoverPushButton(this);
	pushButtonLoopPlayTimerPause->setToolTip("停止播放");
	pushButtonLoopPlayTimerPause->setStyleSheet(
		"QPushButton {"
		"    background-color: rgb(255, 255, 255);"
		"    border: none;"
		"}"
	);
	pushButtonLoopPlayTimerPause->setFixedSize(40, 40);
	QIcon icon("../res/pqVcrPause.png");
	pushButtonLoopPlayTimerPause->setIcon(icon);
	pushButtonLoopPlayTimerPause->setIconSize(QSize(32, 32));
	ui->horizontalLayout_6->insertWidget(6, pushButtonLoopPlayTimerPause);
	pushButtonLoopPlayTimerPause->hide();

	// 创建并设置 pushButtonPlayTimerPause
	pushButtonPlayTimerPause = new CustomHoverPushButton(this);
	pushButtonPlayTimerPause->setToolTip("停止播放");
	pushButtonPlayTimerPause->setStyleSheet(
		"QPushButton {"
		"    background-color: rgb(255, 255, 255);"
		"    border: none;"
		"}"
	);
	pushButtonPlayTimerPause->setFixedSize(40, 40);
	pushButtonPlayTimerPause->setIcon(icon);
	pushButtonPlayTimerPause->setIconSize(QSize(32, 32));
	ui->horizontalLayout_6->insertWidget(3, pushButtonPlayTimerPause);
	pushButtonPlayTimerPause->hide();

	// 创建并设置 pushButtonreverseTimerPause
	pushButtonReverseTimerPause = new CustomHoverPushButton(this);
	pushButtonReverseTimerPause->setToolTip("停止播放");
	pushButtonReverseTimerPause->setStyleSheet(
		"QPushButton {"
		"    background-color: rgb(255, 255, 255);"
		"    border: none;"
		"}"
	);
	pushButtonReverseTimerPause->setFixedSize(40, 40);
	pushButtonReverseTimerPause->setIcon(icon);
	pushButtonReverseTimerPause->setIconSize(QSize(32, 32));
	ui->horizontalLayout_6->insertWidget(2, pushButtonReverseTimerPause);
	pushButtonReverseTimerPause->hide();

	connect(ui->treeView, &QTreeView::clicked, this, &FormPostprocessing::on_treeView_itemClicked);

	//鼠标抬起
	connect(pushButtonPlayTimerPause, &QPushButton::released, this, &FormPostprocessing::on_pushButtonPlayTimerPause_clicked);
	connect(pushButtonReverseTimerPause, &QPushButton::released, this, &FormPostprocessing::on_pushButtonReverseTimerPause_clicked);
	connect(pushButtonLoopPlayTimerPause, &QPushButton::released, this, &FormPostprocessing::on_pushButtonLoopPlayTimerPause_clicked);
}

FormPostprocessing::~FormPostprocessing()
{
	delete ui;
}

void FormPostprocessing::on_pushButton_2_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || treeViewModel->rowCount() == 0) {
		return;
	}

	emit apply();
}

void FormPostprocessing::on_treeView_itemClicked(const QModelIndex& index)
{
	QStandardItem* item = treeViewModel->itemFromIndex(index);
	if (item)
	{
		Qt::CheckState newState = (item->checkState() == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
		item->setCheckState(newState);
	}
}

void FormPostprocessing::on_pushButton_3_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || treeViewModel->rowCount() == 0) {
		return;
	}
	emit firstFrame();
}

void FormPostprocessing::on_pushButton_4_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || treeViewModel->rowCount() == 0) {
		return;
	}
	emit previousFrame();
}

void FormPostprocessing::on_pushButton_5_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || treeViewModel->rowCount() == 0) {
		return;
	}
	emit reverse();
}

void FormPostprocessing::on_pushButton_6_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || treeViewModel->rowCount() == 0) {
		return;
	}
	emit play();
}

void FormPostprocessing::on_pushButton_7_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || treeViewModel->rowCount() == 0) {
		return;
	}
	emit nextFrame();
}

void FormPostprocessing::on_pushButton_8_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || treeViewModel->rowCount() == 0) {
		return;
	}
	emit lastFrame();
}

void FormPostprocessing::on_pushButton_9_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || treeViewModel->rowCount() == 0) {
		return;
	}
	emit loopPlay();
}

void FormPostprocessing::on_pushButtonPlayTimerPause_clicked()
{
	emit playPause();
}

void FormPostprocessing::on_pushButtonReverseTimerPause_clicked()
{
	emit reversePause();
}

void FormPostprocessing::on_pushButtonLoopPlayTimerPause_clicked()
{
	emit loopPlayPause();
}
