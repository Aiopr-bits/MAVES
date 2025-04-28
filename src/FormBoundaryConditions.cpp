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

	//this->widget添加一个FormBoundaryConditionsTabWidget
	FormBoundaryConditionsTabWidget* tabWidget = new FormBoundaryConditionsTabWidget(this);

	// 检查 this->widget 是否已有布局
	if (!ui->widget->layout()) {
		// 如果没有布局，创建一个 QVBoxLayout
		QVBoxLayout* layout = new QVBoxLayout(ui->widget);
		layout->setContentsMargins(0, 0, 0, 0); // 可选：移除边距
		ui->widget->setLayout(layout);
	}

	// 将 tabWidget 添加到 this->widget 的布局中
	ui->widget->layout()->addWidget(tabWidget);
}

FormBoundaryConditions::~FormBoundaryConditions()
{
	delete ui;
}

void FormBoundaryConditions::initialization(const QString& solver)
{

}