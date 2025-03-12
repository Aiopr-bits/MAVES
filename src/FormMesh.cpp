/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

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

	connect(ui->pushButton, &QPushButton::clicked, this, &FormMesh::on_pushButton_clicked);
	connect(ui->listView, &QListView::clicked, this, &FormMesh::onListViewClicked);
	connect(this, &FormMesh::toggleRegionSecondAnimation, this, &FormMesh::onToggleRegionSecondAnimation);
}

FormMesh::~FormMesh()
{
	delete ui;
}

QListView* FormMesh::createBoundariesListView(std::string regionName, std::vector<std::string> patchNames)
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
		if (regionName == "default" && actorName != "internalMesh") item->setCheckState(Qt::Checked);
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

void FormMesh::split(const std::string& s, char delimiter, std::vector<std::string>& tokens) {
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

std::unordered_map<std::string, std::vector<std::string>>FormMesh:: analysismeshPatchNames(const std::vector<std::string>& meshPatchNames)
{
	std::unordered_map<std::string, std::vector<std::string>> meshPatchNamesMap;
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

		meshPatchNamesMap[region].push_back(patch);
	}

	//将key==default的元素放到第一个
	std::unordered_map<std::string, std::vector<std::string>> meshPatchNamesMapTemp;
	for (const auto& item : meshPatchNamesMap) {
		if (item.first == "default") {
			meshPatchNamesMapTemp.insert(meshPatchNamesMapTemp.begin(), item);
		}
		else {
			meshPatchNamesMapTemp.insert(meshPatchNamesMapTemp.end(), item);
		}
	}
	meshPatchNamesMap = meshPatchNamesMapTemp;

	return meshPatchNamesMap;
}

void FormMesh::getMeshPatchData(const std::string& casePath)
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

	std::unordered_map<std::string, std::vector<std::string>> meshPatchNamesMap = analysismeshPatchNames(meshPatchNames);
	GlobalData::getInstance().getCaseData()->meshPatchNamesMap = meshPatchNamesMap;
}

void FormMesh::updateForm()
{
	getMeshPatchData(GlobalData::getInstance().getCaseData()->casePath);

	std::unordered_map<std::string, std::vector<std::string>> meshPatchNamesMap =GlobalData::getInstance().getCaseData()->meshPatchNamesMap;
	listViewModel->clear();

	// 遍历 meshPatchNamesMap 并将 key 值添加到 listView 中
	QIcon regionIcon("..\\res\\region.png");
	for (const auto& pair : meshPatchNamesMap)
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
	std::vector<std::pair<std::string, std::vector<std::string>>> reversedMeshPatch;
	reversedMeshPatch.reserve(meshPatchNamesMap.size());
	for (const auto& kv : meshPatchNamesMap) {
		reversedMeshPatch.push_back(kv);
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
		QListView* listView = createBoundariesListView(kv.first, kv.second);
		ui->verticalLayout->insertWidget(1, listView);
		listViewBoundaries.push_back(listView);
	}

	// 点击 ui->listView 的第一个 item
	QModelIndex index = listViewModel->index(0, 0);
	ui->listView->setCurrentIndex(index);
	onListViewClicked(index);

	on_pushButton_clicked();
}

void FormMesh::onListViewClicked(const QModelIndex& index)
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

void FormMesh::on_pushButton_clicked()
{
	emit apply(listViewBoundaries);
}

void FormMesh::onItemEntered(const QString& text)
{
	//emit itemEntered(text);
}

void FormMesh::onItemExited(const QString& text)
{
	//emit itemExited(text);
}

void FormMesh::onToggleRegionSecondAnimation()
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
		emit updateFormFinished();
		});
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
			return false; // 继续处理其他事件
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
