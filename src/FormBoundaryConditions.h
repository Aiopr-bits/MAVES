#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormBoundaryConditions.h"
#include "GlobalData.h"
#include <QStandardItemModel>
#include <QMessageBox.h>

QT_BEGIN_NAMESPACE
namespace Ui { class FormBoundaryConditionsClass; };
QT_END_NAMESPACE

enum class PhysicalFields {
	p,
	T,
	U,
	k,
	nut,
	omega,
	alphat,
	COUNT = 7,
};

enum class pType {
	zeroGradient,
	fixedValue,
};

enum class TType {
	zeroGradient,
	fixedValue,
	externalWallHeatFluxTemperature,
};

enum class UType {
	zeroGradient,
	fixedValue,
	noSlip,
};

enum class kType {
	fixedValue,
	inletOutlet,
	kqRWallFunction,
};

enum class nutType {
	calculated,
	nutkWallFunction,
};

enum class omegaType {
	fixedValue,
	inletOutlet,
	omegaWallFunction,
};

enum class alphatType {
	calculated,
	alphatWallFunction,
};

class FormBoundaryConditions : public QWidget
{
	Q_OBJECT

public:
	FormBoundaryConditions(QWidget *parent = nullptr);
	~FormBoundaryConditions();
	void onMeshImported();
	void initListView();
	void initTabWidget();

private slots:
    void onListViewItemClicked(const QModelIndex& index);

private:
	Ui::FormBoundaryConditionsClass *ui;
	QStandardItemModel* listViewModel;
	
};
