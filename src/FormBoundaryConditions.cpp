#include "FormBoundaryConditions.h"
#include <QTextStream>
#include <qdebug.h>

FormBoundaryConditions::FormBoundaryConditions(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::FormBoundaryConditionsClass())
{
	ui->setupUi(this);

	listViewModel = new QStandardItemModel(this);
	ui->listView->setModel(listViewModel);
	ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->tabWidget->hide();

	connect(ui->listView, &QListView::clicked, this, &FormBoundaryConditions::onListViewItemClicked);
}

FormBoundaryConditions::~FormBoundaryConditions()
{
	delete ui;
}

void FormBoundaryConditions::onMeshImported()
{
	initListView();
	initTabWidget();
}

void FormBoundaryConditions::initListView()
{
	listViewModel->clear();
	std::vector<QString> meshEdgeActorsName;
	const auto& meshEdgeActors = GlobalData::getInstance().getCaseData()->meshEdgeActors;
	for (const auto& pair : meshEdgeActors) {
		meshEdgeActorsName.push_back(pair.first);
	}

	QIcon icon("../res/meshBoundary.png");
	foreach(const QString & name, meshEdgeActorsName) {
		QStandardItem* item = new QStandardItem(icon, name);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		item->setSizeHint(QSize(0, 40));
		listViewModel->appendRow(item);
	}

	// �������� item ���ܸ߶�
	int totalHeight = 0;
	for (int i = 0; i < listViewModel->rowCount(); ++i) {
		totalHeight += ui->listView->sizeHintForRow(i);
	}
	ui->listView->setFixedHeight(totalHeight + 2 * ui->listView->frameWidth());
}

void FormBoundaryConditions::initTabWidget()
{
	//�Ƴ� ui->tabWidget �е����� tab
	for (int i = ui->tabWidget->count() - 1; i >= 0; --i) {
		ui->tabWidget->removeTab(i);
	}

	// ��ȡ���� meshEdgeActors ������
	std::vector<QString> meshEdgeActorsName;
	const auto& meshEdgeActors = GlobalData::getInstance().getCaseData()->meshEdgeActors;
	for (const auto& pair : meshEdgeActors) {
		meshEdgeActorsName.push_back(pair.first);
	}

	// Ϊÿ�� meshEdgeActor ����һ���µ� tab����������ӵ� ui->tabWidget ��
	for (int i = 0; i < meshEdgeActorsName.size(); ++i) {
		QString meshPartName = meshEdgeActorsName[i];
		QWidget* tabPage = new QWidget();
		FormBoundaryConditionsTabWidget* tabWidget = new FormBoundaryConditionsTabWidget();
		ui->tabWidget->addTab(tabWidget, meshPartName);
		ui->tabWidget->setTabVisible(i, false);
	}
}

void FormBoundaryConditions::initBoundaryConditions()
{
	std::string solverName = GlobalData::getInstance().getCaseData()->solverName;
	const auto& meshFaceActors = GlobalData::getInstance().getCaseData()->meshFaceActors;

	std::vector<QString> physicalFields;
	if (solverName == "rhoSimpleFoam") { // ������
		physicalFields = { "p", "T", "U", "k", "nut", "omega", "alphat" };
	}
	else if (solverName == "buoyantBoussinesqPimpleFoam") { // ������
		physicalFields = { "p", "T", "U", "p_rgh", "nut", "k", "epsilon", "alphat" };
	}

	std::vector<QString> boundaryField;
	for (const auto& pair : meshFaceActors) {
		boundaryField.push_back(pair.first);
	}

	boundaryConditions.clear();
	for (const auto& physicalField : physicalFields) {
		QMap<QString, QVector<QString>> boundaryCondition;
		for (const auto& field : boundaryField) {
			boundaryCondition[field] = QVector<QString>(2, "");
		}
		boundaryConditions[physicalField] = boundaryCondition;
	}
}

void FormBoundaryConditions::parseBoundaryFile(const QString& filePath, const QString& fieldName)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, "����", "�޷����ļ�: " + filePath);
		return;
	}

	QTextStream in(&file);
	QString content = in.readAll();
	content.replace("uniform", "");
	file.close();

	// ��ȡ boundaryField ֮ǰ������
	QRegularExpression boundaryFieldRegex(R"(boundaryField\s*\{)");
	QRegularExpressionMatch boundaryFieldMatch = boundaryFieldRegex.match(content);
	if (!boundaryFieldMatch.hasMatch()) {
		QMessageBox::warning(this, "����", "�޷������ļ�: " + filePath);
		return;
	}

	QString preBoundaryFieldContent = content.left(boundaryFieldMatch.capturedStart());

	// ��ȡ��������
	QMap<QString, QString> variables;
	QRegularExpression variableRegex(R"((\w+)\s+([\w\d\.\+\-eE\(\)\s\$]+);)");
	QRegularExpressionMatchIterator varIt = variableRegex.globalMatch(preBoundaryFieldContent);
	while (varIt.hasNext()) {
		QRegularExpressionMatch varMatch = varIt.next();
		QString varName = varMatch.captured(1);
		QString varValue = varMatch.captured(2);
		variables[varName] = varValue;
	}

	// �ݹ��������ֵ
	auto resolveVariable = [&variables](const QString& value) -> QString {
		QString resolvedValue = value;
		QRegularExpression varRefRegex(R"(\$(\w+))");
		QRegularExpressionMatchIterator varRefIt = varRefRegex.globalMatch(value);
		while (varRefIt.hasNext()) {
			QRegularExpressionMatch varRefMatch = varRefIt.next();
			QString varRefName = varRefMatch.captured(1);
			if (variables.contains(varRefName)) {
				resolvedValue.replace("$" + varRefName, variables[varRefName]);
			}
		}
		return resolvedValue;
		};

	// �������б�����ֵ
	for (auto it = variables.begin(); it != variables.end(); ++it) {
		it.value() = resolveVariable(it.value());
	}

	QRegularExpression boundaryFieldRegex2(R"(boundaryField\s*\{([\s\S]*?)\n\})");
	QRegularExpressionMatch match = boundaryFieldRegex2.match(content);
	if (!match.hasMatch()) {
		QMessageBox::warning(this, "����", "�޷������ļ�: " + filePath);
		return;
	}

	QString boundaryFieldContent = match.captured(1);
	QRegularExpression varRefRegex2(R"(\$(\w+))");
	QRegularExpressionMatchIterator varRefIt2 = varRefRegex2.globalMatch(boundaryFieldContent);
	while (varRefIt2.hasNext()) {
		QRegularExpressionMatch varRefMatch = varRefIt2.next();
		QString varRefName = varRefMatch.captured(1);
		if (variables.contains(varRefName)) {
			boundaryFieldContent.replace("$" + varRefName, variables[varRefName]);
		}
	}

	QRegularExpression boundaryRegex(R"((\w+)\s*\{([\s\S]*?)\n\s*\})");
	QRegularExpressionMatchIterator it = boundaryRegex.globalMatch(boundaryFieldContent);

	while (it.hasNext()) {
		QRegularExpressionMatch boundaryMatch = it.next();
		QString boundaryName = boundaryMatch.captured(1);
		QString boundaryContent = boundaryMatch.captured(2);

		QRegularExpression typeRegex(R"(type\s+([\w:]+);)");
		QRegularExpression valueRegex(R"(value\s+([\w\d\.\+\-eE\(\)\s\$]+);)");

		QRegularExpressionMatch typeMatch = typeRegex.match(boundaryContent);
		QRegularExpressionMatch valueMatch = valueRegex.match(boundaryContent);

		QString type = typeMatch.hasMatch() ? typeMatch.captured(1) : "";
		QString value = valueMatch.hasMatch() ? valueMatch.captured(1) : "";

		//���⴦��
		if (type == "compressible::alphatWallFunction")type = "alphatWallFunction";
		if (type == "externalWallHeatFluxTemperature")type = "heatFluxTemperature";

		if (boundaryConditions.contains(fieldName) && boundaryConditions[fieldName].contains(boundaryName)) {
			boundaryConditions[fieldName][boundaryName][0] = type;
			boundaryConditions[fieldName][boundaryName][1] = value;
		}
	}
}

void FormBoundaryConditions::importParameter()
{
	// ��ȡ����·��
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QString caseDirPath = QFileInfo(casePath).absolutePath();

	std::string solverName = GlobalData::getInstance().getCaseData()->solverName;
	std::vector<QString> fileNames;
	if (solverName == "rhoSimpleFoam") { // ������
		fileNames = { "p", "T", "U", "k", "nut", "omega", "alphat" };
	}
	else if (solverName == "buoyantBoussinesqPimpleFoam") { // ������
		fileNames = { "p", "T", "U", "p_rgh", "nut", "k", "epsilon", "alphat" };
	}

	// ��ʼ���߽�����
	initBoundaryConditions();

	// ����ÿ���ļ������±߽�����
	for (const QString& fileName : fileNames) {
		QString filePath = caseDirPath + "/0/" + fileName;
		parseBoundaryFile(filePath, fileName);
	}

	// ����ÿ�� tab ҳ�� comboBox ѡ����
	for (int i = 0; i < ui->tabWidget->count(); ++i) {
		QString tabName = ui->tabWidget->tabText(i);
		FormBoundaryConditionsTabWidget* tabWidget = qobject_cast<FormBoundaryConditionsTabWidget*>(ui->tabWidget->widget(i));

		auto updateComboBox = [&](QComboBox* comboBox, const QString& field) {
			for (int j = 0; j < comboBox->count(); ++j) {
				if (comboBox->itemText(j) == boundaryConditions[field][tabName][0]) {
					comboBox->setCurrentIndex(j);
					break;
				}
			}
			};

		updateComboBox(tabWidget->ui->comboBox, "p");
		updateComboBox(tabWidget->ui->comboBox_2, "T");
		updateComboBox(tabWidget->ui->comboBox_3, "U");
		updateComboBox(tabWidget->ui->comboBox_4, "k");
		updateComboBox(tabWidget->ui->comboBox_5, "nut");
		updateComboBox(tabWidget->ui->comboBox_6, "omega");
		updateComboBox(tabWidget->ui->comboBox_7, "alphat");

		QStringList specialTypes = { "fixedValue", "heatFluxTemperature", "inletOutlet", "kqRWallFunction", "nutkWallFunction", "omegaWallFunction", "alphatWallFunction", "calculated" };

		auto updateLineEdit = [&](QComboBox* comboBox, QLineEdit* lineEdit, const QString& field) {
			if (specialTypes.contains(comboBox->currentText())) {
				lineEdit->setText(boundaryConditions[field][tabName][1]);
			}
			};

		updateLineEdit(tabWidget->ui->comboBox, tabWidget->ui->lineEdit, "p");
		updateLineEdit(tabWidget->ui->comboBox_2, tabWidget->ui->lineEdit_2, "T");

		if (specialTypes.contains(tabWidget->ui->comboBox_3->currentText())) {
			QString value = boundaryConditions["U"][tabName][1];
			QRegularExpression re(R"(\(([^)]+)\))");
			QRegularExpressionMatch match = re.match(value);
			if (match.hasMatch()) {
				QStringList valueList = match.captured(1).split(" ");
				if (valueList.size() == 3) {
					tabWidget->ui->lineEdit_8->setText(valueList[0]);
					tabWidget->ui->lineEdit_9->setText(valueList[1]);
					tabWidget->ui->lineEdit_10->setText(valueList[2]);
				}
			}
		}

		updateLineEdit(tabWidget->ui->comboBox_4, tabWidget->ui->lineEdit_4, "k");
		updateLineEdit(tabWidget->ui->comboBox_5, tabWidget->ui->lineEdit_5, "nut");
		updateLineEdit(tabWidget->ui->comboBox_6, tabWidget->ui->lineEdit_6, "omega");
		updateLineEdit(tabWidget->ui->comboBox_7, tabWidget->ui->lineEdit_7, "alphat");
	}
}

void FormBoundaryConditions::exportParameter()
{
	// ��ȡ����·��
	QString casePath = GlobalData::getInstance().getCaseData()->casePath.c_str();
	QString caseDirPath = QFileInfo(casePath).absolutePath();
	QStringList fileNames = { "p", "T", "U", "k", "nut", "omega", "alphat" };

	// �������ϵ��޸�д�� boundaryConditions ��
	for (int i = 0; i < ui->tabWidget->count(); ++i) {
		QString tabName = ui->tabWidget->tabText(i);
		FormBoundaryConditionsTabWidget* tabWidget = qobject_cast<FormBoundaryConditionsTabWidget*>(ui->tabWidget->widget(i));

		auto updateBoundaryConditions = [&](QComboBox* comboBox, QLineEdit* lineEdit, const QString& field) {
			boundaryConditions[field][tabName][0] = comboBox->currentText();

			// ���⴦��
			if (boundaryConditions[field][tabName][0] == "alphatWallFunction") boundaryConditions[field][tabName][0] = "compressible::alphatWallFunction";
			if (boundaryConditions[field][tabName][0] == "heatFluxTemperature") boundaryConditions[field][tabName][0] = "externalWallHeatFluxTemperature";

			if (!lineEdit->text().isEmpty()) {
				boundaryConditions[field][tabName][1] = lineEdit->text();
			}
			};

		updateBoundaryConditions(tabWidget->ui->comboBox, tabWidget->ui->lineEdit, "p");
		updateBoundaryConditions(tabWidget->ui->comboBox_2, tabWidget->ui->lineEdit_2, "T");

		boundaryConditions["U"][tabName][0] = tabWidget->ui->comboBox_3->currentText();
		QString value = QString("(%1 %2 %3)").arg(tabWidget->ui->lineEdit_8->text(), tabWidget->ui->lineEdit_9->text(), tabWidget->ui->lineEdit_10->text());
		boundaryConditions["U"][tabName][1] = value;

		updateBoundaryConditions(tabWidget->ui->comboBox_4, tabWidget->ui->lineEdit_4, "k");
		updateBoundaryConditions(tabWidget->ui->comboBox_5, tabWidget->ui->lineEdit_5, "nut");
		updateBoundaryConditions(tabWidget->ui->comboBox_6, tabWidget->ui->lineEdit_6, "omega");
		updateBoundaryConditions(tabWidget->ui->comboBox_7, tabWidget->ui->lineEdit_7, "alphat");
	}

	// ����ÿ�������ļ�
	for (const QString& fileName : fileNames) {
		QString filePath = caseDirPath + "/0/" + fileName;
		QFile file(filePath);
		if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
			QMessageBox::warning(this, "����", "�޷����ļ�: " + filePath);
			continue;
		}

		QTextStream in(&file);
		QString content = in.readAll();
		file.close();

		// ��ȡ boundaryField ֮ǰ������
		QRegularExpression boundaryFieldRegex(R"(boundaryField\s*\{)");
		QRegularExpressionMatch boundaryFieldMatch = boundaryFieldRegex.match(content);
		if (!boundaryFieldMatch.hasMatch()) {
			QMessageBox::warning(this, "����", "�޷������ļ�: " + filePath);
			return;
		}
		QString preBoundaryFieldContent = content.left(boundaryFieldMatch.capturedStart());

		// �� preBoundaryFieldContent ��д�� boundaryField �Ĳ���
		QStringList specialTypes = { "fixedValue", "externalWallHeatFluxTemperature", "inletOutlet", "kqRWallFunction", "nutkWallFunction", "omegaWallFunction", "compressible::alphatWallFunction", "calculated" };
		preBoundaryFieldContent += "boundaryField\n{\n";
		const auto& boundaryField = boundaryConditions[fileName];
		for (auto it = boundaryField.begin(); it != boundaryField.end(); ++it) {
			preBoundaryFieldContent += "    " + it.key() + "\n";
			preBoundaryFieldContent += "    {\n";
			preBoundaryFieldContent += "        type            " + it.value()[0] + ";\n";
			if (it.value()[0] == "externalWallHeatFluxTemperature") {
				preBoundaryFieldContent += "        mode            coefficient;\n";
				preBoundaryFieldContent += "        Ta              constant 300.0;\n";
				preBoundaryFieldContent += "        h               uniform 100.0;\n";
				preBoundaryFieldContent += "        thicknessLayers (0.001);\n";
				preBoundaryFieldContent += "        kappaLayers     (1);\n";
				preBoundaryFieldContent += "        kappaMethod     fluidThermo;\n";
			}
			if (it.value()[0] == "inletOutlet") {
				preBoundaryFieldContent += "        inletValue      uniform " + it.value()[1] + ";\n";
			}
			if (specialTypes.contains(it.value()[0])) {
				preBoundaryFieldContent += "        value           uniform " + it.value()[1] + ";\n";
			}
			preBoundaryFieldContent += "    }\n";
		}
		preBoundaryFieldContent += "}\n";

		// �� preBoundaryFieldContent д���ļ����滻ԭ�����ļ�����
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
			QMessageBox::warning(this, "����", "�޷����ļ�: " + filePath);
			continue;
		}
		QTextStream out(&file);
		out << preBoundaryFieldContent;
		file.close();
	}
}

void FormBoundaryConditions::onListViewItemClicked(const QModelIndex& index)
{
	ui->tabWidget->show();
	QString itemName = listViewModel->data(index, Qt::DisplayRole).toString();
	for (int i = 0; i < ui->tabWidget->count(); ++i) {
		ui->tabWidget->setTabVisible(i, false);
	}
	for (int i = 0; i < ui->tabWidget->count(); ++i) {
		if (ui->tabWidget->tabText(i) == itemName) {
			ui->tabWidget->setTabVisible(i, true); 
			ui->tabWidget->setCurrentIndex(i);
			break;
		}
	}

	//ui->verticalSpacer_3����
	ui->verticalSpacer_3->changeSize(20, 0, QSizePolicy::Minimum, QSizePolicy::Fixed);
}