#pragma once

#include <QWidget>
#include "ui_FormCellZones.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormCellZonesClass; };
QT_END_NAMESPACE

class FormCellZones : public QWidget
{
	Q_OBJECT

public:
	FormCellZones(QWidget *parent = nullptr);
	~FormCellZones();

private:
	Ui::FormCellZonesClass *ui;
};
