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
	, previousIndex(0)
	/*, listViewModel(new QStandardItemModel(this))*/
{
	ui->setupUi(this);

	//设置多选
	ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui->listWidget_2->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui->listWidget_4->setSelectionMode(QAbstractItemView::ExtendedSelection);

	onSelectionChanged();

	//信号与槽
	connect(ui->listWidget_2->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FormMesh::onSelectionChanged);

	//ui->listView->setModel(listViewModel);
	//ui->listView->setMouseTracking(true);
	//ui->listView->viewport()->installEventFilter(this);

	//connect(ui->pushButton, &QPushButton::clicked, this, &FormMesh::on_pushButton_clicked);
	//connect(ui->listView, &QListView::clicked, this, &FormMesh::onListViewClicked);
	//connect(this, &FormMesh::toggleRegionSecondAnimation, this, &FormMesh::onToggleRegionSecondAnimation);
}

FormMesh::~FormMesh()
{
	delete ui;
}

void FormMesh::onSelectionChanged()
{
	//链接公共面按钮
	if (ui->listWidget_2->selectedItems().size() == 2) {
		ui->pushButton_3->show();
	}
	else {
		ui->pushButton_3->hide();
	}

	//应用
	if (ui->listWidget_2->selectedItems().size() >0) {
		ui->pushButton->show();
	}
	else {
		ui->pushButton->hide();
	}
}

void FormMesh::resizeEvent(QResizeEvent* event)
{
	ui->tabWidget->tabBar()->setFixedWidth(ui->tabWidget->width());
}

void FormMesh::on_tabWidget_currentChanged(int index)
{
	QWidget* widget0 = ui->tabWidget->widget(0);
	QWidget* widget1 = ui->tabWidget->widget(1);
	QPixmap pixmap0 = QPixmap::grabWidget(widget0);
	QPixmap pixmap1 = QPixmap::grabWidget(widget1);

	int imageWidth = pixmap0.width() + pixmap1.width();
	int imageHeight = pixmap0.height();
	QImage image(imageWidth, imageHeight, QImage::Format_ARGB32);
	image.fill(QColor(Qt::black));

	QPainter p;
	p.begin(&image);
	QBrush brush(QColor(255, 255, 0), Qt::Dense4Pattern);
	p.setBrush(brush);
	QPen pen;
	pen.setColor(QColor(Qt::red));
	p.setPen(pen);
	p.drawPixmap(0, 0, pixmap0);
	p.drawPixmap(pixmap0.width(), 0, pixmap1);
	p.end();

	QLabel* animationWidget = new QLabel(ui->tabWidget);
	animationWidget->setPixmap(QPixmap::fromImage(image));
	QTabBar* bar = ui->tabWidget->tabBar();
	QSize size1 = bar->size();
	QSize size2 = ui->tabWidget->size();
	int pixmapWidth = pixmap0.width();
	int pixmapHeight = pixmap0.height();

	animationWidget->show();
	animationWidget->raise();
	QPropertyAnimation* move = new QPropertyAnimation(animationWidget, "geometry");
	move->setDuration(200);

	if (previousIndex == 0) {
		if (index == 1) {
			move->setStartValue(QRect(0, bar->size().height(), pixmapWidth, pixmapHeight));
			move->setEndValue(QRect(-pixmapWidth, bar->size().height(), pixmapWidth * 2, pixmapHeight));
		}
	}
	else if (previousIndex == 1) {
		if (index == 0) {
			move->setStartValue(QRect(-pixmapWidth, bar->size().height(), pixmapWidth * 2, pixmapHeight));
			move->setEndValue(QRect(0, bar->size().height(), pixmapWidth, pixmapHeight));
		}
	}

	move->start();
	connect(move, &QAbstractAnimation::finished, this, [=]() {
		delete animationWidget;
		delete move;
		});

	previousIndex = index;
}

QListView* FormMesh::createBoundariesListView(std::string regionName, std::vector<std::string> patchNames)
{
	QListView* listView = new QListView(this);
	//QStandardItemModel* listViewModel = new QStandardItemModel(this);

	//listView->setModel(listViewModel);
	//listView->setMouseTracking(true);
	//listView->viewport()->installEventFilter(this);
	//CustomCheckBoxDelegate* delegate = new CustomCheckBoxDelegate(this);
	//listView->setItemDelegate(delegate);
	//listView->setFocusPolicy(Qt::NoFocus);

	//// 设置 QSS 样式
	//listView->setStyleSheet(
	//	"QListView::item {"
	//	"    padding: 11px;"
	//	"}"
	//	"QListView {"
	//	"    border: none;"
	//	"}"
	//	"QListView::item {"
	//	"    background: #FFFFFF;"
	//	"}"
	//	"QListView::item:hover {"
	//	"    background: #F7F7F7;"
	//	"}"
	//	"QListView::item:selected {"
	//	"    background: #EEEEEE;"
	//	"}"
	//	"QListView QScrollBar:vertical {"
	//	"    border: 1px solid #dcdcdc;"
	//	"    background: white;"
	//	"    width: 12px;"
	//	"    margin: 0px 0px 0px 0px;"
	//	"}"
	//	"QListView QScrollBar::handle:vertical {"
	//	"    background: #bfbfbf;"
	//	"    min-height: 20px;"
	//	"    border-radius: 5px;"
	//	"}"
	//	"QListView QScrollBar::add-line:vertical {"
	//	"    background: white;"
	//	"    height: 0px;"
	//	"    subcontrol-position: bottom;"
	//	"    subcontrol-origin: margin;"
	//	"}"
	//	"QListView QScrollBar::sub-line:vertical {"
	//	"    background: white;"
	//	"    height: 0px;"
	//	"    subcontrol-position: top;"
	//	"    subcontrol-origin: margin;"
	//	"}"
	//	"QListView QScrollBar::up-arrow:vertical, QListView QScrollBar::down-arrow:vertical {"
	//	"    border: none;"
	//	"    width: 0px;"
	//	"    height: 0px;"
	//	"    background: none;"
	//	"}"
	//	"QListView QScrollBar::add-page:vertical, QListView QScrollBar::sub-page:vertical {"
	//	"    background: none;"
	//	"}"
	//);

	//// 遍历 patchNames 并添加到 listView 中
	//for (const auto& patchName : patchNames)
	//{
	//	QString actorName = QString::fromStdString(patchName);
	//	QStandardItem* item = new QStandardItem(actorName);
	//	item->setCheckable(true);
	//	if (actorName.endsWith("internalMesh"))item->setCheckState(Qt::Checked);
	//	item->setFlags(item->flags() & ~Qt::ItemIsEditable);
	//	item->setSizeHint(QSize(0, 30));
	//	listViewModel->appendRow(item);
	//}

	//// 设置 QListView 的高度
	//int totalHeight = 0;
	//for (int i = 0; i < listViewModel->rowCount(); ++i) {
	//	totalHeight += listView->sizeHintForRow(i);
	//}
	//listView->setFixedHeight(totalHeight + 2 * ui->listView->frameWidth());

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

std::unordered_map<std::string, unordered_map<std::string, std::string>>FormMesh::analysismeshPatchNames(const std::vector<std::string>& meshPatchNames)
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
			patchTypeTemp.insert(patchTypeTemp.begin(), std::make_pair(item.first, item.second));
		}
		else {
			patchTypeTemp.insert(patchTypeTemp.end(), std::make_pair(item.first, item.second));
		}
	}
	patchType = patchTypeTemp;

	return patchType;
}

void FormMesh::getRegionTypes(const std::string& casePath)
{
	// 修正后的文件路径
	std::string filePath = casePath.substr(0, casePath.find_last_of("/\\")) + "/constant/regionProperties";
	std::ifstream file(filePath);
	if (!file.is_open()) {
		return;  // 文件无法打开则直接返回
	}

	std::unordered_map<std::string, std::string> regionsType;

	// 简单的去除首尾空白字符的函数
	auto trim = [&](const std::string& s) -> std::string {
		if (s.empty())
			return s;
		auto start = s.begin();
		while (start != s.end() && std::isspace(*start))
			++start;
		auto end = s.end();
		do {
			--end;
		} while (std::distance(start, end) > 0 && std::isspace(*end));
		return std::string(start, end + 1);
		};

	bool inRegionsBlock = false;
	bool foundRegionsToken = false;
	std::string line;

	while (std::getline(file, line))
	{
		std::string trimmedLine = trim(line);

		// 寻找 "regions" 关键字
		if (!foundRegionsToken && trimmedLine.find("regions") != std::string::npos)
		{
			foundRegionsToken = true;
			continue;
		}
		// 一旦发现 "regions" 后，遇到 "(" 表示进入解析区域
		if (foundRegionsToken && !inRegionsBlock && trimmedLine.find('(') != std::string::npos)
		{
			inRegionsBlock = true;
			continue;
		}
		// 碰到 ")" 就表示 regions 块结束
		if (inRegionsBlock && trimmedLine[0] == ')')
		{
			break;
		}

		// 在 regions 块中，解析每行结构如：
		// fluid       (bottomAir topAir)
		// solid       (heater leftSolid rightSolid)
		if (inRegionsBlock && !trimmedLine.empty())
		{
			// 先提取区域类型，例如 "fluid"
			// 再从后面的括号中提取所有区域名
			// 形式大概为：<regionType> ( regionName1 regionName2 ... )
			// 可以先找到左括号 "(" 的位置
			size_t bracketPos = trimmedLine.find('(');
			if (bracketPos == std::string::npos)
			{
				continue; // 格式异常则跳过
			}
			std::string regionType = trim(trimmedLine.substr(0, bracketPos));
			// 去掉 regionType 末尾多余空格后如 "fluid"

			// 提取括号内的部分
			size_t rightBracketPos = trimmedLine.find(')', bracketPos + 1);
			if (rightBracketPos == std::string::npos)
			{
				continue; // 没有闭合括号则跳过
			}
			std::string insideBrackets =
				trimmedLine.substr(bracketPos + 1, rightBracketPos - (bracketPos + 1));
			insideBrackets = trim(insideBrackets);

			// 将括号内的区域名称按空白拆分
			std::istringstream iss(insideBrackets);
			std::string regionName;
			while (iss >> regionName)
			{
				regionName = trim(regionName);
				if (!regionName.empty())
				{
					// 将 "regionName" -> "regionType" 存储
					regionsType[regionName] = regionType;
				}
			}
		}
	}

	file.close();

	// 将解析结果存储到全局数据
	GlobalData::getInstance().getCaseData()->regionsType = regionsType;
}

void FormMesh::getPatchTypes(const std::string& casePath)
{
	// 1. 定义 trim 函数（可用 lambda 内联实现）
	auto trim = [&](const std::string& s) -> std::string {
		if (s.empty())
			return s;

		auto start = s.begin();
		while (start != s.end() && std::isspace(*start))
			start++;
		auto end = s.end();
		do {
			end--;
		} while (std::distance(start, end) > 0 && std::isspace(*end));
		return std::string(start, end + 1);
		};

	// 2. 定义 ParseBoundaryFile 函数
	auto parseBoundaryFile = [&](const std::string& filepath) -> std::map<std::string, std::string> {
		std::map<std::string, std::string> patchTypes;
		std::ifstream infile(filepath);
		if (!infile.is_open())
		{
			std::cerr << "无法打开 boundary 文件: " << filepath << std::endl;
			return patchTypes;
		}

		std::string line;
		std::string currentPatch;
		bool inBlock = false;
		while (std::getline(infile, line))
		{
			std::string trimmed = trim(line);
			if (trimmed.empty() || trimmed[0] == '/' || trimmed[0] == '#' || trimmed[0] == '|' || trimmed[0] == '\\')
				continue;

			if (!inBlock)
			{
				currentPatch = trimmed;
				if (std::getline(infile, line))
				{
					trimmed = trim(line);
					if (trimmed == "{")
					{
						inBlock = true;
					}
				}
			}
			else
			{
				if (trimmed == "}")
				{
					inBlock = false;
					currentPatch.clear();
				}
				else
				{
					if (trimmed.find("type") == 0)
					{
						std::istringstream iss(trimmed);
						std::string key, value;
						iss >> key >> value;
						if (!value.empty() && value.back() == ';')
							value.pop_back();  // 去除末尾分号
						patchTypes[currentPatch] = value;
					}
				}
			}
		}
		infile.close();
		return patchTypes;
		};

	// 3. 获取指定路径下的补丁名称 (getPatchNames 逻辑)
	{
		std::vector<std::string> meshPatchNames;
		vtkSmartPointer<vtkOpenFOAMReader> openFOAMReader =
			vtkSmartPointer<vtkOpenFOAMReader>::New();
		openFOAMReader->SetFileName(casePath.c_str());
		openFOAMReader->SetCreateCellToPoint(1);
		openFOAMReader->SetSkipZeroTime(1);
		openFOAMReader->UpdateInformation();

		int numPatches = openFOAMReader->GetNumberOfPatchArrays();
		if (numPatches == 0)
		{
			std::cerr << "没有找到任何补丁。" << std::endl;
			return;
		}
		openFOAMReader->DisableAllPatchArrays();

		for (int i = 0; i < numPatches; ++i)
		{
			const char* currentPatchName = openFOAMReader->GetPatchArrayName(i);
			meshPatchNames.push_back(std::string(currentPatchName));
		}

		// 将解析结果写入全局数据
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> patchNames =
			analysismeshPatchNames(meshPatchNames);
		GlobalData::getInstance().getCaseData()->patchType = patchNames;
	}

	// 4. 解析所有 boundary 文件，获得补丁类型 (getPatchTypes 逻辑)
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> patchType =
		GlobalData::getInstance().getCaseData()->patchType;

	// 尝试读取 single domain 情形
	fs::path mainBoundaryFile = fs::path(casePath.substr(0, casePath.find_last_of("/\\"))) / "constant" / "polyMesh" / "boundary";
	if (fs::exists(mainBoundaryFile))
	{
		auto boundaries = parseBoundaryFile(mainBoundaryFile.string());
		for (const auto& pair : boundaries)
		{
			if (pair.first.find("internalMesh") != std::string::npos)
				continue;
			auto domain = patchType.find("default");
			if (domain != patchType.end())
			{
				domain->second[pair.first] = pair.second;
			}
		}
	}

	// 检查多域 (cellZoneNames)，在不同文件夹下继续解析 boundary
	std::vector<std::string> cellZoneNames = GlobalData::getInstance().getCaseData()->cellZoneNames;
	if (cellZoneNames.size() > 1)
	{
		for (auto& cellZoneName : cellZoneNames)
		{
			fs::path subBoundaryFile =
				fs::path(casePath.substr(0, casePath.find_last_of("/\\"))) / "constant" / cellZoneName / "polyMesh" / "boundary";
			if (fs::exists(subBoundaryFile))
			{
				auto boundaries = parseBoundaryFile(subBoundaryFile.string());
				for (const auto& pair : boundaries)
				{
					if (pair.first.find("internalMesh") != std::string::npos)
						continue;
					auto domain = patchType.find(cellZoneName);
					if (domain != patchType.end())
					{
						domain->second[pair.first] = pair.second;
					}
				}
			}
		}
	}

	// 将解析结果写入全局数据
	GlobalData::getInstance().getCaseData()->patchType = patchType;
}

void FormMesh::getCellZoneNames(const std::string& casePath)
{
	std::vector<std::string> cellZoneNames;
	// 修正后的文件路径
	std::string filePath = casePath.substr(0, casePath.find_last_of("/\\")) + "/constant/polyMesh/cellZones";
	std::ifstream file(filePath);
	if (!file.is_open()) {
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		// 查找含有 "names" 的行
		if (line.find("names") != std::string::npos) {
			// 找到括号范围
			size_t startPos = line.find('(');
			size_t endPos = line.find(')');
			if (startPos != std::string::npos && endPos != std::string::npos) {
				std::string namesStr = line.substr(startPos + 1, endPos - startPos - 1);
				std::istringstream iss(namesStr);
				std::string zoneName;
				while (iss >> zoneName) {
					cellZoneNames.push_back(zoneName);
				}
			}
			break; // 找到后即可跳出循环
		}
	}
	file.close();

	// 将解析结果存储到全局数据
	GlobalData::getInstance().getCaseData()->cellZoneNames = cellZoneNames;
}

void FormMesh::updateForm(bool isRender)
{
	getRegionTypes(GlobalData::getInstance().getCaseData()->casePath);
	getPatchTypes(GlobalData::getInstance().getCaseData()->casePath);
	getCellZoneNames(GlobalData::getInstance().getCaseData()->casePath);

	//Regions
	ui->listWidget->clear();
	std::unordered_map < std::string, std::string> regionsType = GlobalData::getInstance().getCaseData()->regionsType;

	std::string casePath = GlobalData::getInstance().getCaseData()->casePath;
	std::string filePath = casePath.substr(0, casePath.find_last_of("/\\")) + "/constant/polyMesh";
	if (fs::exists(filePath)) {
		auto item = new QListWidgetItem(ui->listWidget);
		ui->listWidget->addItem(item);
		auto widget = new CustomItemWidget(2, this, "default");
		ui->listWidget->setItemWidget(item, widget);
	}

	for (const auto& pair : regionsType)
	{
		auto item = new QListWidgetItem(ui->listWidget);
		ui->listWidget->addItem(item);
		auto widget = new CustomItemWidget(3, this, QString::fromStdString(pair.first));

		QString text = QString::fromStdString(pair.second);
		if (text == "fluid")text = "液体";
		else if (text == "solid")text = "固体";
		widget->ui_ItemWidgetMeshRegions2->comboBox->setCurrentText(text);
		ui->listWidget->setItemWidget(item, widget);
	}

	int totalHeight = 0;
	for (int i = 0; i < std::min(ui->listWidget->count(), 5); ++i) {
		totalHeight += ui->listWidget->sizeHintForRow(i);
	}
	ui->listWidget->setFixedHeight(totalHeight);

	//Boundaries
	ui->listWidget_2->clear();
	std::unordered_map<std::string, unordered_map<std::string, std::string>> patchType = GlobalData::getInstance().getCaseData()->patchType;
	for (const auto& region : patchType)
	{
		QString regionName = QString::fromStdString(region.first);
		for (const auto& patch : region.second)
		{
			QString patchName = QString::fromStdString(patch.first);
			if (patchName == "internalMesh")
				continue;
			auto item = new QListWidgetItem(ui->listWidget_2);
			ui->listWidget->addItem(item);
			auto widget = new CustomItemWidget(0, this, patchName, patchName + " in " + regionName);
			QString text = QString::fromStdString(patch.second);
			if (!text.isEmpty()) {
				text[0] = text[0].toUpper();
			}
			widget->ui_ItemWidgetMeshBoundaries1->comboBox->setCurrentText(text);
			ui->listWidget_2->setItemWidget(item, widget);
		}
	}

	totalHeight = 0;
	for (int i = 0; i < std::min(ui->listWidget_2->count(), 15); ++i) {
		totalHeight += ui->listWidget_2->sizeHintForRow(i);
	}
	ui->listWidget_2->setFixedHeight(totalHeight);

	//CellZones
	ui->listWidget_4->clear();
	std::vector<std::string> cellZoneNames = GlobalData::getInstance().getCaseData()->cellZoneNames;
	for (int i = 0; i < cellZoneNames.size(); i++)
	{
		auto item = new QListWidgetItem(ui->listWidget_4);
		ui->listWidget_4->addItem(item);
		auto widget = new CustomItemWidget(4, this, QString::fromStdString(cellZoneNames[i]));
		ui->listWidget_4->setItemWidget(item, widget);
	}

	totalHeight = 0;
	for (int i = 0; i < std::min(ui->listWidget_4->count(), 5); ++i) {
		totalHeight += ui->listWidget_4->sizeHintForRow(i);
	}
	ui->listWidget_4->setFixedHeight(totalHeight);

	if (isRender) {
		for (int i = 0; i < ui->listWidget_2->count(); ++i)	ui->listWidget_2->item(i)->setSelected(true);
		on_pushButton_clicked();
		updateFormFinished();
	}
}

void FormMesh::onListViewClicked(const QModelIndex& index)
{
	//// 获取 ui->widget 的切换前截图
	//QPixmap pixmap1 = ui->widget->grab();

	//// 页面切换
	//int row = index.row();
	//for (int i = 0; i < listViewBoundaries.size(); ++i)
	//{
	//	listViewBoundaries[listViewBoundaries.size() - i - 1]->setVisible(i == row);
	//}

	//// 获取ui->widget 的切换后截图
	//QPixmap pixmap2 = ui->widget->grab();

	////第一段动画
	////在pixmap下方拼接一个和当前pixmap大小一样的白色图片
	//QPixmap whitePixmap1(pixmap1.size());
	//whitePixmap1.fill(Qt::white);
	//QPixmap combinedPixmap1(pixmap1.width(), pixmap1.height() * 2);
	//QPainter painter1(&combinedPixmap1);
	//painter1.drawPixmap(0, 0, pixmap1);
	//painter1.drawPixmap(0, pixmap1.height(), whitePixmap1);

	//// 创建一个 QLabel 来显示 combinedPixmap1
	//QLabel* animationLabel = new QLabel(ui->widget);
	//animationLabel->setPixmap(combinedPixmap1);
	//animationLabel->setGeometry(
	//	(ui->widget->width() - combinedPixmap1.width()) / 2,
	//	(ui->widget->height() - combinedPixmap1.height()) / 2,
	//	combinedPixmap1.width(),
	//	combinedPixmap1.height()
	//);
	//animationLabel->show();

	//// 制作动画1
	//QPropertyAnimation* animation = new QPropertyAnimation(animationLabel, "geometry");
	//animation->setDuration(100);
	//animation->setStartValue(
	//	QRect(
	//		(ui->widget->width() - combinedPixmap1.width()) / 2,
	//		0,
	//		combinedPixmap1.width(),
	//		combinedPixmap1.height()
	//	)
	//);
	//animation->setEndValue(animationLabel->geometry());
	//animation->start();
	//connect(animation, &QAbstractAnimation::finished, this, [=]() {
	//	delete animationLabel;
	//	delete animation;
	//	emit toggleRegionSecondAnimation();
	//	});
}

void FormMesh::on_pushButton_clicked()
{
	emit apply();
}

void FormMesh::onItemEntered(const QString& text)
{
	////emit itemEntered(text);
}

void FormMesh::onItemExited(const QString& text)
{
	////emit itemExited(text);
}

void FormMesh::onToggleRegionSecondAnimation()
{
	//// 获取ui->widget 的切换后截图
	//QPixmap pixmap2 = ui->widget->grab();

	////第二段动画
	////在pixmap下方拼接一个和当前pixmap大小一样的白色图片
	//QPixmap whitePixmap2(pixmap2.size());
	//whitePixmap2.fill(Qt::white);
	//QPixmap combinedPixmap2(pixmap2.width(), pixmap2.height() * 2);
	//QPainter painter2(&combinedPixmap2);
	//painter2.drawPixmap(0, 0, pixmap2);
	//painter2.drawPixmap(0, pixmap2.height(), whitePixmap2);

	//// 创建一个 QLabel 来显示 combinedPixmap2
	//QLabel* animationLabel2 = new QLabel(ui->widget);
	//animationLabel2->setPixmap(combinedPixmap2);
	//animationLabel2->setGeometry(
	//	(ui->widget->width() - combinedPixmap2.width()) / 2,
	//	(ui->widget->height() - combinedPixmap2.height()) / 2,
	//	combinedPixmap2.width(),
	//	combinedPixmap2.height()
	//);
	//animationLabel2->show();

	//// 制作动画2
	//QPropertyAnimation* animation2 = new QPropertyAnimation(animationLabel2, "geometry");
	//animation2->setDuration(100);
	//animation2->setEndValue(
	//	QRect(
	//		(ui->widget->width() - combinedPixmap2.width()) / 2,
	//		0,
	//		combinedPixmap2.width(),
	//		combinedPixmap2.height()
	//	)
	//);
	//animation2->setStartValue(animationLabel2->geometry());
	//animation2->start(QAbstractAnimation::DeleteWhenStopped); 
	//connect(animation2, &QAbstractAnimation::finished, this, [=]() {
	//	delete animationLabel2;
	//	delete animation2;
	//	});
}

bool FormMesh::eventFilter(QObject* watched, QEvent* event)
{
	//if (watched == ui->listView->viewport())
	//{
	//	if (event->type() == QEvent::MouseMove)
	//	{
	//		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
	//		QPoint pos = mouseEvent->pos();
	//		QModelIndex index = ui->listView->indexAt(pos);

	//		if (index.isValid())
	//		{
	//			if (index != lastIndex)
	//			{
	//				if (lastIndex.isValid())
	//				{
	//					onItemExited(lastIndex.data(Qt::DisplayRole).toString());
	//				}
	//				onItemEntered(index.data(Qt::DisplayRole).toString());
	//				lastIndex = index;
	//			}
	//		}
	//		else
	//		{
	//			if (lastIndex.isValid())
	//			{
	//				onItemExited(lastIndex.data(Qt::DisplayRole).toString());
	//				lastIndex = QModelIndex();
	//			}
	//		}
	//		return false; // 继续处理其他事件
	//	}
	//	else if (event->type() == QEvent::Leave)
	//	{
	//		if (lastIndex.isValid())
	//		{
	//			onItemExited(lastIndex.data(Qt::DisplayRole).toString());
	//			lastIndex = QModelIndex();
	//		}
	//		return false;
	//	}
	//}
	return QWidget::eventFilter(watched, event);
}
