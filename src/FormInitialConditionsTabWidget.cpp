#include "FormInitialConditionsTabWidget.h"

FormInitialConditionsTabWidget::FormInitialConditionsTabWidget(const QString& solver, const QString& regionType, const QString& turbulenceType, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FormInitialConditionsTabWidgetClass())
{
	ui->setupUi(this);
	initialization(solver, regionType, turbulenceType);
}

FormInitialConditionsTabWidget::~FormInitialConditionsTabWidget()
{
	delete ui;
}

void FormInitialConditionsTabWidget::resizeEvent(QResizeEvent* event)
{
	ui->tabWidget->tabBar()->setFixedWidth(ui->tabWidget->width());
}

void FormInitialConditionsTabWidget::initialization(const QString& solver, const QString& regionType, const QString& turbulenceType)
{
	if (solver == "rhoSimpleFoam")
	{
		ui->tabWidget->tabBar()->show();
		ui->tabWidget->setCurrentIndex(0);

		if (regionType == "fluid")
		{
			ui->widget->hide();
			ui->widget_2->hide();
			ui->widget_5->hide();
			ui->widget_6->hide();
			ui->widget_7->hide();
			ui->widget_8->hide();
			ui->widget_9->hide();
			ui->widget_10->hide();
			ui->widget_11->hide();
			ui->widget_21->hide();
			ui->widget_32->hide();
			ui->widget_33->hide();
			ui->widget_34->hide();

			if (turbulenceType == "Laminar")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
			}
			else if (turbulenceType == "k-ε")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_9->show();
				ui->widget_21->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "RNG k-ε")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_9->show();
				ui->widget_21->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "Realizable k-ε")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_9->show();
				ui->widget_21->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "Spalart-Allmaras")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_8->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "k-ω")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "k-ω SST")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "k-ω SST SAS")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "Buoyant k-ε")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_9->show();
				ui->widget_21->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "k-kl-ω")
			{

			}
			else if (turbulenceType == "k-ω SST DDES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "k-ω SST IDDES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "k-ω SST DES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "Spalart-Allmaras DES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_8->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "Spalart-Allmaras DDES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_8->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "Spalart-Allmaras IDDES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_8->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "Smagorinsky")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "WALE")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "k Equation")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_9->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "Dynamic k Equation")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_9->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
		}
	}
	else if (solver == "buoyantBoussinesqPimpleFoam")
	{
		ui->tabWidget->tabBar()->show();
		ui->tabWidget->setCurrentIndex(0);

		if (regionType == "fluid")
		{
			ui->widget->hide();
			ui->widget_2->hide();
			ui->widget_5->hide();
			ui->widget_6->hide();
			ui->widget_7->hide();
			ui->widget_8->hide();
			ui->widget_9->hide();
			ui->widget_10->hide();
			ui->widget_11->hide();
			ui->widget_21->hide();
			ui->widget_32->hide();
			ui->widget_33->hide();
			ui->widget_34->hide();

			if (turbulenceType == "Laminar")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
			}
			else if (turbulenceType == "k-ε")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_21->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "RNG k-ε")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_21->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Realizable k-ε")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_21->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Spalart-Allmaras")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_8->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k-ω")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k-ω SST")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k-ω SST SAS")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Buoyant k-ε")
			{
				
			}
			else if (turbulenceType == "k-kl-ω")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_10->show();
				ui->widget_11->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k-ω SST DDES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k-ω SST IDDES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k-ω SST DES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Spalart-Allmaras DES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_8->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Spalart-Allmaras DDES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_8->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Spalart-Allmaras IDDES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_8->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Smagorinsky")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "WALE")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k Equation")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "Dynamic k Equation")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
		}
	}
	else if (solver == "chtMultiRegionFoam")
	{
		ui->tabWidget->tabBar()->hide();
		ui->tabWidget->setCurrentIndex(0);

		if (regionType == "fluid")
		{
			ui->widget->hide();
			ui->widget_2->hide();
			ui->widget_5->hide();
			ui->widget_6->hide();
			ui->widget_7->hide();
			ui->widget_8->hide();
			ui->widget_9->hide();
			ui->widget_10->hide();
			ui->widget_11->hide();
			ui->widget_21->hide();
			ui->widget_32->hide();
			ui->widget_33->hide();
			ui->widget_34->hide();

			if (turbulenceType == "Laminar")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
			}
			else if (turbulenceType == "k-ε")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_21->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "RNG k-ε")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_21->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Realizable k-ε")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_21->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Spalart-Allmaras")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_8->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k-ω")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k-ω SST")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k-ω SST SAS")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Buoyant k-ε")
			{

			}
			else if (turbulenceType == "k-kl-ω")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_10->show();
				ui->widget_11->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k-ω SST DDES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k-ω SST IDDES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k-ω SST DES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_32->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Spalart-Allmaras DES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_8->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Spalart-Allmaras DDES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_8->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Spalart-Allmaras IDDES")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_8->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "Smagorinsky")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "WALE")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_33->show();
			}
			else if (turbulenceType == "k Equation")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
			else if (turbulenceType == "Dynamic k Equation")
			{
				ui->widget_2->show();
				ui->widget_5->show();
				ui->widget_6->show();
				ui->widget_7->show();
				ui->widget_9->show();
				ui->widget_33->show();
				ui->widget_34->show();
			}
		}
		else if (regionType == "solid")
		{
			ui->widget->hide();
			ui->widget_2->hide();
			ui->widget_5->hide();
			ui->widget_6->hide();
			ui->widget_7->hide();
			ui->widget_8->hide();
			ui->widget_9->hide();
			ui->widget_10->hide();
			ui->widget_11->hide();
			ui->widget_21->hide();
			ui->widget_32->hide();
			ui->widget_33->hide();
			ui->widget_34->hide();

			if (turbulenceType == "Laminar")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "k-ε")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "RNG k-ε")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "Realizable k-ε")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "Spalart-Allmaras")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "k-ω")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "k-ω SST")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "k-ω SST SAS")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "Buoyant k-ε")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "k-ω SST DDES")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "k-ω SST IDDES")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "k-ω SST DES")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "Spalart-Allmaras DES")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "Spalart-Allmaras DDES")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "Spalart-Allmaras IDDES")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "Smagorinsky")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "WALE")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "k Equation")
			{
				ui->widget_6->show();
			}
			else if (turbulenceType == "Dynamic k Equation")
			{
				ui->widget_6->show();
			}
		}
	}
}
