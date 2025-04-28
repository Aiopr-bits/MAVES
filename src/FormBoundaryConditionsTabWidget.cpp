/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#include "FormBoundaryConditionsTabWidget.h"

FormBoundaryConditionsTabWidget::FormBoundaryConditionsTabWidget(
	QWidget *parent, 
	const QString& solver, 
	const QString& turbulenceType, 
	const QString& boundaryType, 
	const QString& boundaryConditionType)
	: QWidget(parent)
	, ui(new Ui::FormBoundaryConditionsTabWidgetClass())
{
	ui->setupUi(this);
	initialization(solver, turbulenceType, boundaryType, boundaryConditionType);

	// 输入限制
	QRegularExpression regex("^-?(\\d+(\\.\\d*)?|\\.\\d+)([eE][-+]?\\d+)?$");
	QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, this);
	const auto lineEdits = this->findChildren<QLineEdit*>();
	for (QLineEdit* lineEdit : lineEdits) {
		lineEdit->setValidator(validator);
	}
}

FormBoundaryConditionsTabWidget::~FormBoundaryConditionsTabWidget()
{
	delete ui;
}

void FormBoundaryConditionsTabWidget::on_comboBox_14_itemActivated(int index)
{
	QString text = ui->comboBox_14->currentText();

	ui->widget_2->hide();
	ui->widget_3->hide();
	ui->widget_4->hide();
	ui->widget_5->hide();
	ui->widget_6->hide();
	ui->widget_7->hide();
	ui->widget_8->hide();
	ui->widget_9->hide();
	ui->widget_10->hide();

	if (text == "Fixed Value")
	{
		ui->widget_2->show();
	}
	else if (text == "Fixed Mean Value")
	{
		ui->widget_3->show();
	}
	else if (text == "Uniform Fixed Value")
	{
		ui->widget_4->show();
	}
	else if (text == "Uniform Fixed Gradient")
	{
		ui->widget_5->show();
	}
	else if (text == "Fixed Gradient")
	{
		ui->widget_6->show();
	}
	else if (text == "Total Pressure")
	{
		ui->widget_7->show();
	}
	else if (text == "Uniform Total Pressure")
	{
		ui->widget_8->show();
	}
	else if (text == "Fan")
	{
		ui->widget_9->show();
	}
	else if (text == "Free Stream Pressure")
	{
		ui->widget_10->show();
	}
}

void FormBoundaryConditionsTabWidget::on_comboBox_16_itemActivated(int index)
{
	QString text = ui->comboBox_16->currentText();

	ui->widget_13->hide();
	ui->widget_14->hide();
	ui->widget_15->hide();
	ui->widget_16->hide();
	ui->widget_17->hide();
	ui->widget_18->hide();
	ui->widget_19->hide();
	ui->widget_20->hide();
	ui->widget_21->hide();
	ui->widget_22->hide();

	if (text == "Fixed Flux Pressure")
	{
		ui->widget_13->show();
	}
	else if (text == "Total Pressure")
	{
		ui->widget_14->show();
	}
	else if (text == "Uniform Total Pressure")
	{
		ui->widget_15->show();
	}
	else if (text == "Fan")
	{
		ui->widget_16->show();
	}
	else if (text == "Free Stream Pressure")
	{
		ui->widget_17->show();
	}
	else if (text == "Plenum Inlet")
	{
		ui->widget_18->show();
	}
	else if (text == "Zero Gradient")
	{
	}
	else if (text == "Fixed Value")
	{
		ui->widget_19->show();
	}
	else if (text == "Fixed Mean Value")
	{
		ui->widget_20->show();
	}
	else if (text == "Uniform Fixed Value")
	{
		ui->widget_21->show();
	}
	else if (text == "Uniform Fixed Gradient")
	{
		ui->widget_22->show();
	}
}

void FormBoundaryConditionsTabWidget::on_comboBox_26_itemActivated(int index)
{
	QString text = ui->comboBox_26->currentText();

	ui->widget_35->hide();
	ui->widget_36->hide();
	ui->widget_37->hide();
	ui->widget_38->hide();
	ui->widget_39->hide();
	ui->widget_40->hide();
	ui->widget_41->hide();
	ui->widget_42->hide();
	ui->widget_43->hide();
	ui->widget_44->hide();
	ui->widget_45->hide();
	ui->widget_46->hide();
	ui->widget_47->hide();
	ui->widget_48->hide();
	ui->widget_49->hide();
	ui->widget_50->hide();
	ui->widget_51->hide();
	ui->widget_52->hide();
	ui->widget_53->hide();

	if (text == "Fixed Value")
	{
		ui->widget_35->show();
	}
	else if (text == "Fixed Mean Value")
	{
		ui->widget_36->show();
	}
	else if (text == "Uniform Fixed Value")
	{
		ui->widget_37->show();
	}
	else if (text == "Uniform Fixed Gradient")
	{
		ui->widget_38->show();
	}
	else if (text == "Fixed Gradient")
	{
		ui->widget_39->show();
	}
	else if (text == "Flow Rate Inlet Velocity")
	{
		ui->widget_40->show();
	}
	else if (text == "Flow Rate Outlet Velocity")
	{
		ui->widget_41->show();
	}
	else if (text == "Surface Normal Fixed Value")
	{
		ui->widget_42->show();
	}
	else if (text == "Pressure Directed Inlet-Outlet Velocity")
	{
		ui->widget_43->show();
	}
	else if (text == "Inlet-Outlet")
	{
		ui->widget_44->show();
	}
	else if (text == "Atmospheric Inlet")
	{
		ui->widget_45->show();
	}
	else if (text == "Free Stream Velocity" || text == "Free Stream")
	{
		ui->widget_46->show();
	}
	else if (text == "Turbulent Inlet")
	{
		ui->widget_48->show();
	}
	else if (text == "Cylindrical Inlet")
	{
		ui->widget_49->show();
	}
	else if (text == "Swirl Flow Rate Inlet")
	{
		ui->widget_47->show();
	}
	else if (text == "Matched Outlet")
	{
		ui->widget_51->show();
	}
	else if (text == "Moving Wall Velocity")
	{
		ui->widget_52->show();
	}
	else if (text == "Rotating Wall Velocity")
	{
		ui->widget_53->show();
	}
}

void FormBoundaryConditionsTabWidget::on_comboBox_43_itemActivated(int index)
{
	QString text = ui->comboBox_43->currentText();

	ui->widget_57->hide();
	ui->widget_58->hide();
	ui->widget_59->hide();
	ui->widget_60->hide();
	ui->widget_61->hide();
	ui->widget_62->hide();
	ui->widget_63->hide();
	ui->widget_64->hide();
	ui->widget_65->hide();
	ui->widget_66->hide();
	ui->widget_67->hide();
	ui->widget_68->hide();
	ui->widget_69->hide();
	ui->widget_70->hide();
	ui->widget_71->hide();
	ui->widget_72->hide();
	ui->widget_73->hide();

	if (text == "Inlet-Outlet")
	{
		ui->widget_57->show();
	}
	else if (text == "Uniform Inlet-Outlet")
	{
		ui->widget_58->show();
	}
	else if (text == "Outlet-Inlet")
	{
		ui->widget_59->show();
	}
	else if (text == "Free Stream")
	{
		ui->widget_60->show();
	}
	else if (text == "Turbulent Inlet")
	{
		ui->widget_61->show();
	}
	else if (text == "Mapped Inlet")
	{
		ui->widget_62->show();
	}
	else if (text == "Fixed Value")
	{
		ui->widget_63->show();
	}
	else if (text == "Fixed Mean Value")
	{
		ui->widget_64->show();
	}
	else if (text == "Uniform Fixed Value")
	{
		ui->widget_65->show();
	}
	else if (text == "Uniform Fixed Gradient")
	{
		ui->widget_66->show();
	}
	else if (text == "Fixed Gradient")
	{
		ui->widget_67->show();
	}
	else if (text == "Total Temperature" || text == "Inlet-Outlet Total Temperature")
	{
		ui->widget_68->show();
	}
	else if (text == "Recycled Temperature")
	{
		ui->widget_69->show();
	}
	else if (text == "Wall Heat Transfer")
	{
		ui->widget_70->show();
	}
	else if (text == "Heat Flux")
	{
		ui->widget_71->show();
	}
	else if (text == "Power")
	{
		ui->widget_72->show();
	}
	else if (text == "Lumped Mass")
	{
		ui->widget_73->show();
	}
}

void FormBoundaryConditionsTabWidget::resizeEvent(QResizeEvent* event)
{
	ui->tabWidget->tabBar()->setFixedWidth(ui->tabWidget->width());
}

void FormBoundaryConditionsTabWidget::initialization(
	const QString& solver,
	const QString& turbulenceType,
	const QString& boundaryType,
	const QString& boundaryConditionType)
{
	//隐藏所有物理量面板
	ui->widget_11->hide();
	ui->widget_23->hide();
	ui->widget_55->hide();
	ui->widget_57->hide();
	ui->widget_24->hide();
	ui->widget_25->hide();
	ui->widget_26->hide();
	ui->widget_27->hide();
	ui->widget_33->hide();
	ui->widget_28->hide();
	ui->widget_31->hide();
	ui->widget_32->hide();

	//隐藏所有tab页面
	ui->tabWidget->setTabVisible(0, false);	
	ui->tabWidget->setTabVisible(1, false);
	ui->tabWidget->setTabVisible(2, false);

	//物理量参数
	if (solver == "rhoSimpleFoam")
	{
		ui->tabWidget->setTabVisible(0, true);
		ui->tabWidget->setTabVisible(1, true);
		if (turbulenceType != "Laminar")ui->tabWidget->setTabVisible(2, true);
		ui->widget_11->show();
		ui->widget_55->show();
		ui->widget_57->show();

		//p
		ui->comboBox_14->clear();
		if (boundaryType == "patch")
		{
			ui->comboBox_14->addItem("Zero Gradient");
			ui->comboBox_14->addItem("Fixed Value");
			ui->comboBox_14->addItem("Fixed Mean Value");
			ui->comboBox_14->addItem("Uniform Fixed Value");
			ui->comboBox_14->addItem("Uniform Fixed Gradient");
			ui->comboBox_14->addItem("Fixed Gradient");
			ui->comboBox_14->addItem("Fixed Flux Pressure");
			ui->comboBox_14->addItem("Total Pressure");
			ui->comboBox_14->addItem("Uniform Total Pressure");
			ui->comboBox_14->addItem("Fan");
			ui->comboBox_14->addItem("Free Stream Pressure");
		}
		else if (boundaryType == "wall")
		{
			ui->comboBox_14->addItem("Zero Gradient");
			ui->comboBox_14->addItem("Fixed Value");
			ui->comboBox_14->addItem("Fixed Mean Value");
			ui->comboBox_14->addItem("Uniform Fixed Value");
			ui->comboBox_14->addItem("Uniform Fixed Gradient");
			ui->comboBox_14->addItem("Fixed Gradient");
			ui->comboBox_14->addItem("Fixed Flux Pressure");
		}
		else if (boundaryType == "symmetry")
		{
			ui->comboBox_14->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->comboBox_14->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->comboBox_14->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->comboBox_14->addItem("Overset");
		}
		connect(ui->comboBox_14, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_14_itemActivated);
		on_comboBox_14_itemActivated(0);

		//U
		ui->comboBox_26->clear();
		if (boundaryType == "patch")
		{
			ui->comboBox_26->addItem("Zero Gradient");
			ui->comboBox_26->addItem("Fixed Value");
			ui->comboBox_26->addItem("Fixed Mean Value");
			ui->comboBox_26->addItem("Uniform Fixed Value");
			ui->comboBox_26->addItem("Uniform Fixed Gradient");
			ui->comboBox_26->addItem("Fixed Gradient");
			ui->comboBox_26->addItem("Flow Rate Inlet Velocity");
			ui->comboBox_26->addItem("Flow Rate Outlet Velocity");
			ui->comboBox_26->addItem("Surface Normal Fixed Value");
			ui->comboBox_26->addItem("Pressure Inlet-Outlet Velocity");
			ui->comboBox_26->addItem("Pressure Normal Inlet-Outlet Velocity");
			ui->comboBox_26->addItem("Pressure Directed Inlet-Outlet Velocity");
			ui->comboBox_26->addItem("Inlet-Outlet");
			ui->comboBox_26->addItem("Free Stream Velocity");
			ui->comboBox_26->addItem("Free Stream");
			ui->comboBox_26->addItem("Turbulent Inlet");
			ui->comboBox_26->addItem("No-Slip");
			ui->comboBox_26->addItem("Slip");
			ui->comboBox_26->addItem("Cylindrical Inlet");
			ui->comboBox_26->addItem("Swirl Flow Rate Inlet");
			ui->comboBox_26->addItem("Swirl Inlet");
			ui->comboBox_26->addItem("Matched Outlet");
		}
		else if (boundaryType == "wall")
		{
			ui->comboBox_26->addItem("Zero Gradient");
			ui->comboBox_26->addItem("Fixed Value");
			ui->comboBox_26->addItem("No-Slip");
			ui->comboBox_26->addItem("Slip");
			ui->comboBox_26->addItem("Uniform Fixed Value");
			ui->comboBox_26->addItem("Moving Wall Velocity");
			ui->comboBox_26->addItem("Rotating Wall Velocity");
		}
		else if (boundaryType == "symmetry")
		{
			ui->comboBox_26->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->comboBox_26->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->comboBox_26->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->comboBox_26->addItem("Overset");
		}
		connect(ui->comboBox_26, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_26_itemActivated);
		on_comboBox_26_itemActivated(0);

		//T
		ui->comboBox_43->clear();
		if (boundaryType == "patch")
		{
			ui->comboBox_43->addItem("Inlet-Outlet");
			ui->comboBox_43->addItem("Uniform Inlet-Outlet");
			ui->comboBox_43->addItem("Outlet-Inlet");
			ui->comboBox_43->addItem("Free Stream");
			ui->comboBox_43->addItem("Turbulent Inlet");
			ui->comboBox_43->addItem("Mapped Inlet");
			ui->comboBox_43->addItem("Zero Gradient");
			ui->comboBox_43->addItem("Fixed Value");
			ui->comboBox_43->addItem("Fixed Mean Value");
			ui->comboBox_43->addItem("Uniform Fixed Value");
			ui->comboBox_43->addItem("Uniform Fixed Gradient");
			ui->comboBox_43->addItem("Fixed Gradient");
			ui->comboBox_43->addItem("Total Temperature");
			ui->comboBox_43->addItem("Inlet-Outlet Total Temperature");
			ui->comboBox_43->addItem("Recycled Temperature");
		}
		else if (boundaryType == "wall")
		{
			ui->comboBox_43->addItem("Zero Gradient");
			ui->comboBox_43->addItem("Fixed Value");
			ui->comboBox_43->addItem("Fixed Mean Value");
			ui->comboBox_43->addItem("Uniform Fixed Value");
			ui->comboBox_43->addItem("Uniform Fixed Gradient");
			ui->comboBox_43->addItem("Fixed Gradient");
			ui->comboBox_43->addItem("Wall Heat Transfer");
			ui->comboBox_43->addItem("Heat Flux");
			ui->comboBox_43->addItem("Power");
			ui->comboBox_43->addItem("Lumped Mass");
		}
		else if (boundaryType == "symmetry")
		{
			ui->comboBox_43->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->comboBox_43->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->comboBox_43->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->comboBox_43->addItem("Overset");
		}
		connect(ui->comboBox_43, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_43_itemActivated);
		on_comboBox_43_itemActivated(0);

		//湍流参数
		if (turbulenceType == "Laminar")
		{

		}
		else if (turbulenceType == "k-ε" || turbulenceType == "RNG k-ε" || turbulenceType == "Realizable k-ε")
		{

		}
		else if (turbulenceType == "Spalart-Allmaras" || turbulenceType == "Spalart-Allmaras DES" || turbulenceType == "Spalart-Allmaras DDES" || turbulenceType == "Spalart-Allmaras IDDES")
		{

		}
		else if (turbulenceType == "k-ω" || turbulenceType == "k-ω SST" || turbulenceType == "k-ω SST SAS" || turbulenceType == "k-ω SST DDES" || turbulenceType == "k-ω SST IDDES" || turbulenceType == "k-ω SST DES" || turbulenceType == "Spalart-Allmaras DES" || turbulenceType == "Spalart-Allmaras DDES" || turbulenceType == "Spalart-Allmaras IDDES")
		{

		}
		else if (turbulenceType == "Smagorinsky" || turbulenceType == "WALE")
		{

		}
		else if (turbulenceType == "k Equation" || turbulenceType == "Dynamic k Equation")
		{

		}
	}
	else if (solver == "buoyantBoussinesqPimpleFoam")
	{
		ui->tabWidget->setTabVisible(0, true);
		ui->tabWidget->setTabVisible(1, true);
		ui->tabWidget->setTabVisible(2, true);
		ui->widget_23->show();
		ui->widget_55->show();
		ui->widget_57->show();

		//p-ρgh
		ui->comboBox_16->clear();
		if (boundaryType == "patch")
		{
			ui->comboBox_16->addItem("Fixed Flux Pressure");
			ui->comboBox_16->addItem("Total Pressure");
			ui->comboBox_16->addItem("Uniform Total Pressure");
			ui->comboBox_16->addItem("Fan");
			ui->comboBox_16->addItem("Free Stream Pressure");
			ui->comboBox_16->addItem("Plenum Inlet");
			ui->comboBox_16->addItem("Zero Gradient");
			ui->comboBox_16->addItem("Fixed Value");
			ui->comboBox_16->addItem("Fixed Mean Value");
			ui->comboBox_16->addItem("Uniform Fixed Value");
			ui->comboBox_16->addItem("Uniform Fixed Gradient");
			ui->comboBox_16->addItem("Fixed Gradient");
		}
		else if (boundaryType == "wall")
		{
			ui->comboBox_16->addItem("Fixed Flux Pressure");
			ui->comboBox_16->addItem("Zero Gradient");
			ui->comboBox_16->addItem("Fixed Value");
			ui->comboBox_16->addItem("Fixed Mean Value");
			ui->comboBox_16->addItem("Uniform Fixed Value");
			ui->comboBox_16->addItem("Uniform Fixed Gradient");
			ui->comboBox_16->addItem("Fixed Gradient");
		}
		else if (boundaryType == "symmetry")
		{
			ui->comboBox_16->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->comboBox_16->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->comboBox_16->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->comboBox_16->addItem("Overset");
		}
		connect(ui->comboBox_16, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_16_itemActivated);
		on_comboBox_16_itemActivated(0);

		//U
		ui->comboBox_26->clear();
		if (boundaryType == "patch")
		{
			ui->comboBox_26->addItem("Zero Gradient");
			ui->comboBox_26->addItem("Fixed Value");
			ui->comboBox_26->addItem("Fixed Mean Value");
			ui->comboBox_26->addItem("Uniform Fixed Value");
			ui->comboBox_26->addItem("Uniform Fixed Gradient");
			ui->comboBox_26->addItem("Fixed Gradient");
			ui->comboBox_26->addItem("Flow Rate Inlet Velocity");
			ui->comboBox_26->addItem("Flow Rate Outlet Velocity");
			ui->comboBox_26->addItem("Surface Normal Fixed Value");
			ui->comboBox_26->addItem("Pressure Inlet-Outlet Velocity");
			ui->comboBox_26->addItem("Pressure Normal Inlet-Outlet Velocity");
			ui->comboBox_26->addItem("Pressure Directed Inlet-Outlet Velocity");
			ui->comboBox_26->addItem("Inlet-Outlet");
			ui->comboBox_26->addItem("Atmospheric Inlet");
			ui->comboBox_26->addItem("Free Stream Velocity");
			ui->comboBox_26->addItem("Free Stream");
			ui->comboBox_26->addItem("Turbulent Inlet");
			ui->comboBox_26->addItem("No-Slip");
			ui->comboBox_26->addItem("Slip");
			ui->comboBox_26->addItem("Cylindrical Inlet");
			ui->comboBox_26->addItem("Swirl Flow Rate Inlet");
			ui->comboBox_26->addItem("Swirl Inlet");
			ui->comboBox_26->addItem("Matched Outlet");
		}
		else if (boundaryType == "wall")
		{
			ui->comboBox_26->addItem("Fixed Value");
			ui->comboBox_26->addItem("Zero Gradient");
			ui->comboBox_26->addItem("No-Slip");
			ui->comboBox_26->addItem("Slip");
			ui->comboBox_26->addItem("Uniform Fixed Value");
			ui->comboBox_26->addItem("Moving Wall Velocity");
			ui->comboBox_26->addItem("Rotating Wall Velocity");
		}
		else if (boundaryType == "symmetry")
		{
			ui->comboBox_26->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->comboBox_26->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->comboBox_26->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->comboBox_26->addItem("Overset");
		}
		connect(ui->comboBox_26, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_26_itemActivated);
		on_comboBox_26_itemActivated(0);

		//T
		ui->comboBox_43->clear();
		if (boundaryType == "patch")
		{
			ui->comboBox_43->addItem("Inlet-Outlet");
			ui->comboBox_43->addItem("Uniform Inlet-Outlet");
			ui->comboBox_43->addItem("Outlet-Inlet");
			ui->comboBox_43->addItem("Free Stream");
			ui->comboBox_43->addItem("Turbulent Inlet");
			ui->comboBox_43->addItem("Mapped Inlet");
			ui->comboBox_43->addItem("Zero Gradient");
			ui->comboBox_43->addItem("Fixed Value");
			ui->comboBox_43->addItem("Fixed Mean Value");
			ui->comboBox_43->addItem("Uniform Fixed Value");
			ui->comboBox_43->addItem("Uniform Fixed Gradient");
			ui->comboBox_43->addItem("Fixed Gradient");
		}
		else if (boundaryType == "wall")
		{
			ui->comboBox_43->addItem("Zero Gradient");
			ui->comboBox_43->addItem("Fixed Value");
			ui->comboBox_43->addItem("Fixed Mean Value");
			ui->comboBox_43->addItem("Uniform Fixed Value");
			ui->comboBox_43->addItem("Uniform Fixed Gradient");
			ui->comboBox_43->addItem("Fixed Gradient");
		}
		else if (boundaryType == "symmetry")
		{
			ui->comboBox_43->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->comboBox_43->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->comboBox_43->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->comboBox_43->addItem("Overset");
		}
		connect(ui->comboBox_43, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_43_itemActivated);
		on_comboBox_43_itemActivated(0);

		//湍流参数
		if (turbulenceType == "Laminar")
		{

		}
		else if (turbulenceType == "k-ε" || turbulenceType == "RNG k-ε" || turbulenceType == "Realizable k-ε")
		{

		}
		else if (turbulenceType == "Spalart-Allmaras" || turbulenceType == "Spalart-Allmaras DES" || turbulenceType == "Spalart-Allmaras DDES" || turbulenceType == "Spalart-Allmaras IDDES")
		{

		}
		else if (turbulenceType == "k-ω" || turbulenceType == "k-ω SST" || turbulenceType == "k-ω SST SAS" || turbulenceType == "k-ω SST DDES" || turbulenceType == "k-ω SST IDDES" || turbulenceType == "k-ω SST DES" || turbulenceType == "Spalart-Allmaras DES" || turbulenceType == "Spalart-Allmaras DDES" || turbulenceType == "Spalart-Allmaras IDDES")
		{

		}
		else if (turbulenceType == "Smagorinsky" || turbulenceType == "WALE")
		{

		}
		else if (turbulenceType == "k Equation" || turbulenceType == "Dynamic k Equation")
		{

		}
	}
	else if (solver == "chtMultiRegionFoam")
	{
		ui->tabWidget->setTabVisible(0, true);
		ui->tabWidget->setTabVisible(1, true);
		if (turbulenceType != "Laminar")ui->tabWidget->setTabVisible(2, true);
		ui->widget_23->show();
		ui->widget_55->show();
		ui->widget_57->show();

		//p-ρgh
		ui->comboBox_16->clear();
		if (boundaryType == "patch")
		{
			ui->comboBox_16->addItem("Fixed Flux Pressure");
			ui->comboBox_16->addItem("Total Pressure");
			ui->comboBox_16->addItem("Uniform Total Pressure");
			ui->comboBox_16->addItem("Fan");
			ui->comboBox_16->addItem("Free Stream Pressure");
			ui->comboBox_16->addItem("Plenum Inlet");
			ui->comboBox_16->addItem("Zero Gradient");
			ui->comboBox_16->addItem("Fixed Value");
			ui->comboBox_16->addItem("Fixed Mean Value");
			ui->comboBox_16->addItem("Uniform Fixed Value");
			ui->comboBox_16->addItem("Uniform Fixed Gradient");
			ui->comboBox_16->addItem("Fixed Gradient");
		}
		else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
		{
			ui->comboBox_16->addItem("Fixed Flux Pressure");
			ui->comboBox_16->addItem("Zero Gradient");
			ui->comboBox_16->addItem("Fixed Value");
			ui->comboBox_16->addItem("Fixed Mean Value");
			ui->comboBox_16->addItem("Uniform Fixed Value");
			ui->comboBox_16->addItem("Uniform Fixed Gradient");
			ui->comboBox_16->addItem("Fixed Gradient");
		}
		else if (boundaryType == "symmetry")
		{
			ui->comboBox_16->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->comboBox_16->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->comboBox_16->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->comboBox_16->addItem("Overset");
		}
		connect(ui->comboBox_16, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_16_itemActivated);
		on_comboBox_16_itemActivated(0);

		//U
		ui->comboBox_26->clear();
		if (boundaryType == "patch")
		{
			ui->comboBox_26->addItem("Zero Gradient");
			ui->comboBox_26->addItem("Fixed Value");
			ui->comboBox_26->addItem("Fixed Mean Value");
			ui->comboBox_26->addItem("Uniform Fixed Value");
			ui->comboBox_26->addItem("Uniform Fixed Gradient");
			ui->comboBox_26->addItem("Fixed Gradient");
			ui->comboBox_26->addItem("Flow Rate Inlet Velocity");
			ui->comboBox_26->addItem("Flow Rate Outlet Velocity");
			ui->comboBox_26->addItem("Surface Normal Fixed Value");
			ui->comboBox_26->addItem("Pressure Inlet-Outlet Velocity");
			ui->comboBox_26->addItem("Pressure Normal Inlet-Outlet Velocity");
			ui->comboBox_26->addItem("Pressure Directed Inlet-Outlet Velocity");
			ui->comboBox_26->addItem("Inlet-Outlet");
			ui->comboBox_26->addItem("Free Stream Velocity");
			ui->comboBox_26->addItem("Free Stream");
			ui->comboBox_26->addItem("Turbulent Inlet");
			ui->comboBox_26->addItem("No-Slip");
			ui->comboBox_26->addItem("Slip");
			ui->comboBox_26->addItem("Cylindrical Inlet");
			ui->comboBox_26->addItem("Swirl Flow Rate Inlet");
			ui->comboBox_26->addItem("Swirl Inlet");
			ui->comboBox_26->addItem("Matched Outlet");
		}
		else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
		{
			ui->comboBox_26->addItem("Zero Gradient");
			ui->comboBox_26->addItem("Fixed Value");
			ui->comboBox_26->addItem("No-Slip");
			ui->comboBox_26->addItem("Slip");
			ui->comboBox_26->addItem("Uniform Fixed Value");
			ui->comboBox_26->addItem("Moving Wall Velocity");
			ui->comboBox_26->addItem("Rotating Wall Velocity");
		}
		else if (boundaryType == "symmetry")
		{
			ui->comboBox_26->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->comboBox_26->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->comboBox_26->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->comboBox_26->addItem("Overset");
		}
		connect(ui->comboBox_26, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_26_itemActivated);
		on_comboBox_26_itemActivated(0);

		//T
		ui->comboBox_43->clear();
		if (boundaryType == "patch")
		{
			ui->comboBox_43->addItem("Inlet-Outlet");
			ui->comboBox_43->addItem("Uniform Inlet-Outlet");
			ui->comboBox_43->addItem("Outlet-Inlet");
			ui->comboBox_43->addItem("Free Stream");
			ui->comboBox_43->addItem("Turbulent Inlet");
			ui->comboBox_43->addItem("Mapped Inlet");
			ui->comboBox_43->addItem("Zero Gradient");
			ui->comboBox_43->addItem("Fixed Value");
			ui->comboBox_43->addItem("Fixed Mean Value");
			ui->comboBox_43->addItem("Uniform Fixed Value");
			ui->comboBox_43->addItem("Uniform Fixed Gradient");
			ui->comboBox_43->addItem("Fixed Gradient");
			ui->comboBox_43->addItem("Total Temperature");
			ui->comboBox_43->addItem("Inlet-Outlet Total Temperature");
			ui->comboBox_43->addItem("Recycled Temperature");
		}
		else if (boundaryType == "wall")
		{
			ui->comboBox_43->addItem("Zero Gradient");
			ui->comboBox_43->addItem("Fixed Value");
			ui->comboBox_43->addItem("Fixed Mean Value");
			ui->comboBox_43->addItem("Uniform Fixed Value");
			ui->comboBox_43->addItem("Uniform Fixed Gradient");
			ui->comboBox_43->addItem("Fixed Gradient");
			ui->comboBox_43->addItem("Wall Heat Transfer");
			ui->comboBox_43->addItem("Heat Flux");
			ui->comboBox_43->addItem("Power");
			ui->comboBox_43->addItem("Lumped Mass");
		}
		else if (boundaryType.startsWith("mappedWall"))
		{
			ui->comboBox_43->addItem("Coupled Temperature and Radiation");
			ui->comboBox_43->addItem("Coupled Temperature");
		}
		else if (boundaryType == "symmetry")
		{
			ui->comboBox_43->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->comboBox_43->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->comboBox_43->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->comboBox_43->addItem("Overset");
		}
		connect(ui->comboBox_43, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_43_itemActivated);
		on_comboBox_43_itemActivated(0);

		//湍流参数
		if (turbulenceType == "Laminar")
		{

		}
		else if (turbulenceType == "k-ε" || turbulenceType == "RNG k-ε" || turbulenceType == "Realizable k-ε")
		{

		}
		else if (turbulenceType == "Spalart-Allmaras" || turbulenceType == "Spalart-Allmaras DES" || turbulenceType == "Spalart-Allmaras DDES" || turbulenceType == "Spalart-Allmaras IDDES")
		{

		}
		else if (turbulenceType == "k-ω" || turbulenceType == "k-ω SST" || turbulenceType == "k-ω SST SAS" || turbulenceType == "k-ω SST DDES" || turbulenceType == "k-ω SST IDDES" || turbulenceType == "k-ω SST DES" || turbulenceType == "Spalart-Allmaras DES" || turbulenceType == "Spalart-Allmaras DDES" || turbulenceType == "Spalart-Allmaras IDDES")
		{

		}
		else if (turbulenceType == "Smagorinsky" || turbulenceType == "WALE")
		{

		}
		else if (turbulenceType == "k Equation" || turbulenceType == "Dynamic k Equation")
		{

		}
	}
}