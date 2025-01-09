#include "FormBoundaryConditions.h"

FormBoundaryConditions::FormBoundaryConditions(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::FormBoundaryConditionsClass())
{
	ui->setupUi(this);

	listViewModel = new QStandardItemModel(this);
	ui->listView->setModel(listViewModel);
	ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);

	// 设置 listView 的样式表
	ui->listView->setStyleSheet(
		"QListView {"
		"    background-color: white;" // 设置元素背景颜色为白色
		"    border: 1px solid rgb(230, 230, 230);" // 设置边框颜色
		"    show-decoration-selected: 1;" // 显示选中项的装饰
		"}"
		"QListView::item {"
		"    border-bottom: 1px solid rgb(230, 230, 230);" // 设置元素之间的横线颜色
		"}"
		"QHeaderView::section {"
		"    background-color: rgb(230, 230, 230);" // 设置标题背景颜色
		"    border: 1px solid rgb(230, 230, 230);" // 设置标题边框颜色
		"}"
	);
}

FormBoundaryConditions::~FormBoundaryConditions()
{
	delete ui;
}

void FormBoundaryConditions::onMeshImported()
{
	std::vector<QString> meshEdgeActorsName;
	const auto& meshEdgeActors = GlobalData::getInstance().getCaseData()->meshEdgeActors;
	for (const auto& pair : meshEdgeActors) {
		meshEdgeActorsName.push_back(pair.first);
	}

	listViewModel->clear();
	foreach(const QString & name, meshEdgeActorsName) {
		QStandardItem* item = new QStandardItem(name);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		item->setSizeHint(QSize(0, 40));
		listViewModel->appendRow(item);
	}
}

