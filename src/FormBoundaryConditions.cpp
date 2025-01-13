#include "FormBoundaryConditions.h"

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
	std::vector<QString> meshEdgeActorsName;
	const auto& meshEdgeActors = GlobalData::getInstance().getCaseData()->meshEdgeActors;
	for (const auto& pair : meshEdgeActors) {
		meshEdgeActorsName.push_back(pair.first);
	}

	listViewModel->clear();

	QIcon icon("../res/meshBoundary.png");
	foreach(const QString & name, meshEdgeActorsName) {
		QStandardItem* item = new QStandardItem(icon, name);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		item->setSizeHint(QSize(0, 40));
		listViewModel->appendRow(item);
	}

	// 计算所有 item 的总高度
	int totalHeight = 0;
	for (int i = 0; i < listViewModel->rowCount(); ++i) {
		totalHeight += ui->listView->sizeHintForRow(i);
	}
	ui->listView->setFixedHeight(totalHeight + 2 * ui->listView->frameWidth());
}

void FormBoundaryConditions::initTabWidget()
{
	std::vector<QString> meshEdgeActorsName;
	const auto& meshEdgeActors = GlobalData::getInstance().getCaseData()->meshEdgeActors;
	for (const auto& pair : meshEdgeActors) {
		meshEdgeActorsName.push_back(pair.first);
	}

	for (int i = ui->tabWidget->count() - 1; i >= 0; --i) {
		ui->tabWidget->removeTab(i);
	}

	for (int i = 0; i < meshEdgeActorsName.size(); ++i) {
		QString meshPartName = meshEdgeActorsName[i];
		ui->tabWidget->addTab(new QWidget(), meshPartName);
		ui->tabWidget->setTabVisible(i, false);	
	}

	for (int i = 0; i < static_cast<int>(PhysicalFields::COUNT); ++i) {
		PhysicalFields field = static_cast<PhysicalFields>(i);
		switch (field) {
		case PhysicalFields::p:

			break;
		case PhysicalFields::T:

			break;
		case PhysicalFields::U:

			break;
		case PhysicalFields::k:

			break;
		case PhysicalFields::nut:

			break;
		case PhysicalFields::omega:

			break;
		case PhysicalFields::alphat:

			break;
		default:
			break;
		}
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

	ui->tabWidget->setStyleSheet(
		"QTabWidget::pane {"
		"    border-top: 1px solid #dcdcdc;"
		"    position: relative;"
		"    top: -1px;"
		"}"
		"QTabBar::tab {"
		"    font-family: 'Microsoft YaHei';"
		"    font-size: 12px;"
		"    min-height: 20px;"
		"    min-width: 80px;"
		"    border: 1px solid #dcdcdc;"
		"    border-bottom: none;"
		"}"
		"QTabBar::tab:selected {"
		"    border-bottom: 1px solid #ffffff;"
		"}"
	);
}