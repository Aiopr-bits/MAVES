#include "FormPostprocessing.h"
#include <QSvgRenderer>
#include <QPixmap>
#include <QPainter>
#include <QDir>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <vtkCellData.h>

FormPostprocessing::FormPostprocessing(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormPostprocessingClass())
{
    ui->setupUi(this);
    GlobalData::getInstance().getCaseData()->casePath = "C:\\Data\\kaisihaopeng\\OpenFOAM-2406\\OF2406\\j20-u=680\\case.foam";

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
	if (!QDir(caseDirPath + "/VTK").exists())return;

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
		item->setCheckState(Qt::Checked);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		item->setSizeHint(QSize(0, 40));
		treeViewModel->appendRow(item);
	}

	// 初始化三维容器
	GlobalData::getInstance().getCaseData()->boundaryActors.clear();
	for (double time : times)
	{
		GlobalData::getInstance().getCaseData()->boundaryActors[time] = std::map<QString, std::map<QString, vtkSmartPointer<vtkActor>>>();
		for (const QString& variableName : variableNames)
		{
			GlobalData::getInstance().getCaseData()->boundaryActors[time][variableName] = std::map<QString, vtkSmartPointer<vtkActor>>();
			foreach(const QString & vtpFileName, vtpFileNames)
			{
				QString vtpFilePath;
				if (vtpFileName == "internal") {
					vtpFilePath = caseDirPath + "/VTK/" + caseDirName + "_"+ QString::number(time) + "/internal.vtu";
				}
				else {
					vtpFilePath = caseDirPath + "/VTK/" + caseDirName + "_" + QString::number(time) + "/boundary/" + vtpFileName + ".vtp";
				}

				vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();




				GlobalData::getInstance().getCaseData()->boundaryActors[time][variableName][vtpFileName] = actor;
			}
		}
	}





}

void FormPostprocessing::on_pushButton_clicked()
{
	loadResultData();
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