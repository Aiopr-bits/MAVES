/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#include "FormBoundaryConditions.h"
#include <QTextStream>
#include <qdebug.h>

FormBoundaryConditions::FormBoundaryConditions(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::FormBoundaryConditionsClass())
{
	ui->setupUi(this);

	//this->widget添加一个FormBoundaryConditionsTabWidget
	FormBoundaryConditionsTabWidget* tabWidget = new FormBoundaryConditionsTabWidget(this);

	// 检查 this->widget 是否已有布局
	if (!ui->widget->layout()) {
		// 如果没有布局，创建一个 QVBoxLayout
		QVBoxLayout* layout = new QVBoxLayout(ui->widget);
		layout->setContentsMargins(0, 0, 0, 0); // 可选：移除边距
		ui->widget->setLayout(layout);
	}

	// 将 tabWidget 添加到 this->widget 的布局中
	ui->widget->layout()->addWidget(tabWidget);



	//listViewModel = new QStandardItemModel(this);
	//ui->listView->setModel(listViewModel);
	//ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);

	//connect(ui->listView, &QListView::clicked, this, &FormBoundaryConditions::onListViewItemClicked);
	//connect(this, &FormBoundaryConditions::toggleRegionSecondAnimation, this, &FormBoundaryConditions::onToggleRegionSecondAnimation);
}

FormBoundaryConditions::~FormBoundaryConditions()
{
	delete ui;
}

void FormBoundaryConditions::onMeshImported()
{
	//initListView();
	//initSubWidget();

	////点击第一个item
	//QModelIndex index = listViewModel->index(0, 0);
	//ui->listView->setCurrentIndex(index);
	//onListViewItemClicked(index);
}

void FormBoundaryConditions::initListView()
{
	//listViewModel->clear();
	//patchName.clear();

	//std::unordered_map<std::string, unordered_map<std::string, std::string>> patchType = GlobalData::getInstance().getCaseData()->patchType;
	//for (const auto& regions : patchType) {
	//	for (const auto& patchname : regions.second) {
	//		if (patchname.first != "internalMesh")
	//		{
	//			if (regions.first != "default")
	//				patchName.push_back(QString::fromStdString(regions.first + "/" + patchname.first));
	//			else
	//				patchName.push_back(QString::fromStdString(patchname.first));
	//		}
	//	}
	//}

	//QIcon icon("../res/patch.png");
	//foreach(const QString & name, patchName) {
	//	QStandardItem* item = new QStandardItem(icon, name);
	//	item->setFlags(item->flags() & ~Qt::ItemIsEditable);
	//	item->setSizeHint(QSize(0, 30));
	//	listViewModel->appendRow(item);
	//}
	//
	//// 计算所有 item 的总高度
	//int totalHeight = 0;
	//for (int i = 0; i < std::min(8, listViewModel->rowCount()); ++i) {
	//	totalHeight += ui->listView->sizeHintForRow(i);
	//}
	//ui->listView->setFixedHeight(totalHeight + 2 * ui->listView->frameWidth());
}

void FormBoundaryConditions::initSubWidget()
{
	//// 先移除并销毁 ui->widget 上所有布局和控件
	//QLayout* layout = ui->widget->layout();
	//if (layout) {
	//	QLayoutItem* item;
	//	while ((item = layout->takeAt(0)) != nullptr) {
	//		if (item->widget()) {
	//			delete item->widget();
	//		}
	//		delete item;
	//	}
	//	delete layout;
	//}

	////为每个patch创建一个FormBoundaryConditionsTabWidget
	//tabWidgetsGroup.clear();
	//QVBoxLayout* vLayout = new QVBoxLayout();
	//ui->widget->setLayout(vLayout);
	//for (int i = 0; i < patchName.size(); ++i) {
	//	FormBoundaryConditionsTabWidget* tabWidget = new FormBoundaryConditionsTabWidget();
	//	tabWidgetsGroup.push_back(tabWidget);
	//	vLayout->addWidget(tabWidget);
	//	tabWidget->setVisible(false);		
	//}
}

void FormBoundaryConditions::initBoundaryConditions()
{
	//std::string solverName = GlobalData::getInstance().getCaseData()->solverName;
	//const auto& meshFaceActors = GlobalData::getInstance().getCaseData()->meshFaceActors;

	//std::vector<QString> physicalFields;
	//if (solverName == "steadyCompressibleSolver") { // 外流场
	//	physicalFields = { "p", "T", "U", "k", "nut", "omega", "alphat" };
	//}
	//else if (solverName == "transientIncompressibleSolver") { // 内流场
	//	physicalFields = { "p", "T", "U", "p_rgh", "nut", "k", "epsilon", "alphat" };
	//}

	//std::vector<QString> boundaryField;
	//for (const auto& pair : meshFaceActors) {
	//	boundaryField.push_back(pair.first);
	//}

	//boundaryConditions.clear();
	//for (const auto& physicalField : physicalFields) {
	//	QMap<QString, QVector<QString>> boundaryCondition;
	//	for (const auto& field : boundaryField) {
	//		boundaryCondition[field] = QVector<QString>(2, "");
	//	}
	//	boundaryConditions[physicalField] = boundaryCondition;
	//}
}

void FormBoundaryConditions::parseBoundaryFile(const QString& filePath, const QString& fieldName)
{
	//QFile file(filePath);
	//if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	//	QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
	//	return;
	//}

	//QTextStream in(&file);
	//QString content = in.readAll();
	//content.replace("uniform", "");
	//file.close();

	//// 提取 boundaryField 之前的内容
	//QRegularExpression boundaryFieldRegex(R"(boundaryField\s*\{)");
	//QRegularExpressionMatch boundaryFieldMatch = boundaryFieldRegex.match(content);
	//if (!boundaryFieldMatch.hasMatch()) {
	//	QMessageBox::warning(this, "错误", "无法解析文件: " + filePath);
	//	return;
	//}

	//QString preBoundaryFieldContent = content.left(boundaryFieldMatch.capturedStart());

	//// 提取变量定义
	//QMap<QString, QString> variables;
	//QRegularExpression variableRegex(R"((\w+)\s+([\w\d\.\+\-eE\(\)\s\$]+);)");
	//QRegularExpressionMatchIterator varIt = variableRegex.globalMatch(preBoundaryFieldContent);
	//while (varIt.hasNext()) {
	//	QRegularExpressionMatch varMatch = varIt.next();
	//	QString varName = varMatch.captured(1);
	//	QString varValue = varMatch.captured(2);
	//	variables[varName] = varValue;
	//}

	//// 递归解析变量值
	//auto resolveVariable = [&variables](const QString& value) -> QString {
	//	QString resolvedValue = value;
	//	QRegularExpression varRefRegex(R"(\$(\w+))");
	//	QRegularExpressionMatchIterator varRefIt = varRefRegex.globalMatch(value);
	//	while (varRefIt.hasNext()) {
	//		QRegularExpressionMatch varRefMatch = varRefIt.next();
	//		QString varRefName = varRefMatch.captured(1);
	//		if (variables.contains(varRefName)) {
	//			resolvedValue.replace("$" + varRefName, variables[varRefName]);
	//		}
	//	}
	//	return resolvedValue;
	//	};

	//// 解析所有变量的值
	//for (auto it = variables.begin(); it != variables.end(); ++it) {
	//	it.value() = resolveVariable(it.value());
	//}

	//QRegularExpression boundaryFieldRegex2(R"(boundaryField\s*\{([\s\S]*?)\n\})");
	//QRegularExpressionMatch match = boundaryFieldRegex2.match(content);
	//if (!match.hasMatch()) {
	//	QMessageBox::warning(this, "错误", "无法解析文件: " + filePath);
	//	return;
	//}

	//QString boundaryFieldContent = match.captured(1);
	//QRegularExpression varRefRegex2(R"(\$(\w+))");
	//QRegularExpressionMatchIterator varRefIt2 = varRefRegex2.globalMatch(boundaryFieldContent);
	//while (varRefIt2.hasNext()) {
	//	QRegularExpressionMatch varRefMatch = varRefIt2.next();
	//	QString varRefName = varRefMatch.captured(1);
	//	if (variables.contains(varRefName)) {
	//		boundaryFieldContent.replace("$" + varRefName, variables[varRefName]);
	//	}
	//}

	//QRegularExpression boundaryRegex(R"((\w+)\s*\{([\s\S]*?)\n\s*\})");
	//QRegularExpressionMatchIterator it = boundaryRegex.globalMatch(boundaryFieldContent);

	//while (it.hasNext()) {
	//	QRegularExpressionMatch boundaryMatch = it.next();
	//	QString boundaryName = boundaryMatch.captured(1);
	//	QString boundaryContent = boundaryMatch.captured(2);

	//	QRegularExpression typeRegex(R"(type\s+([\w:]+);)");
	//	QRegularExpression valueRegex(R"(value\s+([\w\d\.\+\-eE\(\)\s\$]+);)");

	//	QRegularExpressionMatch typeMatch = typeRegex.match(boundaryContent);
	//	QRegularExpressionMatch valueMatch = valueRegex.match(boundaryContent);

	//	QString type = typeMatch.hasMatch() ? typeMatch.captured(1) : "";
	//	QString value = valueMatch.hasMatch() ? valueMatch.captured(1) : "";

	//	//特殊处理
	//	if (type == "compressible::alphatWallFunction")type = "alphatWallFunction";
	//	if (type == "externalWallHeatFluxTemperature")type = "heatFluxTemperature";

	//	if (boundaryConditions.contains(fieldName) && boundaryConditions[fieldName].contains(boundaryName)) {
	//		boundaryConditions[fieldName][boundaryName][0] = type;
	//		boundaryConditions[fieldName][boundaryName][1] = value;
	//	}
	//}
}

void FormBoundaryConditions::onToggleRegionSecondAnimation()
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

void FormBoundaryConditions::importParameter()
{
	//// 获取案例路径
	//QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	//QString caseDirPath = QFileInfo(casePath).absolutePath();

	//std::string solverName = GlobalData::getInstance().getCaseData()->solverName;
	//std::vector<QString> fileNames;
	//if (solverName == "steadyCompressibleSolver") { // 外流场
	//	fileNames = { "p", "T", "U", "k", "nut", "omega", "alphat" };
	//}
	//else if (solverName == "transientIncompressibleSolver") { // 内流场
	//	fileNames = { "p", "T", "U", "p_rgh", "nut", "k", "epsilon", "alphat" };
	//}

	//// 初始化边界条件
	//initBoundaryConditions();

	//// 解析每个文件并更新边界条件
	//for (const QString& fileName : fileNames) {
	//	QString filePath = caseDirPath + "/0/" + fileName;
	//	parseBoundaryFile(filePath, fileName);
	//}

	//// 更新每个 tab 页的 comboBox 选择项
	//for (int i = 0; i < ui->tabWidget->count(); ++i) {
	//	QString tabName = ui->tabWidget->tabText(i);
	//	FormBoundaryConditionsTabWidget* tabWidget = qobject_cast<FormBoundaryConditionsTabWidget*>(ui->tabWidget->widget(i));

	//	auto updateComboBox = [&](QComboBox* comboBox, const QString& field) {
	//		for (int j = 0; j < comboBox->count(); ++j) {
	//			if (comboBox->itemText(j) == boundaryConditions[field][tabName][0]) {
	//				comboBox->setCurrentIndex(j);
	//				break;
	//			}
	//		}
	//		};

	//	updateComboBox(tabWidget->ui->comboBox, "p");
	//	updateComboBox(tabWidget->ui->comboBox_2, "T");
	//	updateComboBox(tabWidget->ui->comboBox_3, "U");
	//	updateComboBox(tabWidget->ui->comboBox_4, "k");
	//	updateComboBox(tabWidget->ui->comboBox_5, "nut");
	//	updateComboBox(tabWidget->ui->comboBox_6, "omega");
	//	updateComboBox(tabWidget->ui->comboBox_7, "alphat");

	//	QStringList specialTypes = { "fixedValue", "heatFluxTemperature", "inletOutlet", "kqRWallFunction", "nutkWallFunction", "omegaWallFunction", "alphatWallFunction", "calculated" };

	//	auto updateLineEdit = [&](QComboBox* comboBox, QLineEdit* lineEdit, const QString& field) {
	//		if (specialTypes.contains(comboBox->currentText())) {
	//			lineEdit->setText(boundaryConditions[field][tabName][1]);
	//		}
	//		};

	//	updateLineEdit(tabWidget->ui->comboBox, tabWidget->ui->lineEdit, "p");
	//	updateLineEdit(tabWidget->ui->comboBox_2, tabWidget->ui->lineEdit_2, "T");

	//	if (specialTypes.contains(tabWidget->ui->comboBox_3->currentText())) {
	//		QString value = boundaryConditions["U"][tabName][1];
	//		QRegularExpression re(R"(\(([^)]+)\))");
	//		QRegularExpressionMatch match = re.match(value);
	//		if (match.hasMatch()) {
	//			QStringList valueList = match.captured(1).split(" ");
	//			if (valueList.size() == 3) {
	//				tabWidget->ui->lineEdit_8->setText(valueList[0]);
	//				tabWidget->ui->lineEdit_9->setText(valueList[1]);
	//				tabWidget->ui->lineEdit_10->setText(valueList[2]);
	//			}
	//		}
	//	}

	//	updateLineEdit(tabWidget->ui->comboBox_4, tabWidget->ui->lineEdit_4, "k");
	//	updateLineEdit(tabWidget->ui->comboBox_5, tabWidget->ui->lineEdit_5, "nut");
	//	updateLineEdit(tabWidget->ui->comboBox_6, tabWidget->ui->lineEdit_6, "omega");
	//	updateLineEdit(tabWidget->ui->comboBox_7, tabWidget->ui->lineEdit_7, "alphat");
	//}
}

void FormBoundaryConditions::exportParameter()
{
	//// 获取案例路径
	//QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	//QString caseDirPath = QFileInfo(casePath).absolutePath();
	//QStringList fileNames = { "p", "T", "U", "k", "nut", "omega", "alphat" };

	//// 将界面上的修改写入 boundaryConditions 中
	//for (int i = 0; i < ui->tabWidget->count(); ++i) {
	//	QString tabName = ui->tabWidget->tabText(i);
	//	FormBoundaryConditionsTabWidget* tabWidget = qobject_cast<FormBoundaryConditionsTabWidget*>(ui->tabWidget->widget(i));

	//	auto updateBoundaryConditions = [&](QComboBox* comboBox, QLineEdit* lineEdit, const QString& field) {
	//		boundaryConditions[field][tabName][0] = comboBox->currentText();

	//		// 特殊处理
	//		if (boundaryConditions[field][tabName][0] == "alphatWallFunction") boundaryConditions[field][tabName][0] = "compressible::alphatWallFunction";
	//		if (boundaryConditions[field][tabName][0] == "heatFluxTemperature") boundaryConditions[field][tabName][0] = "externalWallHeatFluxTemperature";

	//		if (!lineEdit->text().isEmpty()) {
	//			boundaryConditions[field][tabName][1] = lineEdit->text();
	//		}
	//		};

	//	updateBoundaryConditions(tabWidget->ui->comboBox, tabWidget->ui->lineEdit, "p");
	//	updateBoundaryConditions(tabWidget->ui->comboBox_2, tabWidget->ui->lineEdit_2, "T");

	//	boundaryConditions["U"][tabName][0] = tabWidget->ui->comboBox_3->currentText();
	//	QString value = QString("(%1 %2 %3)").arg(tabWidget->ui->lineEdit_8->text(), tabWidget->ui->lineEdit_9->text(), tabWidget->ui->lineEdit_10->text());
	//	boundaryConditions["U"][tabName][1] = value;

	//	updateBoundaryConditions(tabWidget->ui->comboBox_4, tabWidget->ui->lineEdit_4, "k");
	//	updateBoundaryConditions(tabWidget->ui->comboBox_5, tabWidget->ui->lineEdit_5, "nut");
	//	updateBoundaryConditions(tabWidget->ui->comboBox_6, tabWidget->ui->lineEdit_6, "omega");
	//	updateBoundaryConditions(tabWidget->ui->comboBox_7, tabWidget->ui->lineEdit_7, "alphat");
	//}

	//// 遍历每个物理场文件
	//for (const QString& fileName : fileNames) {
	//	QString filePath = caseDirPath + "/0/" + fileName;
	//	QFile file(filePath);
	//	if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
	//		QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
	//		continue;
	//	}

	//	QTextStream in(&file);
	//	QString content = in.readAll();
	//	file.close();

	//	// 提取 boundaryField 之前的内容
	//	QRegularExpression boundaryFieldRegex(R"(boundaryField\s*\{)");
	//	QRegularExpressionMatch boundaryFieldMatch = boundaryFieldRegex.match(content);
	//	if (!boundaryFieldMatch.hasMatch()) {
	//		QMessageBox::warning(this, "错误", "无法解析文件: " + filePath);
	//		return;
	//	}
	//	QString preBoundaryFieldContent = content.left(boundaryFieldMatch.capturedStart());

	//	// 向 preBoundaryFieldContent 中写入 boundaryField 的部分
	//	QStringList specialTypes = { "fixedValue", "externalWallHeatFluxTemperature", "inletOutlet", "kqRWallFunction", "nutkWallFunction", "omegaWallFunction", "compressible::alphatWallFunction", "calculated" };
	//	preBoundaryFieldContent += "boundaryField\n{\n";
	//	const auto& boundaryField = boundaryConditions[fileName];
	//	for (auto it = boundaryField.begin(); it != boundaryField.end(); ++it) {
	//		preBoundaryFieldContent += "    " + it.key() + "\n";
	//		preBoundaryFieldContent += "    {\n";
	//		preBoundaryFieldContent += "        type            " + it.value()[0] + ";\n";
	//		if (it.value()[0] == "externalWallHeatFluxTemperature") {
	//			preBoundaryFieldContent += "        mode            coefficient;\n";
	//			preBoundaryFieldContent += "        Ta              constant 300.0;\n";
	//			preBoundaryFieldContent += "        h               uniform 100.0;\n";
	//			preBoundaryFieldContent += "        thicknessLayers (0.001);\n";
	//			preBoundaryFieldContent += "        kappaLayers     (1);\n";
	//			preBoundaryFieldContent += "        kappaMethod     fluidThermo;\n";
	//		}
	//		if (it.value()[0] == "inletOutlet") {
	//			preBoundaryFieldContent += "        inletValue      uniform " + it.value()[1] + ";\n";
	//		}
	//		if (specialTypes.contains(it.value()[0])) {
	//			preBoundaryFieldContent += "        value           uniform " + it.value()[1] + ";\n";
	//		}
	//		preBoundaryFieldContent += "    }\n";
	//	}
	//	preBoundaryFieldContent += "}\n";

	//	// 将 preBoundaryFieldContent 写入文件，替换原来的文件内容
	//	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
	//		QMessageBox::warning(this, "错误", "无法打开文件: " + filePath);
	//		continue;
	//	}
	//	QTextStream out(&file);
	//	out << preBoundaryFieldContent;
	//	file.close();
	//}
}

void FormBoundaryConditions::onListViewItemClicked(const QModelIndex& index)
{
//	// 获取 ui->widget 的切换前截图
//	QPixmap pixmap1 = ui->widget->grab();
//
//	//隐藏所有的FormBoundaryConditionsTabWidget
//	for (int i = 0; i < tabWidgetsGroup.size(); ++i) {
//		tabWidgetsGroup[i]->setVisible(false);
//	}
//
//	//显示选中的FormBoundaryConditionsTabWidget
//	tabWidgetsGroup[index.row()]->setVisible(true);
//
//	// 获取ui->widget 的切换后截图
//	QPixmap pixmap2 = ui->widget->grab();
//
//	//第一段动画
////在pixmap下方拼接一个和当前pixmap大小一样的白色图片
//	QPixmap whitePixmap1(pixmap1.size());
//	whitePixmap1.fill(Qt::white);
//	QPixmap combinedPixmap1(pixmap1.width(), pixmap1.height() * 2);
//	QPainter painter1(&combinedPixmap1);
//	painter1.drawPixmap(0, 0, pixmap1);
//	painter1.drawPixmap(0, pixmap1.height(), whitePixmap1);
//
//	// 创建一个 QLabel 来显示 combinedPixmap1
//	QLabel* animationLabel = new QLabel(ui->widget);
//	animationLabel->setPixmap(combinedPixmap1);
//	animationLabel->setGeometry(
//		(ui->widget->width() - combinedPixmap1.width()) / 2,
//		(ui->widget->height() - combinedPixmap1.height()) / 2,
//		combinedPixmap1.width(),
//		combinedPixmap1.height()
//	);
//	animationLabel->show();
//
//	// 制作动画1
//	QPropertyAnimation* animation = new QPropertyAnimation(animationLabel, "geometry");
//	animation->setDuration(100);
//	animation->setStartValue(
//		QRect(
//			(ui->widget->width() - combinedPixmap1.width()) / 2,
//			0,
//			combinedPixmap1.width(),
//			combinedPixmap1.height()
//		)
//	);
//	animation->setEndValue(animationLabel->geometry());
//	animation->start();
//	connect(animation, &QAbstractAnimation::finished, this, [=]() {
//		delete animationLabel;
//		delete animation;
//		emit toggleRegionSecondAnimation();
//		});
}

void FormBoundaryConditions::initialization(const QString& solver)
{
	//清理之前的内容
	ui->listWidget->clear();

	if (solver == "rhoSimpleFoam" || solver == "buoyantBoussinesqPimpleFoam")
	{
		//获取边界类型
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> patchType = GlobalData::getInstance().getCaseData()->patchType;
		for (const auto& region : patchType)
		{
			QString regionName = QString::fromStdString(region.first);
			for (const auto& patch : region.second)
			{
				QString patchName = QString::fromStdString(patch.first);
				if (regionName == "default")
				{
					if (patchName == "internalMesh")
						continue;

					auto item = new QListWidgetItem(ui->listWidget);
					ui->listWidget->addItem(item);

					auto widget = new CustomItemWidget(5, this, patchName, QString::fromStdString(patch.second), patchName);
					widget->ui_ItemWidgetBoundaryConditions->comboBox->setCurrentIndex(0);
					ui->listWidget->setItemWidget(item, widget);

					connect(widget, &CustomItemWidget::textChanged, this, &FormBoundaryConditions::on_textChanged);
					connect(widget, &CustomItemWidget::typeChanged, this, &FormBoundaryConditions::on_typeChanged);

				}
			}

			int totalHeight = 0;
			for (int i = 0; i < std::min(ui->listWidget->count(), 5); ++i) {
				QListWidgetItem* item = ui->listWidget->item(i);
				if (!item->isHidden()) totalHeight += ui->listWidget->sizeHintForRow(i);
			}
			ui->listWidget->setFixedHeight(totalHeight);
		}
	}
	else if (solver == "chtMultiRegionFoam")
	{
		//获取边界类型
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> patchType = GlobalData::getInstance().getCaseData()->patchType;
		for (const auto& region : patchType)
		{
			QString regionName = QString::fromStdString(region.first);
			for (const auto& patch : region.second)
			{
				QString patchName = QString::fromStdString(patch.first);
				if (regionName != "default")
				{
					if (patchName == "internalMesh")
						continue;

					auto item = new QListWidgetItem(ui->listWidget);
					ui->listWidget->addItem(item);

					auto widget = new CustomItemWidget(5, this, patchName, QString::fromStdString(patch.second), patchName + " in " + regionName);
					widget->ui_ItemWidgetBoundaryConditions->comboBox->setCurrentIndex(0);
					ui->listWidget->setItemWidget(item, widget);

					connect(widget, &CustomItemWidget::textChanged, this, &FormBoundaryConditions::on_textChanged);
					connect(widget, &CustomItemWidget::typeChanged, this, &FormBoundaryConditions::on_typeChanged);

				}
			}
		}

		int totalHeight = 0;
		for (int i = 0; i < std::min(ui->listWidget->count(), 5); ++i) {
			QListWidgetItem* item = ui->listWidget->item(i);
			if (!item->isHidden()) totalHeight += ui->listWidget->sizeHintForRow(i);
		}
		ui->listWidget->setFixedHeight(totalHeight);
	}
}

void FormBoundaryConditions::on_textChanged(CustomItemWidget* widget, QString previousText)
{
}

void FormBoundaryConditions::on_typeChanged(CustomItemWidget* widget, int previousIndex)
{
}