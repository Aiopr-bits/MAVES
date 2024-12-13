#pragma once

#include <QWidget>
#include "ui_FormGeometry.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormGeometryClass; };
QT_END_NAMESPACE

class FormGeometry : public QWidget
{
	Q_OBJECT

public:
	FormGeometry(QWidget *parent = nullptr);
	~FormGeometry();

private:
	Ui::FormGeometryClass *ui;
};
