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

FormPostprocessing::FormPostprocessing(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormPostprocessingClass())
{
    ui->setupUi(this);
    GlobalData::getInstance().getCaseData()->casePath = "C:\\Data\\kaisihaopeng\\OpenFOAM-2406\\OF2406\\j20-s\\case.foam";

    treeViewModel = new QStandardItemModel(this);
    ui->treeView->setModel(treeViewModel);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->pushButton, &QPushButton::clicked, this, &FormPostprocessing::on_pushButton_clicked);
    connect(ui->treeView, &QTreeView::clicked, this, &FormPostprocessing::on_treeView_itemClicked);
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
            times.push_back(time.toDouble());
        }
    }
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
	emit firstFrame();
}

void FormPostprocessing::on_pushButton_4_clicked()
{
	emit previousFrame();
}

void FormPostprocessing::on_pushButton_5_clicked()
{
	emit reverse();
}

void FormPostprocessing::on_pushButton_6_clicked()
{
	emit play();
}

void FormPostprocessing::on_pushButton_7_clicked()
{
	emit nextFrame();
}

void FormPostprocessing::on_pushButton_8_clicked()
{
	emit lastFrame();
}

void FormPostprocessing::on_pushButton_9_clicked()
{
	emit loopPlay();
}
