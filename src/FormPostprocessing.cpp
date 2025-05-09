﻿/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

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

	listViewModel = new QStandardItemModel(this);
	ui->listView->setModel(listViewModel);
	ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);

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

	//鼠标抬起
	connect(pushButtonPlayTimerPause, &QPushButton::released, this, &FormPostprocessing::on_pushButtonPlayTimerPause_clicked);
	connect(pushButtonReverseTimerPause, &QPushButton::released, this, &FormPostprocessing::on_pushButtonReverseTimerPause_clicked);
	connect(pushButtonLoopPlayTimerPause, &QPushButton::released, this, &FormPostprocessing::on_pushButtonLoopPlayTimerPause_clicked);

	connect(ui->listView, &QListView::clicked, this, &FormPostprocessing::onListViewClicked);
	connect(this, &FormPostprocessing::toggleRegionSecondAnimation, this, &FormPostprocessing::onToggleRegionSecondAnimation);
}

FormPostprocessing::~FormPostprocessing()
{
	delete ui;
}

void FormPostprocessing::updateForm()
{
	std::string casePath = GlobalData::getInstance().getCaseData()->casePath;
	getNephogramPatchData(casePath);
	std::unordered_map<std::string, unordered_map<std::string, std::string>> patchType = GlobalData::getInstance().getCaseData()->patchType;

	this->ui->comboBox->clear();
	this->ui->comboBox_2->clear();
	this->listViewModel->clear();

	// 更新时间步和物理量下拉框
	QStringList timeStepList;
	std::vector<double> timeSteps = GlobalData::getInstance().getCaseData()->timeSteps;
	for (const double& timeStep : timeSteps) timeStepList.append(QString::number(timeStep));
	if (timeStepList.size() == 0) return;
	this->ui->comboBox->addItems(timeStepList);

	// 更新物理量下拉框
	QStringList fieldNameList;
	std::vector<std::string> fieldName = GlobalData::getInstance().getCaseData()->fieldName;
	for (const std::string& field : fieldName) fieldNameList.append(QString::fromStdString(field));
	if (fieldNameList.size() == 0) return;
	this->ui->comboBox_2->addItems(fieldNameList);

	// 遍历 meshPatchNamesMap 并将 key 值添加到 listView 中
	QIcon regionIcon("..\\res\\region.png");
	for (const auto& pair : patchType)
	{
		QString actorName = QString::fromStdString(pair.first);
		QStandardItem* item = new QStandardItem(actorName);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		item->setSizeHint(QSize(0, 30));
		item->setIcon(regionIcon);
		listViewModel->appendRow(item);
	}

	// 设置 QListView 的高度
	int totalHeight = 0;
	for (int i = 0; i < listViewModel->rowCount(); ++i) {
		totalHeight += ui->listView->sizeHintForRow(i);
	}
	ui->listView->setFixedHeight(totalHeight + 2 * ui->listView->frameWidth());

	// 将数据复制到 vector 并反转，以模拟逆序遍历
	std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> reversedMeshPatch;
	reversedMeshPatch.reserve(patchType.size());
	for (const auto& kv : patchType)
	{
		std::vector<std::pair<std::string, std::string>> patchNames;
		for (const auto& patch : kv.second)
		{
			patchNames.push_back(std::make_pair(patch.first, patch.second));
		}
		reversedMeshPatch.push_back(std::make_pair(kv.first, patchNames));
	}
	std::reverse(reversedMeshPatch.begin(), reversedMeshPatch.end());

	//清除释放listViewBoundaries
	for (auto& listView : listViewBoundaries)
	{
		delete listView;
	}
	listViewBoundaries.clear();

	// 创建 patch 的 listView
	for (const auto& kv : reversedMeshPatch)
	{
		std::vector<std::string> patchNames;
		for (const auto& patch : kv.second)
		{
			patchNames.push_back(patch.first);
		}
		QListView* listView = createBoundariesListView(kv.first, patchNames);
		ui->verticalLayout->insertWidget(1, listView);
		listViewBoundaries.push_back(listView);
	}

	// 点击 ui->listView 的第一个 item
	QModelIndex index = listViewModel->index(0, 0);
	ui->listView->setCurrentIndex(index);
	onListViewClicked(index);

	emit updateFormFinished();
}

void FormPostprocessing::split(const std::string& s, char delimiter, std::vector<std::string>& tokens) {
	// 如果s以/开头，删除第一个字符
	std::string str = s;
	if (str[0] == delimiter) {
		str.erase(str.begin());
	}

	std::string token;
	std::istringstream tokenStream(str);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
}

std::unordered_map<std::string, unordered_map<std::string, std::string>> FormPostprocessing::analysismeshPatchNames(const std::vector<std::string>& meshPatchNames)
{
	std::unordered_map<std::string, unordered_map<std::string, std::string>> patchType;
	for (const auto& name : meshPatchNames)
	{
		std::vector<std::string> tokens;
		split(name, '/', tokens);
		if ((tokens.size() == 2 && tokens[0] == "group") || (tokens.size() == 3 && tokens[1] == "group"))
			continue;

		//区域名称和边界名称
		std::string region, patch;
		if (tokens[0] == "internalMesh" || tokens[0] == "patch") {
			region = "default";
			patch = tokens[tokens.size() - 1];
		}
		else {
			region = tokens[0];
			patch = tokens[tokens.size() - 1];
		}

		patchType[region][patch] = "";
	}

	//将key==default的元素放到第一个
	std::unordered_map<std::string, unordered_map<std::string, std::string>> patchTypeTemp;
	for (const auto& item : patchType) {
		if (item.first == "default") {
			patchTypeTemp.insert(patchTypeTemp.begin(), item);
		}
		else {
			patchTypeTemp.insert(patchTypeTemp.end(), item);
		}
	}
	patchType = patchTypeTemp;

	return patchType;
}

QListView* FormPostprocessing::createBoundariesListView(std::string regionName, std::vector<std::string> patchNames)
{
	QListView* listView = new QListView(this);
	QStandardItemModel* listViewModel = new QStandardItemModel(this);

	listView->setModel(listViewModel);
	listView->setMouseTracking(true);
	listView->viewport()->installEventFilter(this);
	CustomCheckBoxDelegate* delegate = new CustomCheckBoxDelegate(this);
	listView->setItemDelegate(delegate);
	listView->setFocusPolicy(Qt::NoFocus);

	// 设置 QSS 样式
	listView->setStyleSheet(
		"QListView::item {"
		"    padding: 11px;"
		"}"
		"QListView {"
		"    border: none;"
		"}"
		"QListView::item {"
		"    background: #FFFFFF;"
		"}"
		"QListView::item:hover {"
		"    background: #F7F7F7;"
		"}"
		"QListView::item:selected {"
		"    background: #EEEEEE;"
		"}"
		"QListView QScrollBar:vertical {"
		"    border: 1px solid #dcdcdc;"
		"    background: white;"
		"    width: 12px;"
		"    margin: 0px 0px 0px 0px;"
		"}"
		"QListView QScrollBar::handle:vertical {"
		"    background: #bfbfbf;"
		"    min-height: 20px;"
		"    border-radius: 5px;"
		"}"
		"QListView QScrollBar::add-line:vertical {"
		"    background: white;"
		"    height: 0px;"
		"    subcontrol-position: bottom;"
		"    subcontrol-origin: margin;"
		"}"
		"QListView QScrollBar::sub-line:vertical {"
		"    background: white;"
		"    height: 0px;"
		"    subcontrol-position: top;"
		"    subcontrol-origin: margin;"
		"}"
		"QListView QScrollBar::up-arrow:vertical, QListView QScrollBar::down-arrow:vertical {"
		"    border: none;"
		"    width: 0px;"
		"    height: 0px;"
		"    background: none;"
		"}"
		"QListView QScrollBar::add-page:vertical, QListView QScrollBar::sub-page:vertical {"
		"    background: none;"
		"}"
	);

	// 遍历 patchNames 并添加到 listView 中
	for (const auto& patchName : patchNames)
	{
		QString actorName = QString::fromStdString(patchName);
		QStandardItem* item = new QStandardItem(actorName);
		item->setCheckable(true);
		if (actorName.endsWith("internalMesh"))item->setCheckState(Qt::Checked);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		item->setSizeHint(QSize(0, 30));
		listViewModel->appendRow(item);
	}

	// 设置 QListView 的高度
	int totalHeight = 0;
	for (int i = 0; i < listViewModel->rowCount(); ++i) {
		totalHeight += listView->sizeHintForRow(i);
	}
	listView->setFixedHeight(totalHeight + 2 * ui->listView->frameWidth());

	return listView;
}

void FormPostprocessing::getMeshPatchData(const std::string& casePath)
{
	std::vector<std::string> meshPatchNames;

	// 创建 OpenFOAM 读取器
	vtkSmartPointer<vtkOpenFOAMReader> openFOAMReader =
		vtkSmartPointer<vtkOpenFOAMReader>::New();
	openFOAMReader->SetFileName(casePath.c_str());
	openFOAMReader->SetCreateCellToPoint(1);
	openFOAMReader->SetSkipZeroTime(1);

	// 更新信息以获取补丁名称
	openFOAMReader->UpdateInformation();

	// 获取所有补丁名称
	int numPatches = openFOAMReader->GetNumberOfPatchArrays();
	if (numPatches == 0)
	{
		std::cerr << "没有找到任何补丁。" << std::endl;
		return;
	}

	// 禁用所有补丁
	openFOAMReader->DisableAllPatchArrays();

	// 遍历所有补丁
	for (int i = 0; i < numPatches; ++i)
	{
		const char* currentPatchName = openFOAMReader->GetPatchArrayName(i);
		meshPatchNames.push_back(std::string(currentPatchName));
	}

	std::unordered_map<std::string, unordered_map<std::string, std::string>> patchType = analysismeshPatchNames(meshPatchNames);
	GlobalData::getInstance().getCaseData()->patchType = patchType;
}

void FormPostprocessing::getNephogramPatchData(const std::string& casePath)
{
	//获取网格patch数据
	getMeshPatchData(casePath);

	std::map<std::string, std::pair<double, double>> fieldsScalarRange;
	std::vector<double> timeSteps;
	std::vector<std::string> fieldName;

	vtkSmartPointer<vtkOpenFOAMReader> reader = vtkSmartPointer<vtkOpenFOAMReader>::New();
	reader->SetFileName(casePath.c_str());
	reader->SetCreateCellToPoint(1);
	reader->SetSkipZeroTime(1);
	reader->UpdateInformation();

	vtkSmartPointer<vtkDoubleArray> timeValues = reader->GetTimeValues();
	if (!timeValues || timeValues->GetNumberOfValues() == 0) {
		std::cerr << "没有找到可用的时间步。" << std::endl;
		return;
	}

	if (timeValues->GetNumberOfValues() == 1 && timeValues->GetValue(0) == 0) {
		GlobalData::getInstance().getCaseData()->timeSteps = timeSteps;
		GlobalData::getInstance().getCaseData()->fieldName = fieldName;
		GlobalData::getInstance().getCaseData()->fieldsScalarRange = fieldsScalarRange;
		return;
	}

	for (int i = 0; i < timeValues->GetNumberOfValues(); ++i) {
		timeSteps.push_back(timeValues->GetValue(i));
	}

	double lastTime = timeSteps.back();
	reader->SetTimeValue(lastTime);

	//auto meshPatchNamesMap = GlobalData::getInstance().getCaseData()->meshPatchNamesMap;
	std::unordered_map<std::string, unordered_map<std::string, std::string>> patchType = GlobalData::getInstance().getCaseData()->patchType;
	if (patchType.size() > 1) {
		// 多域情况下，对各子域分别进行读取并记录其标量并集
		fieldsScalarRange.clear();
		fieldName.clear();
		std::unordered_set<std::string> fieldNameSet;

		for (const auto& kv : patchType) {
			vtkSmartPointer<vtkOpenFOAMReader> subdomainReader = vtkSmartPointer<vtkOpenFOAMReader>::New();
			subdomainReader->SetFileName(casePath.c_str());
			subdomainReader->SetCreateCellToPoint(1);
			subdomainReader->SetSkipZeroTime(1);
			subdomainReader->UpdateInformation();
			subdomainReader->SetTimeValue(lastTime);

			subdomainReader->DisableAllPatchArrays();
			for (const auto& patch : kv.second) {
				std::string patchPath = "/" + kv.first + "/" + patch.first;
				subdomainReader->SetPatchArrayStatus(patchPath.c_str(), 1);
			}
			subdomainReader->Update();

			vtkSmartPointer<vtkCompositeDataGeometryFilter> geometryFilter =
				vtkSmartPointer<vtkCompositeDataGeometryFilter>::New();
			geometryFilter->SetInputConnection(subdomainReader->GetOutputPort());
			geometryFilter->Update();

			vtkPolyData* polyDataTemp = geometryFilter->GetOutput();
			if (!polyDataTemp) continue;

			int arrayCountTemp = polyDataTemp->GetPointData()->GetNumberOfArrays();
			for (int i = 0; i < arrayCountTemp; ++i) {
				vtkDataArray* arr = polyDataTemp->GetPointData()->GetArray(i);
				if (!arr) continue;
				std::string arrName = arr->GetName() ? arr->GetName() : "";
				double rangeTemp[2];
				arr->GetRange(rangeTemp);

				// 如果是新标量就插入fieldsScalarRange，否则更新其范围
				if (fieldNameSet.find(arrName) == fieldNameSet.end()) {
					fieldsScalarRange[arrName] = std::make_pair(rangeTemp[0], rangeTemp[1]);
					fieldNameSet.insert(arrName);
				} else {
					auto& rangeRef = fieldsScalarRange[arrName];
					rangeRef.first = std::min(rangeRef.first, rangeTemp[0]);
					rangeRef.second = std::max(rangeRef.second, rangeTemp[1]);
				}
			}
		}
		// 将结果写入向量
		for (const auto& fn : fieldNameSet) {
			fieldName.push_back(fn);
		}
	} else {
		// 单域情况
		reader->DisableAllPatchArrays();
		if (patchType.size() > 1) {//多域情况
			for (const auto& kv : patchType) {
				for (const auto& patch : kv.second) {
					if (kv.first != "default"&& patch.first == "internalMesh") {
						std::string patchName = "/" + kv.first + "/" + patch.first;
						reader->SetPatchArrayStatus(patchName.c_str(), 1);
					}
				}
			}
		}
		else if (patchType.size() == 1) {
			reader->SetPatchArrayStatus("internalMesh", 1);
		}
		reader->Update();

		vtkSmartPointer<vtkCompositeDataGeometryFilter> geometryFilter =
			vtkSmartPointer<vtkCompositeDataGeometryFilter>::New();
		geometryFilter->SetInputConnection(reader->GetOutputPort());
		geometryFilter->Update();

		vtkPolyData* polyData = geometryFilter->GetOutput();
		if (!polyData || polyData->GetNumberOfPoints() == 0) {
			std::cerr << "无法提取几何数据。" << std::endl;
			return;
		}

		// 收集标量数组的范围
		int arrayCount = polyData->GetPointData()->GetNumberOfArrays();
		for (int i = 0; i < arrayCount; ++i) {
			vtkDataArray* arr = polyData->GetPointData()->GetArray(i);
			if (!arr) continue;
			std::string arrayName = arr->GetName() ? arr->GetName() : "";
			//if (arrayName != "p" && arrayName != "U" && arrayName != "T")continue;

			double range[2];
			arr->GetRange(range);
			fieldsScalarRange[arrayName] = std::make_pair(range[0], range[1]);
			fieldName.push_back(arrayName);
		}
	}

	GlobalData::getInstance().getCaseData()->timeSteps = timeSteps;
	GlobalData::getInstance().getCaseData()->fieldName = fieldName;
	GlobalData::getInstance().getCaseData()->fieldsScalarRange = fieldsScalarRange;
}

void FormPostprocessing::on_pushButton_2_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || listViewModel->rowCount() == 0) {
		return;
	}

	emit apply(listViewBoundaries);
}

void FormPostprocessing::on_pushButton_3_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || listViewModel->rowCount() == 0) {
		return;
	}
	emit firstFrame();
}

void FormPostprocessing::on_pushButton_4_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || listViewModel->rowCount() == 0) {
		return;
	}
	emit previousFrame();
}

void FormPostprocessing::on_pushButton_5_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || listViewModel->rowCount() == 0) {
		return;
	}
	emit reverse();
}

void FormPostprocessing::on_pushButton_6_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || listViewModel->rowCount() == 0) {
		return;
	}
	emit play();
}

void FormPostprocessing::on_pushButton_7_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || listViewModel->rowCount() == 0) {
		return;
	}
	emit nextFrame();
}

void FormPostprocessing::on_pushButton_8_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || listViewModel->rowCount() == 0) {
		return;
	}
	emit lastFrame();
}

void FormPostprocessing::on_pushButton_9_clicked()
{
	if (ui->comboBox->count() == 0 || ui->comboBox_2->count() == 0 || listViewModel->rowCount() == 0) {
		return;
	}
	emit loopPlay();
}

void FormPostprocessing::on_pushButton_10_clicked()
{
	emit loadData();
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

void FormPostprocessing::onListViewClicked(const QModelIndex& index)
{
	// 获取 ui->widget 的切换前截图
	QPixmap pixmap1 = ui->widget->grab();

	// 页面切换
	int row = index.row();
	for (int i = 0; i < listViewBoundaries.size(); ++i)
	{
		listViewBoundaries[listViewBoundaries.size() - i - 1]->setVisible(i == row);
	}

	// 获取ui->widget 的切换后截图
	QPixmap pixmap2 = ui->widget->grab();

	//第一段动画
	//在pixmap下方拼接一个和当前pixmap大小一样的白色图片
	QPixmap whitePixmap1(pixmap1.size());
	whitePixmap1.fill(Qt::white);
	QPixmap combinedPixmap1(pixmap1.width(), pixmap1.height() * 2);
	QPainter painter1(&combinedPixmap1);
	painter1.drawPixmap(0, 0, pixmap1);
	painter1.drawPixmap(0, pixmap1.height(), whitePixmap1);

	// 创建一个 QLabel 来显示 combinedPixmap1
	QLabel* animationLabel = new QLabel(ui->widget);
	animationLabel->setPixmap(combinedPixmap1);
	animationLabel->setGeometry(
		(ui->widget->width() - combinedPixmap1.width()) / 2,
		(ui->widget->height() - combinedPixmap1.height()) / 2,
		combinedPixmap1.width(),
		combinedPixmap1.height()
	);
	animationLabel->show();

	// 制作动画1
	QPropertyAnimation* animation = new QPropertyAnimation(animationLabel, "geometry");
	animation->setDuration(100);
	animation->setStartValue(
		QRect(
			(ui->widget->width() - combinedPixmap1.width()) / 2,
			0,
			combinedPixmap1.width(),
			combinedPixmap1.height()
		)
	);
	animation->setEndValue(animationLabel->geometry());
	animation->start();
	connect(animation, &QAbstractAnimation::finished, this, [=]() {
		delete animationLabel;
		delete animation;
		emit toggleRegionSecondAnimation();
		});
}

void FormPostprocessing::onToggleRegionSecondAnimation()
{
	// 获取ui->widget 的切换后截图
	QPixmap pixmap2 = ui->widget->grab();

	//第二段动画
	//在pixmap下方拼接一个和当前pixmap大小一样的白色图片
	QPixmap whitePixmap2(pixmap2.size());
	whitePixmap2.fill(Qt::white);
	QPixmap combinedPixmap2(pixmap2.width(), pixmap2.height() * 2);
	QPainter painter2(&combinedPixmap2);
	painter2.drawPixmap(0, 0, pixmap2);
	painter2.drawPixmap(0, pixmap2.height(), whitePixmap2);

	// 创建一个 QLabel 来显示 combinedPixmap2
	QLabel* animationLabel2 = new QLabel(ui->widget);
	animationLabel2->setPixmap(combinedPixmap2);
	animationLabel2->setGeometry(
		(ui->widget->width() - combinedPixmap2.width()) / 2,
		(ui->widget->height() - combinedPixmap2.height()) / 2,
		combinedPixmap2.width(),
		combinedPixmap2.height()
	);
	animationLabel2->show();

	// 制作动画2
	QPropertyAnimation* animation2 = new QPropertyAnimation(animationLabel2, "geometry");
	animation2->setDuration(100);
	animation2->setEndValue(
		QRect(
			(ui->widget->width() - combinedPixmap2.width()) / 2,
			0,
			combinedPixmap2.width(),
			combinedPixmap2.height()
		)
	);
	animation2->setStartValue(animationLabel2->geometry());
	animation2->start(QAbstractAnimation::DeleteWhenStopped);
	connect(animation2, &QAbstractAnimation::finished, this, [=]() {
		delete animationLabel2;
		delete animation2;
		});
}
