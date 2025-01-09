#include "FormBoundaryConditions.h"

FormBoundaryConditions::FormBoundaryConditions(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::FormBoundaryConditionsClass())
{
	ui->setupUi(this);

	listViewModel = new QStandardItemModel(this);
	ui->listView->setModel(listViewModel);
	ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);

	// ���� listView ����ʽ��
	ui->listView->setStyleSheet(
		"QListView {"
		"    background-color: white;" // ����Ԫ�ر�����ɫΪ��ɫ
		"    border: 1px solid rgb(230, 230, 230);" // ���ñ߿���ɫ
		"    show-decoration-selected: 1;" // ��ʾѡ�����װ��
		"}"
		"QListView::item {"
		"    border-bottom: 1px solid rgb(230, 230, 230);" // ����Ԫ��֮��ĺ�����ɫ
		"}"
		"QHeaderView::section {"
		"    background-color: rgb(230, 230, 230);" // ���ñ��ⱳ����ɫ
		"    border: 1px solid rgb(230, 230, 230);" // ���ñ���߿���ɫ
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

