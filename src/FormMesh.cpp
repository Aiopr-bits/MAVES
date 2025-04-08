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
{
	ui->setupUi(this);

	//设置多选
	ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui->listWidget_2->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui->listWidget_4->setSelectionMode(QAbstractItemView::ExtendedSelection);

	onSelectionChanged();

	//信号与槽
	connect(ui->listWidget_2->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FormMesh::onSelectionChanged);		
}

FormMesh::~FormMesh()
{
	delete ui;
}

void FormMesh::onSelectionChanged()
{
	//应用
	if (ui->listWidget_2->selectedItems().size() > 0) {
		ui->pushButton->show();
	}
	else {
		ui->pushButton->hide();
	}

	//链接公共面按钮
	if (ui->listWidget_2->selectedItems().size() == 2) {
		QList<QListWidgetItem*> selectedItems = ui->listWidget_2->selectedItems();
		CustomItemWidget* widget1 = qobject_cast<CustomItemWidget*>(ui->listWidget_2->itemWidget(selectedItems[0]));
		CustomItemWidget* widget2 = qobject_cast<CustomItemWidget*>(ui->listWidget_2->itemWidget(selectedItems[1]));

		QString text1 = widget1->text2;
		QString text2 = widget2->text2;

		int index1 = text1.indexOf(" in ");
		int index2 = text2.indexOf(" in ");
		QString regionName1 = text1.mid(index1 + 4);
		QString regionName2 = text2.mid(index2 + 4);

		if (regionName1 != regionName2 && regionName1 != "default" && regionName2 != "default")
		{
			ui->pushButton_3->show();
		}	
	}
	else {
		ui->pushButton_3->hide();
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
		connect(widget, &CustomItemWidget::textChanged, this, &FormMesh::on_textChanged);
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
		connect(widget, &CustomItemWidget::textChanged, this, &FormMesh::on_textChanged);
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
			connect(widget, &CustomItemWidget::textChanged, this, &FormMesh::on_textChanged);
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
		connect(widget, &CustomItemWidget::textChanged, this, &FormMesh::on_textChanged);
	}

	totalHeight = 0;
	for (int i = 0; i < std::min(ui->listWidget_4->count(), 5); ++i) {
		totalHeight += ui->listWidget_4->sizeHintForRow(i);
	}
	ui->listWidget_4->setFixedHeight(totalHeight);

	if (isRender) {
		for (int i = 0; i < ui->listWidget_2->count(); ++i)	ui->listWidget_2->item(i)->setSelected(true);
		emit updateFormFinished();
	}
}

void FormMesh::on_pushButton_clicked()
{
	emit apply();
}

void FormMesh::on_pushButton_3_clicked()
{
	// 获取选中的两个区域
	QList<QListWidgetItem*> selectedItems = ui->listWidget_2->selectedItems();
	if (selectedItems.size() != 2) {
		return;
	}
	CustomItemWidget* widget1 = qobject_cast<CustomItemWidget*>(ui->listWidget_2->itemWidget(selectedItems[0]));
	CustomItemWidget* widget2 = qobject_cast<CustomItemWidget*>(ui->listWidget_2->itemWidget(selectedItems[1]));
	QString text1 = widget1->text2;
	QString text2 = widget2->text2;
	int index1 = text1.indexOf(" in ");
	int index2 = text2.indexOf(" in ");
	QString regionName1 = text1.mid(index1 + 4);
	QString regionName2 = text2.mid(index2 + 4);
	QString patchName1 = text1.left(index1);
	QString patchName2 = text2.left(index2);

	// 隐藏选中的两个item
	ui->listWidget_2->item(ui->listWidget_2->row(selectedItems[0]))->setHidden(true);
	ui->listWidget_2->item(ui->listWidget_2->row(selectedItems[1]))->setHidden(true);

	auto item = new QListWidgetItem(ui->listWidget_2);
	ui->listWidget_2->addItem(item);
	auto widget = new CustomItemWidget(1, this, patchName1, patchName2, patchName1 + " in " + regionName1, patchName2 + " in " + regionName2);
	ui->listWidget_2->setItemWidget(item, widget);
	connect(widget, &CustomItemWidget::textChanged, this, &FormMesh::on_textChanged);

	//取消选中状态
	ui->listWidget_2->item(ui->listWidget_2->row(selectedItems[0]))->setSelected(false);
	ui->listWidget_2->item(ui->listWidget_2->row(selectedItems[1]))->setSelected(false);

	// 重新设置 ui->listWidget_2 高度，仅计算显示的 item
	int totalHeight = 0;
	for (int i = 0; i < ui->listWidget_2->count(); ++i) {
		QListWidgetItem* item = ui->listWidget_2->item(i);
		if (!item->isHidden())totalHeight += ui->listWidget_2->sizeHintForRow(i);
	}
	ui->listWidget_2->setFixedHeight(totalHeight);

	//取消链接按钮事件
	connect(widget->ui_ItemWidgetMeshBoundaries2->pushButton, &QPushButton::clicked, this, [=]() {
		on_ui_ItemWidgetMeshBoundaries2_pushButton_clicked(widget);
		});
}

void FormMesh::on_ui_ItemWidgetMeshBoundaries2_pushButton_clicked(CustomItemWidget* widget)
{
	// 获取与widget绑定的item
	QListWidgetItem* item = nullptr;
	for (int i = 0; i < ui->listWidget_2->count(); ++i) {
		QListWidgetItem* itemTemp = ui->listWidget_2->item(i);
		if (ui->listWidget_2->itemWidget(itemTemp) == widget) {
			item = itemTemp;
			break;
		}
	}

	if (item == nullptr) {
		return;
	}

	// 获取链接后的item中的信息
	QString text1 = widget->text3;
	QString text2 = widget->text4;
	int index1 = text1.indexOf(" in ");
	int index2 = text2.indexOf(" in ");
	QString regionName1 = text1.mid(index1 + 4);
	QString regionName2 = text2.mid(index2 + 4);
	QString patchName1 = text1.left(index1);
	QString patchName2 = text2.left(index2);

	// 移除链接后的item
	delete ui->listWidget_2->takeItem(ui->listWidget_2->row(item));

	// 显示之前隐藏的两个item
	for (int i = 0; i < ui->listWidget_2->count(); i++)
	{
		QListWidgetItem* item = ui->listWidget_2->item(i);
		CustomItemWidget* widget = qobject_cast<CustomItemWidget*>(ui->listWidget_2->itemWidget(item));
		if (widget != nullptr && (widget->text2 == patchName1 + " in " + regionName1 || widget->text2 == patchName2 + " in " + regionName2)) {
			item->setHidden(false);
		}
	}

	// 重新设置 ui->listWidget_2 高度，仅计算显示的 item
	int totalHeight = 0;
	for (int i = 0; i < ui->listWidget_2->count(); ++i) {
		QListWidgetItem* item = ui->listWidget_2->item(i);
		if (!item->isHidden())totalHeight += ui->listWidget_2->sizeHintForRow(i);
	}
	ui->listWidget_2->setFixedHeight(totalHeight);
}

void FormMesh::on_textChanged(CustomItemWidget* widget, QString previousText)
{
	if (widget->ui_ItemWidgetMeshBoundaries1 != nullptr) 
	{
		QString currentText = widget->text2;
		if (previousText != currentText)
		{
			QString previousRegionsName = previousText.mid(previousText.indexOf(" in ") + 4);
			QString previousPatchName = previousText.left(previousText.indexOf(" in "));
			QString currentRegionsName = currentText.mid(currentText.indexOf(" in ") + 4);
			QString currentPatchName = currentText.left(currentText.indexOf(" in "));

			// 替换 GlobalData 变量
			std::unordered_map<std::string, unordered_map<std::string, std::string>> patchType = GlobalData::getInstance().getCaseData()->patchType;
			patchType[previousRegionsName.toStdString()][currentPatchName.toStdString()] = patchType[previousRegionsName.toStdString()][previousPatchName.toStdString()];
			patchType[previousRegionsName.toStdString()].erase(previousPatchName.toStdString());
			GlobalData::getInstance().getCaseData()->patchType = patchType;

			// 修改文件
			std::string casePath = GlobalData::getInstance().getCaseData()->casePath;
			std::string filePath;
			if (previousRegionsName == "default")
				filePath = casePath.substr(0, casePath.find_last_of("/\\")) + "/constant/polyMesh/boundary";
			else
				filePath = casePath.substr(0, casePath.find_last_of("/\\")) + "/constant/" + previousRegionsName.toStdString() + "/polyMesh/boundary";

			std::ifstream file(filePath);
			if (!file.is_open()) {
				qDebug() << "无法打开文件:" << QString::fromStdString(filePath);
				return;
			}

			std::string content;
			std::string line;
			while (std::getline(file, line)) {
				// 替换边界名称
				if (line.find(previousPatchName.toStdString()) != std::string::npos) {
					size_t pos = line.find(previousPatchName.toStdString());
					line.replace(pos, previousPatchName.toStdString().length(), currentPatchName.toStdString());
				}
				content += line + "\n";
			}
			file.close();

			// 将修改后的内容写回文件
			std::ofstream outFile(filePath);
			if (!outFile.is_open()) {
				qDebug() << "无法写入文件:" << QString::fromStdString(filePath);
				return;
			}
			outFile << content;
			outFile.close();
		}
	}
	else if (widget->ui_ItemWidgetMeshBoundaries2 != nullptr)
	{
		//获取当前文本
		QString currentText = widget->text3;
		for (int i = 0; i < ui->listWidget_2->count(); i++)
		{
			QListWidgetItem* item = ui->listWidget_2->item(i);
			CustomItemWidget* widgetHided = qobject_cast<CustomItemWidget*>(ui->listWidget_2->itemWidget(item));
			if (!widgetHided->isHidden()) continue;

			if (widgetHided->text2 == currentText)
			{
				currentText = widget->text4;
				break;
			}
		}

		for (int i = 0; i < ui->listWidget_2->count(); i++)
		{
			QListWidgetItem* item = ui->listWidget_2->item(i);
			CustomItemWidget* widgetHided = qobject_cast<CustomItemWidget*>(ui->listWidget_2->itemWidget(item));
			if (widgetHided->text2 == previousText)
			{
				widgetHided->text2 = currentText;
				widgetHided->text1 = currentText.left(currentText.indexOf(" in "));
				emit widgetHided->textChanged(widgetHided, previousText);				
				break;
			}

		}
	}
	else if (widget->ui_ItemWidgetMeshRegions1 != nullptr)
	{

	}
	else if (widget->ui_ItemWidgetMeshRegions2 != nullptr)
	{
		QString currentText = widget->text1;
		if (previousText == currentText) return;

		// 替换 GlobalData 变量
		std::unordered_map<std::string, std::string> regionsType = GlobalData::getInstance().getCaseData()->regionsType;
		regionsType[currentText.toStdString()] = regionsType[previousText.toStdString()];
		regionsType.erase(previousText.toStdString());
		GlobalData::getInstance().getCaseData()->regionsType = regionsType;

		// 修改文件
		std::string casePath = GlobalData::getInstance().getCaseData()->casePath;
		std::string filePath = casePath.substr(0, casePath.find_last_of("/\\")) + "/constant/regionProperties";

		std::ifstream file(filePath);
		if (!file.is_open()) {
			qDebug() << "无法打开文件:" << QString::fromStdString(filePath);
			return;
		}

		std::string content;
		std::string line;
		bool inRegionsBlock = false;
		bool foundRegionsToken = false;

		while (std::getline(file, line)) {
			std::string trimmedLine = line;
			trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t")); // 去除行首空格
			trimmedLine.erase(trimmedLine.find_last_not_of(" \t") + 1); // 去除行尾空格

			// 检测是否找到 regions 关键字
			if (!foundRegionsToken && trimmedLine.find("regions") != std::string::npos) {
				foundRegionsToken = true;
				content += line + "\n";
				continue;
			}

			// 检测是否进入 regions 块
			if (foundRegionsToken && !inRegionsBlock && trimmedLine == "(") {
				inRegionsBlock = true;
				content += line + "\n";
				continue;
			}

			// 检测是否退出 regions 块
			if (inRegionsBlock && trimmedLine == ")") {
				inRegionsBlock = false;
				content += line + "\n";
				continue;
			}

			// 在 regions 块中，修改括号中的 regionName
			if (inRegionsBlock && !trimmedLine.empty()) {
				size_t bracketPos = trimmedLine.find('(');
				size_t rightBracketPos = trimmedLine.find(')', bracketPos + 1);

				if (bracketPos != std::string::npos && rightBracketPos != std::string::npos) {
					std::string regionType = trimmedLine.substr(0, bracketPos);
					regionType.erase(regionType.find_last_not_of(" \t") + 1); // 去除区域类型末尾空格

					std::string insideBrackets = trimmedLine.substr(bracketPos + 1, rightBracketPos - bracketPos - 1);
					std::istringstream iss(insideBrackets);
					std::string regionName;
					std::string updatedRegionNames;

					while (iss >> regionName) {
						if (regionName == previousText.toStdString()) {
							regionName = currentText.toStdString(); // 替换 regionName
						}
						updatedRegionNames += regionName + " ";
					}

					updatedRegionNames.erase(updatedRegionNames.find_last_not_of(" ") + 1); // 去除末尾多余空格
					line = "    " + regionType + "       (" + updatedRegionNames + ")";
				}
			}

			content += line + "\n";
		}
		file.close();

		// 将修改后的内容写回文件
		std::ofstream outFile(filePath);
		if (!outFile.is_open()) {
			qDebug() << "无法写入文件:" << QString::fromStdString(filePath);
			return;
		}
		outFile << content;
		outFile.close();

		//修改文件夹名称
		std::string folderPath = casePath.substr(0, casePath.find_last_of("/\\")) + "/constant/" + previousText.toStdString();
		std::string newFolderPath = casePath.substr(0, casePath.find_last_of("/\\")) + "/constant/" + currentText.toStdString();
		if (fs::exists(folderPath)) {
			fs::rename(folderPath, newFolderPath);
		}
	}
	else if (widget->ui_ItemWidgetMeshZones != nullptr)
	{
		QString currentText = widget->text1;
		if (previousText == currentText) return;

		// 替换 GlobalData 变量
		std::vector<std::string> cellZoneNames = GlobalData::getInstance().getCaseData()->cellZoneNames;
		for (int i = 0; i < cellZoneNames.size(); i++)
		{
			if (cellZoneNames[i] == previousText.toStdString())
			{
				cellZoneNames[i] = currentText.toStdString();
				break;
			}
		}
		GlobalData::getInstance().getCaseData()->cellZoneNames = cellZoneNames;

		// 修改文件
		std::string casePath = GlobalData::getInstance().getCaseData()->casePath;
		std::string topoSetDictPath = casePath.substr(0, casePath.find_last_of("/\\")) + "/system/topoSetDict";
		fs::remove(topoSetDictPath); // 删除旧文件（如果存在）
		std::ofstream topoSetDictFile(topoSetDictPath);
		if (!topoSetDictFile) {
			qDebug() << "无法创建文件:" << QString::fromStdString(topoSetDictPath);
			return;
		}

		topoSetDictFile << "FoamFile\n";
		topoSetDictFile << "{\n";
		topoSetDictFile << "	version	2.0;\n";
		topoSetDictFile << "	format	ascii;\n";
		topoSetDictFile << "	class	dictionary;\n";
		topoSetDictFile << "	location	\"system\";\n";
		topoSetDictFile << "	object	topoSetDict;\n";
		topoSetDictFile << "}\n";
		topoSetDictFile << "actions	(\n";
		topoSetDictFile << "	{\n";
        topoSetDictFile << "		name	" << currentText.toStdString() << "; \n";
		topoSetDictFile << "		type	cellZoneSet;\n";
		topoSetDictFile << "		action	new;\n";
		topoSetDictFile << "		source	zoneToCell;\n";
		topoSetDictFile << "		sourceInfo\n";
		topoSetDictFile << "		{\n";
		topoSetDictFile << "			zone	" << previousText.toStdString() << ";\n";
		topoSetDictFile << "		}\n";
		topoSetDictFile << "	}\n";
		topoSetDictFile << "\n";
		topoSetDictFile << "	{\n";
		topoSetDictFile << "		name	" << previousText.toStdString() << "; \n";
		topoSetDictFile << "		type	cellZoneSet;\n";
		topoSetDictFile << "		action	remove;\n";
		topoSetDictFile << "	}\n";
		topoSetDictFile << ");\n";
		topoSetDictFile.close();

		//执行topoSet
		emit topoSet();
	}
}
