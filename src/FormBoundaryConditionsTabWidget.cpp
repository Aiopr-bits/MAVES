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

	//绑定comboBox切换事件
	QList<QComboBox*> comboList = {
		ui->comboBox_3,ui->comboBox_4,ui->comboBox_5,ui->comboBox_6,ui->comboBox_7,ui->comboBox_8,ui->comboBox_9,ui->comboBox_11,
		ui->comboBox_12,ui->comboBox_13,ui->comboBox_27,ui->comboBox_28,ui->comboBox_29,ui->comboBox_30,ui->comboBox_31,ui->comboBox_32,
		ui->comboBox_33,ui->comboBox_34,ui->comboBox_35,ui->comboBox_36,ui->comboBox_37,ui->comboBox_38,ui->comboBox_39,ui->comboBox_42,
		ui->comboBox_44,ui->comboBox_46,ui->comboBox_47,ui->comboBox_48,ui->comboBox_51,ui->comboBox_52,ui->comboBox_53,ui->comboBox_54,
		ui->comboBox_57,ui->comboBox_58,ui->comboBox_59,ui->comboBox_60,ui->comboBox_63,ui->comboBox_64,ui->comboBox_65,ui->comboBox_66,
		ui->comboBox_69,ui->comboBox_70,ui->comboBox_71,ui->comboBox_72,ui->comboBox_76,ui->comboBox_78,ui->comboBox_79,
	};
	for (auto* combo : comboList)
	{
		connect(combo, qOverload<int>(&QComboBox::activated),this, &FormBoundaryConditionsTabWidget::on_anyComboBox_itemActivated);
		combo->setCurrentIndex(0);
	}

	toggleWidgets(ui->lineEdit_17, ui->pushButton, true);
	toggleWidgets(ui->lineEdit_18, ui->pushButton_2, true);
	toggleWidgets(ui->lineEdit_19, ui->pushButton_3, true);
	toggleWidgets(ui->lineEdit_22, ui->pushButton_4, true);
	toggleWidgets(ui->lineEdit_24, ui->pushButton_5, true);
	toggleWidgets(ui->lineEdit_27, ui->pushButton_6, true);
	toggleWidgets(ui->lineEdit_29, ui->pushButton_7, true);
	toggleWidgets(ui->lineEdit_45, ui->pushButton_9, true);
	toggleWidgets(ui->lineEdit_46, ui->pushButton_10, true);
	toggleWidgets(ui->lineEdit_47, ui->pushButton_11, true);
	toggleWidgets(ui->widget_34, ui->pushButton_17, true);
	toggleWidgets(ui->widget_144, ui->pushButton_18, true);
	toggleWidgets(ui->widget_145, ui->pushButton_19, true);
	toggleWidgets(ui->lineEdit_88, ui->pushButton_20, true);
	toggleWidgets(ui->lineEdit_91, ui->pushButton_21, true);
	toggleWidgets(ui->lineEdit_136, ui->pushButton_22, true);
	toggleWidgets(ui->lineEdit_137, ui->pushButton_23, true);
	toggleWidgets(ui->lineEdit_138, ui->pushButton_24, true);
	toggleWidgets(ui->lineEdit_139, ui->pushButton_25, true);
	toggleWidgets(ui->lineEdit_140, ui->pushButton_26, true);
	toggleWidgets(ui->lineEdit_147, ui->pushButton_27, true);
	toggleWidgets(ui->lineEdit_148, ui->pushButton_28, true);
	toggleWidgets(ui->lineEdit_149, ui->pushButton_29, true);
	toggleWidgets(ui->lineEdit_156, ui->pushButton_30, true);
	toggleWidgets(ui->lineEdit_158, ui->pushButton_31, true);
	toggleWidgets(ui->lineEdit_165, ui->pushButton_32, true);
	toggleWidgets(ui->lineEdit_166, ui->pushButton_33, true);
	toggleWidgets(ui->lineEdit_167, ui->pushButton_34, true);
	toggleWidgets(ui->lineEdit_181, ui->pushButton_35, true);
	toggleWidgets(ui->lineEdit_182, ui->pushButton_36, true);
	toggleWidgets(ui->lineEdit_183, ui->pushButton_37, true);
	toggleWidgets(ui->lineEdit_186, ui->pushButton_38, true);
	toggleWidgets(ui->lineEdit_194, ui->pushButton_39, true);
	toggleWidgets(ui->lineEdit_195, ui->pushButton_40, true);
	toggleWidgets(ui->lineEdit_196, ui->pushButton_41, true);
	toggleWidgets(ui->lineEdit_199, ui->pushButton_42, true);
	toggleWidgets(ui->lineEdit_207, ui->pushButton_43, true);
	toggleWidgets(ui->lineEdit_208, ui->pushButton_44, true);
	toggleWidgets(ui->lineEdit_209, ui->pushButton_45, true);
	toggleWidgets(ui->lineEdit_212, ui->pushButton_46, true);
	toggleWidgets(ui->lineEdit_220, ui->pushButton_47, true);
	toggleWidgets(ui->lineEdit_221, ui->pushButton_48, true);
	toggleWidgets(ui->lineEdit_222, ui->pushButton_49, true);
	toggleWidgets(ui->lineEdit_225, ui->pushButton_50, true);
	toggleWidgets(ui->lineEdit_234, ui->pushButton_51, true);
	toggleWidgets(ui->lineEdit_236, ui->pushButton_53, true);
	toggleWidgets(ui->lineEdit_239, ui->pushButton_54, true);
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

void FormBoundaryConditionsTabWidget::on_comboBox_50_itemActivated(int index)
{
	QString text = ui->comboBox_50->currentText();

	ui->widget_76->hide();
	ui->widget_77->hide();
	ui->widget_78->hide();
	ui->widget_79->hide();
	ui->widget_80->hide();
	ui->widget_81->hide();
	ui->widget_82->hide();
	ui->widget_83->hide();
	ui->widget_84->hide();
	ui->widget_85->hide();
	ui->widget_86->hide();
	ui->widget_87->hide();

	if (text == "Turbulent Intensity Inlet")
	{
		ui->widget_76->show();
	}
	else if (text == "Fixed Value")
	{
		ui->widget_77->show();
	}
	else if (text == "Fixed Mean Value")
	{
		ui->widget_78->show();
	}
	else if (text == "Uniform Fixed Value")
	{
		ui->widget_79->show();
	}
	else if (text == "Uniform Fixed Gradient")
	{
		ui->widget_80->show();
	}
	else if (text == "Fixed Gradient")
	{
		ui->widget_81->show();
	}
	else if (text == "Inlet-Outlet")
	{
		ui->widget_82->show();
	}
	else if (text == "Uniform Inlet-Outlet")
	{
		ui->widget_83->show();
	}
	else if (text == "Outlet-Inlet")
	{
		ui->widget_84->show();
	}
	else if (text == "Free Stream")
	{
		ui->widget_85->show();
	}
	else if (text == "Turbulent Inlet")
	{
		ui->widget_86->show();
	}
	else if (text == "Mapped Inlet")
	{
		ui->widget_87->show();
	}
}

void FormBoundaryConditionsTabWidget::on_comboBox_56_itemActivated(int index)
{
	QString text = ui->comboBox_56->currentText();

	ui->widget_89->hide();
	ui->widget_90->hide();
	ui->widget_91->hide();
	ui->widget_92->hide();
	ui->widget_93->hide();
	ui->widget_94->hide();
	ui->widget_95->hide();
	ui->widget_96->hide();
	ui->widget_97->hide();
	ui->widget_98->hide();
	ui->widget_99->hide();
	ui->widget_100->hide();

	if (text == "Turbulent Intensity Inlet")
	{
		ui->widget_89->show();
	}
	else if (text == "Fixed Value")
	{
		ui->widget_90->show();
	}
	else if (text == "Fixed Mean Value")
	{
		ui->widget_91->show();
	}
	else if (text == "Uniform Fixed Value")
	{
		ui->widget_92->show();
	}
	else if (text == "Uniform Fixed Gradient")
	{
		ui->widget_93->show();
	}
	else if (text == "Fixed Gradient")
	{
		ui->widget_94->show();
	}
	else if (text == "Inlet-Outlet")
	{
		ui->widget_95->show();
	}
	else if (text == "Uniform Inlet-Outlet")
	{
		ui->widget_96->show();
	}
	else if (text == "Outlet-Inlet")
	{
		ui->widget_97->show();
	}
	else if (text == "Free Stream")
	{
		ui->widget_98->show();
	}
	else if (text == "Turbulent Inlet")
	{
		ui->widget_99->show();
	}
	else if (text == "Mapped Inlet")
	{
		ui->widget_100->show();
	}
}

void FormBoundaryConditionsTabWidget::on_comboBox_62_itemActivated(int index)
{
	QString text = ui->comboBox_62->currentText();

	ui->widget_102->hide();
	ui->widget_103->hide();
	ui->widget_104->hide();
	ui->widget_105->hide();
	ui->widget_106->hide();
	ui->widget_107->hide();
	ui->widget_108->hide();
	ui->widget_109->hide();
	ui->widget_110->hide();
	ui->widget_111->hide();
	ui->widget_112->hide();
	ui->widget_113->hide();

	if (text == "Turbulent Intensity Inlet")
	{
		ui->widget_102->show();
	}
	else if (text == "Fixed Value")
	{
		ui->widget_103->show();
	}
	else if (text == "Fixed Mean Value")
	{
		ui->widget_104->show();
	}
	else if (text == "Uniform Fixed Value")
	{
		ui->widget_105->show();
	}
	else if (text == "Uniform Fixed Gradient")
	{
		ui->widget_106->show();
	}
	else if (text == "Fixed Gradient")
	{
		ui->widget_107->show();
	}
	else if (text == "Inlet-Outlet")
	{
		ui->widget_108->show();
	}
	else if (text == "Uniform Inlet-Outlet")
	{
		ui->widget_109->show();
	}
	else if (text == "Outlet-Inlet")
	{
		ui->widget_110->show();
	}
	else if (text == "Free Stream")
	{
		ui->widget_111->show();
	}
	else if (text == "Turbulent Inlet")
	{
		ui->widget_112->show();
	}
	else if (text == "Mapped Inlet")
	{
		ui->widget_113->show();
	}
}

void FormBoundaryConditionsTabWidget::on_comboBox_68_itemActivated(int index)
{
	QString text = ui->comboBox_68->currentText();

	ui->widget_115->hide();
	ui->widget_116->hide();
	ui->widget_117->hide();
	ui->widget_118->hide();
	ui->widget_119->hide();
	ui->widget_120->hide();
	ui->widget_121->hide();
	ui->widget_122->hide();
	ui->widget_123->hide();
	ui->widget_124->hide();
	ui->widget_125->hide();
	ui->widget_126->hide();

	if (text == "Turbulent Mixing Length Inlet")
	{
		ui->widget_115->show();
	}
	else if (text == "Fixed Value")
	{
		ui->widget_116->show();
	}
	else if (text == "Fixed Mean Value")
	{
		ui->widget_117->show();
	}
	else if (text == "Uniform Fixed Value")
	{
		ui->widget_118->show();
	}
	else if (text == "Uniform Fixed Gradient")
	{
		ui->widget_119->show();
	}
	else if (text == "Fixed Gradient")
	{
		ui->widget_120->show();
	}
	else if (text == "Inlet-Outlet")
	{
		ui->widget_121->show();
	}
	else if (text == "Uniform Inlet-Outlet")
	{
		ui->widget_122->show();
	}
	else if (text == "Outlet-Inlet")
	{
		ui->widget_123->show();
	}
	else if (text == "Free Stream")
	{
		ui->widget_124->show();
	}
	else if (text == "Turbulent Inlet")
	{
		ui->widget_125->show();
	}
	else if (text == "Mapped Inlet")
	{
		ui->widget_126->show();
	}
}

void FormBoundaryConditionsTabWidget::on_comboBox_74_itemActivated(int index)
{
	QString text = ui->comboBox_74->currentText();

	ui->widget_128->hide();

	if (text == "Fixed Value")
	{
		ui->widget_128->show();
	}
}

void FormBoundaryConditionsTabWidget::on_comboBox_75_itemActivated(int index)
{
	QString text = ui->comboBox_75->currentText();

	ui->widget_130->hide();
	ui->widget_131->hide();
	ui->widget_132->hide();
	ui->widget_133->hide();
	ui->widget_134->hide();
	ui->widget_135->hide();
	ui->widget_136->hide();
	ui->widget_137->hide();
	ui->widget_138->hide();
	ui->widget_139->hide();
	ui->widget_140->hide();
	ui->widget_141->hide();

	if (text == "Turbulent Mixing Length Inlet")
	{
		ui->widget_130->show();
	}
	else if (text == "Fixed Value")
	{
		ui->widget_131->show();
	}
	else if (text == "Fixed Mean Value")
	{
		ui->widget_132->show();
	}
	else if (text == "Uniform Fixed Value")
	{
		ui->widget_133->show();
	}
	else if (text == "Uniform Fixed Gradient")
	{
		ui->widget_134->show();
	}
	else if (text == "Fixed Gradient")
	{
		ui->widget_135->show();
	}
	else if (text == "Inlet-Outlet")
	{
		ui->widget_136->show();
	}
	else if (text == "Uniform Inlet-Outlet")
	{
		ui->widget_137->show();
	}
	else if (text == "Outlet-Inlet")
	{
		ui->widget_138->show();
	}
	else if (text == "Free Stream")
	{
		ui->widget_139->show();
	}
	else if (text == "Turbulent Inlet")
	{
		ui->widget_140->show();
	}
	else if (text == "Mapped Inlet")
	{
		ui->widget_141->show();
	}
}

void FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated(int index)
{
	QString text = ui->comboBox_81->currentText();

	ui->widget_29->hide();
	ui->widget_30->hide();
	ui->widget_141->hide();

	if (text == "Rough Wall Function")
	{
		ui->widget_29->show();
	}
	else if (text == "Rough Wall Function(k)")
	{
		ui->widget_30->show();
	}
	else if (text == "Atmospheric Wall Function")
	{
		ui->widget_141->show();
	}
}

void FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated(int index)
{
	QString text = ui->comboBox_82->currentText();

}

void FormBoundaryConditionsTabWidget::resizeEvent(QResizeEvent* event)
{
	ui->tabWidget->tabBar()->setFixedWidth(ui->tabWidget->width());
}

void FormBoundaryConditionsTabWidget::toggleWidgets(QWidget* w1, QWidget* w2, bool showFirst)
{
	w1->setVisible(showFirst);
	w2->setVisible(!showFirst);
}

void FormBoundaryConditionsTabWidget::on_anyComboBox_itemActivated(int index)
{
	QObject* obj = sender();
	bool showFirst = (index == 0);

	if (obj == ui->comboBox_3)
		toggleWidgets(ui->lineEdit_17, ui->pushButton, showFirst);
	else if (obj == ui->comboBox_4)
		toggleWidgets(ui->lineEdit_18, ui->pushButton_2, showFirst);
	else if (obj == ui->comboBox_5)
		toggleWidgets(ui->lineEdit_19, ui->pushButton_3, showFirst);
	else if (obj == ui->comboBox_6)
		toggleWidgets(ui->lineEdit_22, ui->pushButton_4, showFirst);
	else if (obj == ui->comboBox_7)
		toggleWidgets(ui->lineEdit_24, ui->pushButton_5, showFirst);
	else if (obj == ui->comboBox_8)
		toggleWidgets(ui->lineEdit_27, ui->pushButton_6, showFirst);
	else if (obj == ui->comboBox_9)
		toggleWidgets(ui->lineEdit_29, ui->pushButton_7, showFirst);
	else if (obj == ui->comboBox_11)
		toggleWidgets(ui->lineEdit_45, ui->pushButton_9, showFirst);
	else if (obj == ui->comboBox_12)
		toggleWidgets(ui->lineEdit_46, ui->pushButton_10, showFirst);
	else if (obj == ui->comboBox_13)
		toggleWidgets(ui->lineEdit_47, ui->pushButton_11, showFirst);
	else if (obj == ui->comboBox_27)
		toggleWidgets(ui->widget_34, ui->pushButton_17, showFirst);
	else if (obj == ui->comboBox_28)
		toggleWidgets(ui->widget_144, ui->pushButton_18, showFirst);
	else if (obj == ui->comboBox_29)
		toggleWidgets(ui->widget_145, ui->pushButton_19, showFirst);
	else if (obj == ui->comboBox_30)
		toggleWidgets(ui->lineEdit_88, ui->pushButton_20, showFirst);
	else if (obj == ui->comboBox_31)
		toggleWidgets(ui->lineEdit_91, ui->pushButton_21, showFirst);
	else if (obj == ui->comboBox_32)
		toggleWidgets(ui->lineEdit_136, ui->pushButton_22, showFirst);
	else if (obj == ui->comboBox_33)
		toggleWidgets(ui->lineEdit_137, ui->pushButton_23, showFirst);
	else if (obj == ui->comboBox_34)
		toggleWidgets(ui->lineEdit_138, ui->pushButton_24, showFirst);
	else if (obj == ui->comboBox_35)
		toggleWidgets(ui->lineEdit_139, ui->pushButton_25, showFirst);
	else if (obj == ui->comboBox_36)
		toggleWidgets(ui->lineEdit_140, ui->pushButton_26, showFirst);
	else if (obj == ui->comboBox_37)
		toggleWidgets(ui->lineEdit_147, ui->pushButton_27, showFirst);
	else if (obj == ui->comboBox_38)
		toggleWidgets(ui->lineEdit_148, ui->pushButton_28, showFirst);
	else if (obj == ui->comboBox_39)
		toggleWidgets(ui->lineEdit_149, ui->pushButton_29, showFirst);
	else if (obj == ui->comboBox_42)
		toggleWidgets(ui->lineEdit_156, ui->pushButton_30, showFirst);
	else if (obj == ui->comboBox_44)
		toggleWidgets(ui->lineEdit_158, ui->pushButton_31, showFirst);
	else if (obj == ui->comboBox_46)
		toggleWidgets(ui->lineEdit_165, ui->pushButton_32, showFirst);
	else if (obj == ui->comboBox_47)
		toggleWidgets(ui->lineEdit_166, ui->pushButton_33, showFirst);
	else if (obj == ui->comboBox_48)
		toggleWidgets(ui->lineEdit_167, ui->pushButton_34, showFirst);
	else if (obj == ui->comboBox_51)
		toggleWidgets(ui->lineEdit_181, ui->pushButton_35, showFirst);
	else if (obj == ui->comboBox_52)
		toggleWidgets(ui->lineEdit_182, ui->pushButton_36, showFirst);
	else if (obj == ui->comboBox_53)
		toggleWidgets(ui->lineEdit_183, ui->pushButton_37, showFirst);
	else if (obj == ui->comboBox_54)
		toggleWidgets(ui->lineEdit_186, ui->pushButton_38, showFirst);
	else if (obj == ui->comboBox_57)
		toggleWidgets(ui->lineEdit_194, ui->pushButton_39, showFirst);
	else if (obj == ui->comboBox_58)
		toggleWidgets(ui->lineEdit_195, ui->pushButton_40, showFirst);
	else if (obj == ui->comboBox_59)
		toggleWidgets(ui->lineEdit_196, ui->pushButton_41, showFirst);
	else if (obj == ui->comboBox_60)
		toggleWidgets(ui->lineEdit_199, ui->pushButton_42, showFirst);
	else if (obj == ui->comboBox_63)
		toggleWidgets(ui->lineEdit_207, ui->pushButton_43, showFirst);
	else if (obj == ui->comboBox_64)
		toggleWidgets(ui->lineEdit_208, ui->pushButton_44, showFirst);
	else if (obj == ui->comboBox_65)
		toggleWidgets(ui->lineEdit_209, ui->pushButton_45, showFirst);
	else if (obj == ui->comboBox_66)
		toggleWidgets(ui->lineEdit_212, ui->pushButton_46, showFirst);
	else if (obj == ui->comboBox_69)
		toggleWidgets(ui->lineEdit_220, ui->pushButton_47, showFirst);
	else if (obj == ui->comboBox_70)
		toggleWidgets(ui->lineEdit_221, ui->pushButton_48, showFirst);
	else if (obj == ui->comboBox_71)
		toggleWidgets(ui->lineEdit_222, ui->pushButton_49, showFirst);
	else if (obj == ui->comboBox_72)
		toggleWidgets(ui->lineEdit_225, ui->pushButton_50, showFirst);
	else if (obj == ui->comboBox_76)
		toggleWidgets(ui->lineEdit_234, ui->pushButton_51, showFirst);
	else if (obj == ui->comboBox_78)
		toggleWidgets(ui->lineEdit_236, ui->pushButton_53, showFirst);
	else if (obj == ui->comboBox_79)
		toggleWidgets(ui->lineEdit_239, ui->pushButton_54, showFirst);
}

void FormBoundaryConditionsTabWidget::initialization(
	const QString& solver,
	const QString& turbulenceType,
	const QString& boundaryType,
	const QString& boundaryConditionType)
{
	//隐藏所有物理量参数面板
	ui->widget_11->hide();
	ui->widget_23->hide();
	ui->widget_55->hide();
	ui->widget_57->hide();

	//隐藏所有的湍流参数面板
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

		//p
		ui->comboBox_14->clear();
		if (boundaryType == "patch")
		{
			ui->widget_11->show();
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
			ui->widget_11->show();
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
			ui->widget_11->show();
			ui->comboBox_14->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->widget_11->show();
			ui->comboBox_14->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->widget_11->show();
			ui->comboBox_14->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->widget_11->show();
			ui->comboBox_14->addItem("Overset");
		}
		connect(ui->comboBox_14, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_14_itemActivated);
		on_comboBox_14_itemActivated(0);

		//U
		ui->comboBox_26->clear();
		if (boundaryType == "patch")
		{
			ui->widget_55->show();
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
			ui->widget_55->show();
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
			ui->widget_55->show();
			ui->comboBox_26->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->widget_55->show();
			ui->comboBox_26->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->widget_55->show();
			ui->comboBox_26->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->widget_55->show();
			ui->comboBox_26->addItem("Overset");
		}
		connect(ui->comboBox_26, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_26_itemActivated);
		on_comboBox_26_itemActivated(0);

		//T
		ui->comboBox_43->clear();
		if (boundaryType == "patch")
		{
			ui->widget_57->show();
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
			ui->widget_57->show();
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
			ui->widget_57->show();
			ui->comboBox_43->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->widget_57->show();
			ui->comboBox_43->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->widget_57->show();
			ui->comboBox_43->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->widget_57->show();
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
			ui->tabWidget->setTabVisible(2, true);

			//k
			ui->comboBox_50->clear();
			if (boundaryType == "patch")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Turbulent Intensity Inlet");
				ui->comboBox_50->addItem("Atmospheric Inlet");
				ui->comboBox_50->addItem("Zero Gradient");
				ui->comboBox_50->addItem("Fixed Value");
				ui->comboBox_50->addItem("Fixed Mean Value");
				ui->comboBox_50->addItem("Uniform Fixed Value");
				ui->comboBox_50->addItem("Uniform Fixed Gradient");
				ui->comboBox_50->addItem("Fixed Gradient");
				ui->comboBox_50->addItem("Inlet-Outlet");
				ui->comboBox_50->addItem("Uniform Inlet-Outlet");
				ui->comboBox_50->addItem("Outlet-Inlet");
				ui->comboBox_50->addItem("Free Stream");
				ui->comboBox_50->addItem("Turbulent Inlet");
				ui->comboBox_50->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Standard Wall Function");
				ui->comboBox_50->addItem("Low Re Wall Function");
				ui->comboBox_50->addItem("Fixed Value");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Overset");
			}
			connect(ui->comboBox_50, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_50_itemActivated);
			on_comboBox_50_itemActivated(0);

			//ε
			ui->comboBox_68->clear();
			if (boundaryType == "patch")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Turbulent Mixing Length Inlet");
				ui->comboBox_68->addItem("Zero Gradient");
				ui->comboBox_68->addItem("Fixed Value");
				ui->comboBox_68->addItem("Fixed Mean Value");
				ui->comboBox_68->addItem("Uniform Fixed Value");
				ui->comboBox_68->addItem("Uniform Fixed Gradient");
				ui->comboBox_68->addItem("Fixed Gradient");
				ui->comboBox_68->addItem("Inlet-Outlet");
				ui->comboBox_68->addItem("Uniform Inlet-Outlet");
				ui->comboBox_68->addItem("Outlet-Inlet");
				ui->comboBox_68->addItem("Free Stream");
				ui->comboBox_68->addItem("Turbulent Inlet");
				ui->comboBox_68->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Standard Wall Function");
				ui->comboBox_68->addItem("Low Re Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Overset");
			}
			connect(ui->comboBox_68, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_68_itemActivated);
			on_comboBox_68_itemActivated(0);

			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
			}
			else if (boundaryType == "wall")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Standard Wall Function");
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "Spalart-Allmaras" || turbulenceType == "Spalart-Allmaras DES" || turbulenceType == "Spalart-Allmaras DDES" || turbulenceType == "Spalart-Allmaras IDDES")
		{
			ui->tabWidget->setTabVisible(2, true);

			//v_SA
			ui->comboBox_74->clear();
			if (boundaryType == "patch")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Fixed Value");
				ui->comboBox_74->addItem("Zero Gradient");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Fixed Value");
				ui->comboBox_74->addItem("Zero Gradient");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Overset");
			}
			connect(ui->comboBox_74, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_74_itemActivated);
			on_comboBox_74_itemActivated(0);

			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
			}
			else if (boundaryType == "wall")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Standard Wall Function");
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "k-ω" || turbulenceType == "k-ω SST" || turbulenceType == "k-ω SST SAS" || turbulenceType == "k-ω SST DDES" || turbulenceType == "k-ω SST IDDES" || turbulenceType == "k-ω SST DES" || turbulenceType == "Spalart-Allmaras DES" || turbulenceType == "Spalart-Allmaras DDES" || turbulenceType == "Spalart-Allmaras IDDES")
		{
			ui->tabWidget->setTabVisible(2, true);

			//k
			ui->comboBox_50->clear();
			if (boundaryType == "patch")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Turbulent Intensity Inlet");
				ui->comboBox_50->addItem("Atmospheric Inlet");
				ui->comboBox_50->addItem("Zero Gradient");
				ui->comboBox_50->addItem("Fixed Value");
				ui->comboBox_50->addItem("Fixed Mean Value");
				ui->comboBox_50->addItem("Uniform Fixed Value");
				ui->comboBox_50->addItem("Uniform Fixed Gradient");
				ui->comboBox_50->addItem("Fixed Gradient");
				ui->comboBox_50->addItem("Inlet-Outlet");
				ui->comboBox_50->addItem("Uniform Inlet-Outlet");
				ui->comboBox_50->addItem("Outlet-Inlet");
				ui->comboBox_50->addItem("Free Stream");
				ui->comboBox_50->addItem("Turbulent Inlet");
				ui->comboBox_50->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Standard Wall Function");
				ui->comboBox_50->addItem("Low Re Wall Function");
				ui->comboBox_50->addItem("Fixed Value");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Overset");
			}
			connect(ui->comboBox_50, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_50_itemActivated);
			on_comboBox_50_itemActivated(0);

			//ω
			ui->comboBox_75->clear();
			if (boundaryType == "patch")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Turbulent Mixing Length Inlet");
				ui->comboBox_75->addItem("Zero Gradient");
				ui->comboBox_75->addItem("Fixed Value");
				ui->comboBox_75->addItem("Fixed Mean Value");
				ui->comboBox_75->addItem("Uniform Fixed Value");
				ui->comboBox_75->addItem("Uniform Fixed Gradient");
				ui->comboBox_75->addItem("Fixed Gradient");
				ui->comboBox_75->addItem("Inlet-Outlet");
				ui->comboBox_75->addItem("Uniform Inlet-Outlet");
				ui->comboBox_75->addItem("Outlet-Inlet");
				ui->comboBox_75->addItem("Free Stream");
				ui->comboBox_75->addItem("Turbulent Inlet");
				ui->comboBox_75->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Standard Wall Function");
				ui->comboBox_75->addItem("Zero Gradient");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Overset");
			}
			connect(ui->comboBox_75, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_75_itemActivated);
			on_comboBox_75_itemActivated(0);

			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
			}
			else if (boundaryType == "wall")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Standard Wall Function");
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "Smagorinsky" || turbulenceType == "WALE")
		{
			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
			}
			else if (boundaryType == "wall")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_32->show();
				ui->comboBox_82->addItem("Standard Wall Function");
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "k Equation" || turbulenceType == "Dynamic k Equation")
		{
			ui->tabWidget->setTabVisible(2, true);

			//k
			ui->comboBox_50->clear();
			if (boundaryType == "patch")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Turbulent Intensity Inlet");
				ui->comboBox_50->addItem("Atmospheric Inlet");
				ui->comboBox_50->addItem("Zero Gradient");
				ui->comboBox_50->addItem("Fixed Value");
				ui->comboBox_50->addItem("Fixed Mean Value");
				ui->comboBox_50->addItem("Uniform Fixed Value");
				ui->comboBox_50->addItem("Uniform Fixed Gradient");
				ui->comboBox_50->addItem("Fixed Gradient");
				ui->comboBox_50->addItem("Inlet-Outlet");
				ui->comboBox_50->addItem("Uniform Inlet-Outlet");
				ui->comboBox_50->addItem("Outlet-Inlet");
				ui->comboBox_50->addItem("Free Stream");
				ui->comboBox_50->addItem("Turbulent Inlet");
				ui->comboBox_50->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Standard Wall Function");
				ui->comboBox_50->addItem("Low Re Wall Function");
				ui->comboBox_50->addItem("Fixed Value");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Overset");
			}
			connect(ui->comboBox_50, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_50_itemActivated);
			on_comboBox_50_itemActivated(0);

			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
			}
			else if (boundaryType == "wall")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Standard Wall Function");
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
	}
	else if (solver == "buoyantBoussinesqPimpleFoam")
	{
		ui->tabWidget->setTabVisible(0, true);
		ui->tabWidget->setTabVisible(1, true);
		ui->tabWidget->setTabVisible(2, true);

		//p-ρgh
		ui->comboBox_16->clear();
		if (boundaryType == "patch")
		{
			ui->widget_23->show();
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
			ui->widget_23->show();
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
			ui->widget_23->show();
			ui->comboBox_16->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->widget_23->show();
			ui->comboBox_16->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->widget_23->show();
			ui->comboBox_16->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->widget_23->show();
			ui->comboBox_16->addItem("Overset");
		}
		connect(ui->comboBox_16, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_16_itemActivated);
		on_comboBox_16_itemActivated(0);

		//U
		ui->comboBox_26->clear();
		if (boundaryType == "patch")
		{
			ui->widget_55->show();
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
			ui->widget_55->show();
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
			ui->widget_55->show();
			ui->comboBox_26->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->widget_55->show();
			ui->comboBox_26->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->widget_55->show();
			ui->comboBox_26->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->widget_55->show();
			ui->comboBox_26->addItem("Overset");
		}
		connect(ui->comboBox_26, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_26_itemActivated);
		on_comboBox_26_itemActivated(0);

		//T
		ui->comboBox_43->clear();
		if (boundaryType == "patch")
		{
			ui->widget_57->show();
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
			ui->widget_57->show();
			ui->comboBox_43->addItem("Zero Gradient");
			ui->comboBox_43->addItem("Fixed Value");
			ui->comboBox_43->addItem("Fixed Mean Value");
			ui->comboBox_43->addItem("Uniform Fixed Value");
			ui->comboBox_43->addItem("Uniform Fixed Gradient");
			ui->comboBox_43->addItem("Fixed Gradient");
		}
		else if (boundaryType == "symmetry")
		{
			ui->widget_57->show();
			ui->comboBox_43->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->widget_57->show();
			ui->comboBox_43->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->widget_57->show();
			ui->comboBox_43->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->widget_57->show();
			ui->comboBox_43->addItem("Overset");
		}
		connect(ui->comboBox_43, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_43_itemActivated);
		on_comboBox_43_itemActivated(0);

		//湍流参数
		if (turbulenceType == "Laminar")
		{
			ui->tabWidget->setTabVisible(2, true);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Calculated");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "k-ε" || turbulenceType == "RNG k-ε" || turbulenceType == "Realizable k-ε")
		{
			ui->tabWidget->setTabVisible(2, true);

			//k
			ui->comboBox_50->clear();
			if (boundaryType == "patch")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Turbulent Intensity Inlet");
				ui->comboBox_50->addItem("Atmospheric Inlet");
				ui->comboBox_50->addItem("Zero Gradient");
				ui->comboBox_50->addItem("Fixed Value");
				ui->comboBox_50->addItem("Fixed Mean Value");
				ui->comboBox_50->addItem("Uniform Fixed Value");
				ui->comboBox_50->addItem("Uniform Fixed Gradient");
				ui->comboBox_50->addItem("Fixed Gradient");
				ui->comboBox_50->addItem("Inlet-Outlet");
				ui->comboBox_50->addItem("Uniform Inlet-Outlet");
				ui->comboBox_50->addItem("Outlet-Inlet");
				ui->comboBox_50->addItem("Free Stream");
				ui->comboBox_50->addItem("Turbulent Inlet");
				ui->comboBox_50->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Standard Wall Function");
				ui->comboBox_50->addItem("Low Re Wall Function");
				ui->comboBox_50->addItem("Fixed Value");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Overset");
			}
			connect(ui->comboBox_50, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_50_itemActivated);
			on_comboBox_50_itemActivated(0);

			//ε
			ui->comboBox_68->clear();
			if (boundaryType == "patch")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Turbulent Mixing Length Inlet");
				ui->comboBox_68->addItem("Zero Gradient");
				ui->comboBox_68->addItem("Fixed Value");
				ui->comboBox_68->addItem("Fixed Mean Value");
				ui->comboBox_68->addItem("Uniform Fixed Value");
				ui->comboBox_68->addItem("Uniform Fixed Gradient");
				ui->comboBox_68->addItem("Fixed Gradient");
				ui->comboBox_68->addItem("Inlet-Outlet");
				ui->comboBox_68->addItem("Uniform Inlet-Outlet");
				ui->comboBox_68->addItem("Outlet-Inlet");
				ui->comboBox_68->addItem("Free Stream");
				ui->comboBox_68->addItem("Turbulent Inlet");
				ui->comboBox_68->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Standard Wall Function");
				ui->comboBox_68->addItem("Low Re Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Overset");
			}
			connect(ui->comboBox_68, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_68_itemActivated);
			on_comboBox_68_itemActivated(0);

			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Calculated");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "Spalart-Allmaras" || turbulenceType == "Spalart-Allmaras DES" || turbulenceType == "Spalart-Allmaras DDES" || turbulenceType == "Spalart-Allmaras IDDES")
		{
			ui->tabWidget->setTabVisible(2, true);

			//v_SA
			ui->comboBox_74->clear();
			if (boundaryType == "patch")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Fixed Value");
				ui->comboBox_74->addItem("Zero Gradient");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Fixed Value");
				ui->comboBox_74->addItem("Zero Gradient");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Overset");
			}
			connect(ui->comboBox_74, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_74_itemActivated);
			on_comboBox_74_itemActivated(0);

			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Calculated");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "k-ω" || turbulenceType == "k-ω SST" || turbulenceType == "k-ω SST SAS" || turbulenceType == "k-ω SST DDES" || turbulenceType == "k-ω SST IDDES" || turbulenceType == "k-ω SST DES" || turbulenceType == "Spalart-Allmaras DES" || turbulenceType == "Spalart-Allmaras DDES" || turbulenceType == "Spalart-Allmaras IDDES")
		{
			ui->tabWidget->setTabVisible(2, true);

			//k
			ui->comboBox_50->clear();
			if (boundaryType == "patch")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Turbulent Intensity Inlet");
				ui->comboBox_50->addItem("Atmospheric Inlet");
				ui->comboBox_50->addItem("Zero Gradient");
				ui->comboBox_50->addItem("Fixed Value");
				ui->comboBox_50->addItem("Fixed Mean Value");
				ui->comboBox_50->addItem("Uniform Fixed Value");
				ui->comboBox_50->addItem("Uniform Fixed Gradient");
				ui->comboBox_50->addItem("Fixed Gradient");
				ui->comboBox_50->addItem("Inlet-Outlet");
				ui->comboBox_50->addItem("Uniform Inlet-Outlet");
				ui->comboBox_50->addItem("Outlet-Inlet");
				ui->comboBox_50->addItem("Free Stream");
				ui->comboBox_50->addItem("Turbulent Inlet");
				ui->comboBox_50->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Standard Wall Function");
				ui->comboBox_50->addItem("Low Re Wall Function");
				ui->comboBox_50->addItem("Fixed Value");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Overset");
			}
			connect(ui->comboBox_50, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_50_itemActivated);
			on_comboBox_50_itemActivated(0);

			//ω
			ui->comboBox_75->clear();
			if (boundaryType == "patch")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Turbulent Mixing Length Inlet");
				ui->comboBox_75->addItem("Zero Gradient");
				ui->comboBox_75->addItem("Fixed Value");
				ui->comboBox_75->addItem("Fixed Mean Value");
				ui->comboBox_75->addItem("Uniform Fixed Value");
				ui->comboBox_75->addItem("Uniform Fixed Gradient");
				ui->comboBox_75->addItem("Fixed Gradient");
				ui->comboBox_75->addItem("Inlet-Outlet");
				ui->comboBox_75->addItem("Uniform Inlet-Outlet");
				ui->comboBox_75->addItem("Outlet-Inlet");
				ui->comboBox_75->addItem("Free Stream");
				ui->comboBox_75->addItem("Turbulent Inlet");
				ui->comboBox_75->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Standard Wall Function");
				ui->comboBox_75->addItem("Zero Gradient");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Overset");
			}
			connect(ui->comboBox_75, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_75_itemActivated);
			on_comboBox_75_itemActivated(0);

			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Calculated");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "k-kl-ω")
		{
			ui->tabWidget->setTabVisible(2, true);

			//k_t
			ui->comboBox_56->clear();
			if (boundaryType == "patch")
			{
				ui->widget_25->show();
				ui->comboBox_56->addItem("Turbulent Intensity Inlet");
				ui->comboBox_56->addItem("Atmospheric Inlet");
				ui->comboBox_56->addItem("Zero Gradient");
				ui->comboBox_56->addItem("Fixed Value");
				ui->comboBox_56->addItem("Fixed Mean Value");
				ui->comboBox_56->addItem("Uniform Fixed Value");
				ui->comboBox_56->addItem("Uniform Fixed Gradient");
				ui->comboBox_56->addItem("Fixed Gradient");
				ui->comboBox_56->addItem("Inlet-Outlet");
				ui->comboBox_56->addItem("Uniform Inlet-Outlet");
				ui->comboBox_56->addItem("Outlet-Inlet");
				ui->comboBox_56->addItem("Free Stream");
				ui->comboBox_56->addItem("Turbulent Inlet");
				ui->comboBox_56->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_25->show();
				ui->comboBox_56->addItem("Standard Wall Function");
				ui->comboBox_56->addItem("Low Re Wall Function");
				ui->comboBox_56->addItem("Fixed Value");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_25->show();
				ui->comboBox_56->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_25->show();
				ui->comboBox_56->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_25->show();
				ui->comboBox_56->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_25->show();
				ui->comboBox_56->addItem("Overset");
			}
			connect(ui->comboBox_56, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_56_itemActivated);
			on_comboBox_56_itemActivated(0);

			//k_l
			ui->comboBox_62->clear();
			if (boundaryType == "patch")
			{
				ui->widget_26->show();
				ui->comboBox_62->addItem("Turbulent Intensity Inlet");
				ui->comboBox_62->addItem("Atmospheric Inlet");
				ui->comboBox_62->addItem("Zero Gradient");
				ui->comboBox_62->addItem("Fixed Value");
				ui->comboBox_62->addItem("Fixed Mean Value");
				ui->comboBox_62->addItem("Uniform Fixed Value");
				ui->comboBox_62->addItem("Uniform Fixed Gradient");
				ui->comboBox_62->addItem("Fixed Gradient");
				ui->comboBox_62->addItem("Inlet-Outlet");
				ui->comboBox_62->addItem("Uniform Inlet-Outlet");
				ui->comboBox_62->addItem("Outlet-Inlet");
				ui->comboBox_62->addItem("Free Stream");
				ui->comboBox_62->addItem("Turbulent Inlet");
				ui->comboBox_62->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_26->show();
				ui->comboBox_62->addItem("Standard Wall Function");
				ui->comboBox_62->addItem("Low Re Wall Function");
				ui->comboBox_62->addItem("Fixed Value");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_26->show();
				ui->comboBox_62->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_26->show();
				ui->comboBox_62->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_26->show();
				ui->comboBox_62->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_26->show();
				ui->comboBox_62->addItem("Overset");
			}
			connect(ui->comboBox_62, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_62_itemActivated);
			on_comboBox_62_itemActivated(0);

			//ω
			ui->comboBox_75->clear();
			if (boundaryType == "patch")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Turbulent Mixing Length Inlet");
				ui->comboBox_75->addItem("Zero Gradient");
				ui->comboBox_75->addItem("Fixed Value");
				ui->comboBox_75->addItem("Fixed Mean Value");
				ui->comboBox_75->addItem("Uniform Fixed Value");
				ui->comboBox_75->addItem("Uniform Fixed Gradient");
				ui->comboBox_75->addItem("Fixed Gradient");
				ui->comboBox_75->addItem("Inlet-Outlet");
				ui->comboBox_75->addItem("Uniform Inlet-Outlet");
				ui->comboBox_75->addItem("Outlet-Inlet");
				ui->comboBox_75->addItem("Free Stream");
				ui->comboBox_75->addItem("Turbulent Inlet");
				ui->comboBox_75->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Standard Wall Function");
				ui->comboBox_75->addItem("Zero Gradient");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Overset");
			}
			connect(ui->comboBox_75, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_75_itemActivated);
			on_comboBox_75_itemActivated(0);

			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Calculated");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "Smagorinsky" || turbulenceType == "WALE")
		{
			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Calculated");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "k Equation" || turbulenceType == "Dynamic k Equation")
		{
			ui->tabWidget->setTabVisible(2, true);

			//k
			ui->comboBox_50->clear();
			if (boundaryType == "patch")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Turbulent Intensity Inlet");
				ui->comboBox_50->addItem("Atmospheric Inlet");
				ui->comboBox_50->addItem("Zero Gradient");
				ui->comboBox_50->addItem("Fixed Value");
				ui->comboBox_50->addItem("Fixed Mean Value");
				ui->comboBox_50->addItem("Uniform Fixed Value");
				ui->comboBox_50->addItem("Uniform Fixed Gradient");
				ui->comboBox_50->addItem("Fixed Gradient");
				ui->comboBox_50->addItem("Inlet-Outlet");
				ui->comboBox_50->addItem("Uniform Inlet-Outlet");
				ui->comboBox_50->addItem("Outlet-Inlet");
				ui->comboBox_50->addItem("Free Stream");
				ui->comboBox_50->addItem("Turbulent Inlet");
				ui->comboBox_50->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Standard Wall Function");
				ui->comboBox_50->addItem("Low Re Wall Function");
				ui->comboBox_50->addItem("Fixed Value");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Overset");
			}
			connect(ui->comboBox_50, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_50_itemActivated);
			on_comboBox_50_itemActivated(0);

			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Calculated");
			}
			else if (boundaryType == "wall")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
	}
	else if (solver == "chtMultiRegionSimpleFoam")
	{
		ui->tabWidget->setTabVisible(0, true);
		ui->tabWidget->setTabVisible(1, true);
		if (turbulenceType != "Laminar")ui->tabWidget->setTabVisible(2, true);

		//p-ρgh
		ui->comboBox_16->clear();
		if (boundaryType == "patch")
		{
			ui->widget_23->show();
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
			ui->widget_23->show();
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
			ui->widget_23->show();
			ui->comboBox_16->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->widget_23->show();
			ui->comboBox_16->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->widget_23->show();
			ui->comboBox_16->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->widget_23->show();
			ui->comboBox_16->addItem("Overset");
		}
		connect(ui->comboBox_16, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_16_itemActivated);
		on_comboBox_16_itemActivated(0);

		//U
		ui->comboBox_26->clear();
		if (boundaryType == "patch")
		{
			ui->widget_55->show();
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
			ui->widget_55->show();
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
			ui->widget_55->show();
			ui->comboBox_26->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->widget_55->show();
			ui->comboBox_26->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->widget_55->show();
			ui->comboBox_26->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->widget_55->show();
			ui->comboBox_26->addItem("Overset");
		}
		connect(ui->comboBox_26, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_26_itemActivated);
		on_comboBox_26_itemActivated(0);

		//T
		ui->comboBox_43->clear();
		if (boundaryType == "patch")
		{
			ui->widget_57->show();
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
		else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
		{
			ui->widget_57->show();
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
			ui->widget_57->show();
			ui->comboBox_43->addItem("Coupled Temperature and Radiation");
			ui->comboBox_43->addItem("Coupled Temperature");
		}
		else if (boundaryType == "symmetry")
		{
			ui->widget_57->show();
			ui->comboBox_43->addItem("Symmetry");
		}
		else if (boundaryType == "empty")
		{
			ui->widget_57->show();
			ui->comboBox_43->addItem("Empty");
		}
		else if (boundaryType == "wedge")
		{
			ui->widget_57->show();
			ui->comboBox_43->addItem("Wedge");
		}
		else if (boundaryType == "overset")
		{
			ui->widget_57->show();
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
			ui->tabWidget->setTabVisible(2, true);

			//k
			ui->comboBox_50->clear();
			if (boundaryType == "patch")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Turbulent Intensity Inlet");
				ui->comboBox_50->addItem("Atmospheric Inlet");
				ui->comboBox_50->addItem("Zero Gradient");
				ui->comboBox_50->addItem("Fixed Value");
				ui->comboBox_50->addItem("Fixed Mean Value");
				ui->comboBox_50->addItem("Uniform Fixed Value");
				ui->comboBox_50->addItem("Uniform Fixed Gradient");
				ui->comboBox_50->addItem("Fixed Gradient");
				ui->comboBox_50->addItem("Inlet-Outlet");
				ui->comboBox_50->addItem("Uniform Inlet-Outlet");
				ui->comboBox_50->addItem("Outlet-Inlet");
				ui->comboBox_50->addItem("Free Stream");
				ui->comboBox_50->addItem("Turbulent Inlet");
				ui->comboBox_50->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Standard Wall Function");
				ui->comboBox_50->addItem("Low Re Wall Function");
				ui->comboBox_50->addItem("Fixed Value");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Overset");
			}
			connect(ui->comboBox_50, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_50_itemActivated);
			on_comboBox_50_itemActivated(0);

			//ε
			ui->comboBox_68->clear();
			if (boundaryType == "patch")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Turbulent Mixing Length Inlet");
				ui->comboBox_68->addItem("Zero Gradient");
				ui->comboBox_68->addItem("Fixed Value");
				ui->comboBox_68->addItem("Fixed Mean Value");
				ui->comboBox_68->addItem("Uniform Fixed Value");
				ui->comboBox_68->addItem("Uniform Fixed Gradient");
				ui->comboBox_68->addItem("Fixed Gradient");
				ui->comboBox_68->addItem("Inlet-Outlet");
				ui->comboBox_68->addItem("Uniform Inlet-Outlet");
				ui->comboBox_68->addItem("Outlet-Inlet");
				ui->comboBox_68->addItem("Free Stream");
				ui->comboBox_68->addItem("Turbulent Inlet");
				ui->comboBox_68->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Standard Wall Function");
				ui->comboBox_68->addItem("Low Re Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_27->show();
				ui->comboBox_68->addItem("Overset");
			}
			connect(ui->comboBox_68, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_68_itemActivated);
			on_comboBox_68_itemActivated(0);

			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Standard Wall Function");
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "Spalart-Allmaras" || turbulenceType == "Spalart-Allmaras DES" || turbulenceType == "Spalart-Allmaras DDES" || turbulenceType == "Spalart-Allmaras IDDES")
		{
			ui->tabWidget->setTabVisible(2, true);

			//v_SA
			ui->comboBox_74->clear();
			if (boundaryType == "patch")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Fixed Value");
				ui->comboBox_74->addItem("Zero Gradient");
			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Fixed Value");
				ui->comboBox_74->addItem("Zero Gradient");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_33->show();
				ui->comboBox_74->addItem("Overset");
			}
			connect(ui->comboBox_74, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_74_itemActivated);
			on_comboBox_74_itemActivated(0);

			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Standard Wall Function");
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "k-ω" || turbulenceType == "k-ω SST" || turbulenceType == "k-ω SST SAS" || turbulenceType == "k-ω SST DDES" || turbulenceType == "k-ω SST IDDES" || turbulenceType == "k-ω SST DES" || turbulenceType == "Spalart-Allmaras DES" || turbulenceType == "Spalart-Allmaras DDES" || turbulenceType == "Spalart-Allmaras IDDES")
		{
			ui->tabWidget->setTabVisible(2, true);

			//k
			ui->comboBox_50->clear();
			if (boundaryType == "patch")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Turbulent Intensity Inlet");
				ui->comboBox_50->addItem("Atmospheric Inlet");
				ui->comboBox_50->addItem("Zero Gradient");
				ui->comboBox_50->addItem("Fixed Value");
				ui->comboBox_50->addItem("Fixed Mean Value");
				ui->comboBox_50->addItem("Uniform Fixed Value");
				ui->comboBox_50->addItem("Uniform Fixed Gradient");
				ui->comboBox_50->addItem("Fixed Gradient");
				ui->comboBox_50->addItem("Inlet-Outlet");
				ui->comboBox_50->addItem("Uniform Inlet-Outlet");
				ui->comboBox_50->addItem("Outlet-Inlet");
				ui->comboBox_50->addItem("Free Stream");
				ui->comboBox_50->addItem("Turbulent Inlet");
				ui->comboBox_50->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Standard Wall Function");
				ui->comboBox_50->addItem("Low Re Wall Function");
				ui->comboBox_50->addItem("Fixed Value");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Overset");
			}
			connect(ui->comboBox_50, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_50_itemActivated);
			on_comboBox_50_itemActivated(0);

			//ω
			ui->comboBox_75->clear();
			if (boundaryType == "patch")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Turbulent Mixing Length Inlet");
				ui->comboBox_75->addItem("Zero Gradient");
				ui->comboBox_75->addItem("Fixed Value");
				ui->comboBox_75->addItem("Fixed Mean Value");
				ui->comboBox_75->addItem("Uniform Fixed Value");
				ui->comboBox_75->addItem("Uniform Fixed Gradient");
				ui->comboBox_75->addItem("Fixed Gradient");
				ui->comboBox_75->addItem("Inlet-Outlet");
				ui->comboBox_75->addItem("Uniform Inlet-Outlet");
				ui->comboBox_75->addItem("Outlet-Inlet");
				ui->comboBox_75->addItem("Free Stream");
				ui->comboBox_75->addItem("Turbulent Inlet");
				ui->comboBox_75->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Standard Wall Function");
				ui->comboBox_75->addItem("Zero Gradient");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_28->show();
				ui->comboBox_75->addItem("Overset");
			}
			connect(ui->comboBox_75, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_75_itemActivated);
			on_comboBox_75_itemActivated(0);

			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Standard Wall Function");
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "Smagorinsky" || turbulenceType == "WALE")
		{
			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_32->show();
				ui->comboBox_82->addItem("Standard Wall Function");
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->tabWidget->setTabVisible(2, true);
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
		else if (turbulenceType == "k Equation" || turbulenceType == "Dynamic k Equation")
		{
			ui->tabWidget->setTabVisible(2, true);

			//k
			ui->comboBox_50->clear();
			if (boundaryType == "patch")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Turbulent Intensity Inlet");
				ui->comboBox_50->addItem("Atmospheric Inlet");
				ui->comboBox_50->addItem("Zero Gradient");
				ui->comboBox_50->addItem("Fixed Value");
				ui->comboBox_50->addItem("Fixed Mean Value");
				ui->comboBox_50->addItem("Uniform Fixed Value");
				ui->comboBox_50->addItem("Uniform Fixed Gradient");
				ui->comboBox_50->addItem("Fixed Gradient");
				ui->comboBox_50->addItem("Inlet-Outlet");
				ui->comboBox_50->addItem("Uniform Inlet-Outlet");
				ui->comboBox_50->addItem("Outlet-Inlet");
				ui->comboBox_50->addItem("Free Stream");
				ui->comboBox_50->addItem("Turbulent Inlet");
				ui->comboBox_50->addItem("Mapped Inlet");
			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Standard Wall Function");
				ui->comboBox_50->addItem("Low Re Wall Function");
				ui->comboBox_50->addItem("Fixed Value");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_24->show();
				ui->comboBox_50->addItem("Overset");
			}
			connect(ui->comboBox_50, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_50_itemActivated);
			on_comboBox_50_itemActivated(0);

			//v_t
			ui->comboBox_81->clear();
			if (boundaryType == "patch")
			{

			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Standard Wall Function");
				ui->comboBox_81->addItem("Standard Wall Function(k)");
				ui->comboBox_81->addItem("Spalding Wall Function");
				ui->comboBox_81->addItem("Blended Wall Function");
				ui->comboBox_81->addItem("Low Re Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function");
				ui->comboBox_81->addItem("Rough Wall Function(k)");
				ui->comboBox_81->addItem("Atmospheric Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_31->show();
				ui->comboBox_81->addItem("Overset");
			}
			connect(ui->comboBox_81, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_81_itemActivated);
			on_comboBox_81_itemActivated(0);

			//α_t
			ui->comboBox_82->clear();
			if (boundaryType == "patch")
			{
			}
			else if (boundaryType == "wall" || boundaryType.startsWith("mappedWall"))
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Standard Wall Function");
				ui->comboBox_82->addItem("Jayatilleke Wall Function");
			}
			else if (boundaryType == "symmetry")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Symmetry");
			}
			else if (boundaryType == "empty")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Empty");
			}
			else if (boundaryType == "wedge")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Wedge");
			}
			else if (boundaryType == "overset")
			{
				ui->widget_32->show();
				ui->comboBox_82->addItem("Overset");
			}
			connect(ui->comboBox_82, qOverload<int>(&QComboBox::activated), this, &FormBoundaryConditionsTabWidget::on_comboBox_82_itemActivated);
			on_comboBox_82_itemActivated(0);
		}
	}
}