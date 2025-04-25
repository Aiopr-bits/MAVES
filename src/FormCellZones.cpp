#include "FormCellZones.h"

FormCellZones::FormCellZones(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormCellZonesClass())
{
	ui->setupUi(this);

	connect(ui->listWidget, &QListWidget::itemClicked, this, &FormCellZones::on_ListWidgetItem_Clicked);
}

FormCellZones::~FormCellZones()
{
	delete ui;
}

void FormCellZones::initialization(const QString& solver, const QString& regionType, const QString& turbulenceType )
{
	//清理之前控件
	for (auto& formCellZonesTabWidget : formCellZonesTabWidgetsList)
	{
		delete formCellZonesTabWidget;
	}
	formCellZonesTabWidgetsList.clear();
	ui->listWidget->clear();

	std::vector<std::string> cellZoneNames = GlobalData::getInstance().getCaseData()->cellZoneNames;
	if (solver == "rhoSimpleFoam"|| solver == "buoyantBoussinesqPimpleFoam")
	{
		cellZoneNames.insert(cellZoneNames.begin(), "all cells");

		for (auto& cellZoneName : cellZoneNames)
		{
			if (cellZoneName == "all cells")
			{
				auto item = new QListWidgetItem(ui->listWidget);
				ui->listWidget->addItem(item);
				auto widget = new CustomItemWidget(6, this, QString::fromStdString(cellZoneName), "0");
				ui->listWidget->setItemWidget(item, widget);

				FormCellZonesTabWidget* formCellZonesTabWidget = new FormCellZonesTabWidget(solver, regionType, turbulenceType, this);
				int insertPosition = std::max(0, ui->verticalLayout_2->count() - 1);
				ui->verticalLayout_2->insertWidget(insertPosition, formCellZonesTabWidget);
				formCellZonesTabWidget->hide();
				formCellZonesTabWidgetsList.push_back(formCellZonesTabWidget);

				connect(widget->ui_ItemWidgetCellZones->pushButton, &QPushButton::toggled, this, [this, formCellZonesTabWidget](bool checked) {
					formCellZonesTabWidget->ui->tabWidget->setTabVisible(3, checked);
					});
				connect(widget->ui_ItemWidgetCellZones->pushButton_2, &QPushButton::toggled, this, [this, formCellZonesTabWidget](bool checked) {
					formCellZonesTabWidget->ui->tabWidget->setTabVisible(2, checked);
					});
				connect(widget->ui_ItemWidgetCellZones->pushButton_3, &QPushButton::toggled, this, [this, formCellZonesTabWidget](bool checked) {
					formCellZonesTabWidget->ui->tabWidget->setTabVisible(1, checked);
					});
				connect(widget->ui_ItemWidgetCellZones->pushButton_4, &QPushButton::toggled, this, [this, formCellZonesTabWidget](bool checked) {
					formCellZonesTabWidget->ui->tabWidget->setTabVisible(0, checked);
					});
			}
			else
			{
				auto item = new QListWidgetItem(ui->listWidget);
				ui->listWidget->addItem(item);
				auto widget = new CustomItemWidget(6, this, QString::fromStdString(cellZoneName), "1");
				ui->listWidget->setItemWidget(item, widget);

				FormCellZonesTabWidget* formCellZonesTabWidget = new FormCellZonesTabWidget(solver, regionType, turbulenceType, this);
				int insertPosition = std::max(0, ui->verticalLayout_2->count() - 1);
				ui->verticalLayout_2->insertWidget(insertPosition, formCellZonesTabWidget);
				formCellZonesTabWidget->hide();
				formCellZonesTabWidgetsList.push_back(formCellZonesTabWidget);

				connect(widget->ui_ItemWidgetCellZones->pushButton, &QPushButton::toggled, this, [this, formCellZonesTabWidget](bool checked) {
					formCellZonesTabWidget->ui->tabWidget->setTabVisible(3, checked);
					});
				connect(widget->ui_ItemWidgetCellZones->pushButton_2, &QPushButton::toggled, this, [this, formCellZonesTabWidget](bool checked) {
					formCellZonesTabWidget->ui->tabWidget->setTabVisible(2, checked);
					});
				connect(widget->ui_ItemWidgetCellZones->pushButton_3, &QPushButton::toggled, this, [this, formCellZonesTabWidget](bool checked) {
					formCellZonesTabWidget->ui->tabWidget->setTabVisible(1, checked);
					});
				connect(widget->ui_ItemWidgetCellZones->pushButton_4, &QPushButton::toggled, this, [this, formCellZonesTabWidget](bool checked) {
					formCellZonesTabWidget->ui->tabWidget->setTabVisible(0, checked);
					});
			}
		}
	}
	else if (solver == "chtMultiRegionFoam")
	{
		for (auto& cellZoneName : cellZoneNames)
		{
			cellZoneName = "all cells in " + cellZoneName;
		}

		for (auto& cellZoneName : cellZoneNames)
		{
			auto item = new QListWidgetItem(ui->listWidget);
			ui->listWidget->addItem(item);
			auto widget = new CustomItemWidget(6, this, QString::fromStdString(cellZoneName), "0");
			ui->listWidget->setItemWidget(item, widget);

			FormCellZonesTabWidget* formCellZonesTabWidget = new FormCellZonesTabWidget(solver, regionType, turbulenceType, this);
			int insertPosition = std::max(0, ui->verticalLayout_2->count() - 1);
			ui->verticalLayout_2->insertWidget(insertPosition, formCellZonesTabWidget);
			formCellZonesTabWidget->hide();
			formCellZonesTabWidgetsList.push_back(formCellZonesTabWidget);

			connect(widget->ui_ItemWidgetCellZones->pushButton, &QPushButton::toggled, this, [this, formCellZonesTabWidget](bool checked) {
				formCellZonesTabWidget->ui->tabWidget->setTabVisible(3, checked);
				});
			connect(widget->ui_ItemWidgetCellZones->pushButton_2, &QPushButton::toggled, this, [this, formCellZonesTabWidget](bool checked) {
				formCellZonesTabWidget->ui->tabWidget->setTabVisible(2, checked);
				});
			connect(widget->ui_ItemWidgetCellZones->pushButton_3, &QPushButton::toggled, this, [this, formCellZonesTabWidget](bool checked) {
				formCellZonesTabWidget->ui->tabWidget->setTabVisible(1, checked);
				});
			connect(widget->ui_ItemWidgetCellZones->pushButton_4, &QPushButton::toggled, this, [this, formCellZonesTabWidget](bool checked) {
				formCellZonesTabWidget->ui->tabWidget->setTabVisible(0, checked);
				});
		}
	}

	int totalHeight = 0;
	for (int i = 0; i < std::min(ui->listWidget->count(), 5); ++i)
	{
		totalHeight += ui->listWidget->sizeHintForRow(i);
	}
	ui->listWidget->setFixedHeight(totalHeight);
	int labelHeight = ui->label_2->sizeHint().height();	
	ui->widget_2->setFixedHeight(labelHeight + totalHeight+2); 
}

void FormCellZones::on_ListWidgetItem_Clicked(QListWidgetItem* item)
{
	int index = ui->listWidget->row(item);

	for (auto formCellZonesTabWidget : formCellZonesTabWidgetsList)
	{
		formCellZonesTabWidget->hide();
	}

	if (index >= 0 && index < formCellZonesTabWidgetsList.size())
	{
		formCellZonesTabWidgetsList[index]->show();
	}
}
