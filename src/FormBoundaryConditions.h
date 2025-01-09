#pragma once

#include <QWidget>
#include "ui_FormBoundaryConditions.h"
#include "GlobalData.h"
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class FormBoundaryConditionsClass; };
QT_END_NAMESPACE

class FormBoundaryConditions : public QWidget
{
	Q_OBJECT

public:
	FormBoundaryConditions(QWidget *parent = nullptr);
	~FormBoundaryConditions();
	void onMeshImported();

private:
	Ui::FormBoundaryConditionsClass *ui;
	QStandardItemModel* listViewModel;
	
};
