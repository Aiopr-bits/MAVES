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

	connect(ui->listWidget, &QListWidget::itemClicked, this, &FormBoundaryConditions::on_ListWidgetItem_Clicked);
}

FormBoundaryConditions::~FormBoundaryConditions()
{
	delete ui;
}

void FormBoundaryConditions::initialization(const QString& solver, const QString& turbulenceType)
{
	//清理之前的控件
	for (auto& formBoundaryConditionsTabWidget : formBoundaryConditionsTabWidgetsList)
	{
		delete formBoundaryConditionsTabWidget;
	}
	formBoundaryConditionsTabWidgetsList.clear();
	ui->listWidget->clear();

	if (solver == "rhoSimpleFoam")
	{
		//获取边界类型
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> patchType = GlobalData::getInstance().getCaseData()->patchType;
		for (const auto& region : patchType)
		{
			QString regionName = QString::fromStdString(region.first);
			for (const auto& patch : region.second)
			{
				QString patchName = QString::fromStdString(patch.first);
				QString patchType = QString::fromStdString(patch.second);
				if (regionName == "default")
				{
					if (patchName == "internalMesh")
						continue;

					auto item = new QListWidgetItem(ui->listWidget);
					ui->listWidget->addItem(item);

					auto widget = new CustomItemWidget(5, this, patchName, patchType, patchName);
					widget->ui_ItemWidgetBoundaryConditions->comboBox->setCurrentIndex(0);
					ui->listWidget->setItemWidget(item, widget);

					connect(widget, &CustomItemWidget::textChanged, this, &FormBoundaryConditions::on_textChanged);
					connect(widget, &CustomItemWidget::typeChanged, this, &FormBoundaryConditions::on_typeChanged);

					QString boundaryConditionType = widget->ui_ItemWidgetBoundaryConditions->comboBox->currentText();
					FormBoundaryConditionsTabWidget* formBoundaryConditionsTabWidget = new FormBoundaryConditionsTabWidget(this, solver, turbulenceType, patchType, boundaryConditionType);
					int insertPosition = std::max(0, ui->verticalLayout_2->count() - 1);
					ui->verticalLayout_2->insertWidget(insertPosition, formBoundaryConditionsTabWidget);
					formBoundaryConditionsTabWidget->hide();
					formBoundaryConditionsTabWidgetsList.push_back(formBoundaryConditionsTabWidget);
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
	else if (solver == "buoyantBoussinesqPimpleFoam")
	{
		//获取边界类型
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> patchType = GlobalData::getInstance().getCaseData()->patchType;
		for (const auto& region : patchType)
		{
			QString regionName = QString::fromStdString(region.first);
			for (const auto& patch : region.second)
			{
				QString patchName = QString::fromStdString(patch.first);
				QString patchType = QString::fromStdString(patch.second);
				if (regionName == "default")
				{
					if (patchName == "internalMesh")
						continue;

					auto item = new QListWidgetItem(ui->listWidget);
					ui->listWidget->addItem(item);

					auto widget = new CustomItemWidget(5, this, patchName, patchType, patchName);
					widget->ui_ItemWidgetBoundaryConditions->comboBox->setCurrentIndex(0);
					ui->listWidget->setItemWidget(item, widget);

					connect(widget, &CustomItemWidget::textChanged, this, &FormBoundaryConditions::on_textChanged);
					connect(widget, &CustomItemWidget::typeChanged, this, &FormBoundaryConditions::on_typeChanged);

					QString boundaryConditionType = widget->ui_ItemWidgetBoundaryConditions->comboBox->currentText();
					FormBoundaryConditionsTabWidget* formBoundaryConditionsTabWidget = new FormBoundaryConditionsTabWidget(this, solver, turbulenceType, patchType, boundaryConditionType);
					int insertPosition = std::max(0, ui->verticalLayout_2->count() - 1);
					ui->verticalLayout_2->insertWidget(insertPosition, formBoundaryConditionsTabWidget);
					formBoundaryConditionsTabWidget->hide();
					formBoundaryConditionsTabWidgetsList.push_back(formBoundaryConditionsTabWidget);
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
				QString patchType = QString::fromStdString(patch.second);
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

					QString boundaryConditionType = widget->ui_ItemWidgetBoundaryConditions->comboBox->currentText();
					FormBoundaryConditionsTabWidget* formBoundaryConditionsTabWidget = new FormBoundaryConditionsTabWidget(this, solver, turbulenceType, patchType, boundaryConditionType);
					int insertPosition = std::max(0, ui->verticalLayout_2->count() - 1);
					ui->verticalLayout_2->insertWidget(insertPosition, formBoundaryConditionsTabWidget);
					formBoundaryConditionsTabWidget->hide();
					formBoundaryConditionsTabWidgetsList.push_back(formBoundaryConditionsTabWidget);
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

void FormBoundaryConditions::on_ListWidgetItem_Clicked(QListWidgetItem* item)
{
	int index = ui->listWidget->row(item);

	for (auto formBoundaryConditionsTabWidget : formBoundaryConditionsTabWidgetsList)
	{
		formBoundaryConditionsTabWidget->hide();
	}

	if (index >= 0 && index < formBoundaryConditionsTabWidgetsList.size())
	{
		formBoundaryConditionsTabWidgetsList[index]->show();
	}
}

void FormBoundaryConditions::on_textChanged(CustomItemWidget* widget, QString previousText)
{
}

void FormBoundaryConditions::on_typeChanged(CustomItemWidget* widget, int previousIndex)
{
}