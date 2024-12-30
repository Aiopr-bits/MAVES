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

    //隐藏加载数据按钮
	ui->pushButton->hide();

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

    connect(ui->pushButton, &QPushButton::clicked, this, &FormPostprocessing::on_pushButton_clicked);
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

void FormPostprocessing::loadResultData()
{
    QString caseFilePath = QString::fromStdString(GlobalData::getInstance().getCaseData()->casePath);
    QFileInfo fileInfo(caseFilePath);
    QString caseDirPath = fileInfo.absolutePath();
    QString caseDirName = fileInfo.dir().dirName();
    if (!QDir(caseDirPath + "/VTK").exists()) return;

    //更新comboBox
    QDir dir(caseDirPath + "/VTK");
    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    std::vector<double> times;
    foreach(QString folder, folders)
    {
        QStringList list = folder.split("_");

        if (list.size() > 1)
        {
            QString time = list.last();
            if (time.toDouble() != 0) times.push_back(time.toDouble());
        }
    }
    if (times.empty()) return;
    std::sort(times.begin(), times.end());

    ui->comboBox->clear();
    foreach(double time, times)
    {
        ui->comboBox->addItem(QString::number(time));
    }
	ui->comboBox->setCurrentIndex(ui->comboBox->count() - 1);

    //更新comboBox_2
    QString vtuFilePath = caseDirPath + "/VTK/" + caseDirName + "_0/internal.vtu";
    vtkSmartPointer<vtkXMLUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader->SetFileName(vtuFilePath.toStdString().c_str());
    reader->Update();

    vtkUnstructuredGrid* unstructuredGrid = reader->GetOutput();
    vtkPointData* pointData = unstructuredGrid->GetPointData();
    vtkCellData* cellData = unstructuredGrid->GetCellData();

    std::vector<QString> variableNames;
    for (int i = 0; i < pointData->GetNumberOfArrays(); ++i)
    {
        variableNames.push_back(pointData->GetArrayName(i));
    }

    ui->comboBox_2->clear();
    foreach(QString variableName, variableNames)
    {
        ui->comboBox_2->addItem(variableName);
    }

    //更新 treeView
    QString vtpDirPath = caseDirPath + "/VTK/" + caseDirName + "_0/boundary/";
    QDir vtpDir(vtpDirPath);
    QStringList vtpFiles = vtpDir.entryList(QStringList() << "*.vtp", QDir::Files);

    std::vector<QString> vtpFileNames;
    vtpFileNames.push_back("internal");
    foreach(QString vtpFile, vtpFiles)
    {
        QFileInfo vtpFileInfo(vtpFile);
        vtpFileNames.push_back(vtpFileInfo.baseName());
    }

    treeViewModel->clear();
    foreach(QString vtpFileName, vtpFileNames)
    {
        QStandardItem* item = new QStandardItem(vtpFileName);
        item->setCheckable(true);
		if (vtpFileName == "internal")item->setCheckState(Qt::Checked);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setSizeHint(QSize(0, 40));
        treeViewModel->appendRow(item);
    }

    GlobalData::getInstance().getCaseData()->times = times;
    GlobalData::getInstance().getCaseData()->variableNames = variableNames;
    GlobalData::getInstance().getCaseData()->meshPartName = vtpFileNames;

    emit resultDataLoaded();
}

void FormPostprocessing::on_pushButton_clicked()
{
    loadResultData();
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
