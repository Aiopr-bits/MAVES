#include "FormInitialConditions.h"

FormInitialConditions::FormInitialConditions(QWidget *parent)
: QWidget(parent)
, ui(new Ui::FormInitialConditionsClass())
{
	ui->setupUi(this);
	initialization("rhoSimpleFoam",  "fluid", "Laminar");

	connect(ui->listWidget, &QListWidget::itemClicked, this, &FormInitialConditions::on_ListWidgetItem_Clicked);
}

FormInitialConditions::~FormInitialConditions()
{
	delete ui;
}

void FormInitialConditions::on_ListWidgetItem_Clicked(QListWidgetItem* item)
{
	int index = ui->listWidget->row(item);

	for (auto formInitialConditionsTabWidget : formInitialConditionsTabWidgetsList)
	{
		formInitialConditionsTabWidget->hide();
	}

	if (index >= 0 && index < formInitialConditionsTabWidgetsList.size())
	{
		formInitialConditionsTabWidgetsList[index]->show();
	}
}

void FormInitialConditions::initialization(const QString& solver, const QString& regionType, const QString& turbulenceType)
{
	//清理之前的控件
	for (auto& formInitialConditionsTabWidget : formInitialConditionsTabWidgetsList)
	{
		delete formInitialConditionsTabWidget;
	}
	formInitialConditionsTabWidgetsList.clear();
	ui->listWidget->clear();

	if (solver == "rhoSimpleFoam")
	{
		ui->widget_2->hide();

		FormInitialConditionsTabWidget* formInitialConditionsTabWidget = new FormInitialConditionsTabWidget(solver, regionType, turbulenceType, this);
		int insertPosition = std::max(0, ui->verticalLayout_2->count() - 1);
		ui->verticalLayout_2->insertWidget(insertPosition, formInitialConditionsTabWidget);
		formInitialConditionsTabWidgetsList.push_back(formInitialConditionsTabWidget);
	}
	else if (solver == "buoyantBoussinesqPimpleFoam")
	{
		ui->widget_2->hide();

		FormInitialConditionsTabWidget* formInitialConditionsTabWidget = new FormInitialConditionsTabWidget(solver, regionType, turbulenceType, this);
		int insertPosition = std::max(0, ui->verticalLayout_2->count() - 1);
		ui->verticalLayout_2->insertWidget(insertPosition, formInitialConditionsTabWidget);
		formInitialConditionsTabWidgetsList.push_back(formInitialConditionsTabWidget);
	}
	else if (solver == "chtMultiRegionFoam")
	{
		ui->widget_2->show();

		//获取全局区域类型
		std::unordered_map < std::string, std::string> regionsType = GlobalData::getInstance().getCaseData()->regionsType;
		for (const auto& region : regionsType)
		{
			QString regionName = QString::fromStdString(region.first);
			QString regionType = QString::fromStdString(region.second);
			QIcon icon;

			if (regionType == "solid")
			{
				icon.addFile("../res/Solid.png");
			}
			else if (regionType == "fluid")
			{
				icon.addFile("../res/Fluid.png");
			}

			FormInitialConditionsTabWidget* formInitialConditionsTabWidget = new FormInitialConditionsTabWidget(solver, regionType, turbulenceType, this);
			int insertPosition = std::max(0, ui->verticalLayout_2->count() - 1);
			ui->verticalLayout_2->insertWidget(insertPosition, formInitialConditionsTabWidget);
			formInitialConditionsTabWidget->hide();
			formInitialConditionsTabWidgetsList.push_back(formInitialConditionsTabWidget);

			QListWidgetItem* item = new QListWidgetItem(icon, regionName);
			ui->listWidget->addItem(item);
		}

		int totalHeight = 0;
		for (int i = 0; i < std::min(ui->listWidget->count(), 5); ++i)
		{
			totalHeight += ui->listWidget->sizeHintForRow(i);
		}
		ui->listWidget->setFixedHeight(totalHeight);
		int labelHeight = ui->label_2->sizeHint().height();
		ui->widget_2->setFixedHeight(totalHeight + labelHeight);
	}
}
