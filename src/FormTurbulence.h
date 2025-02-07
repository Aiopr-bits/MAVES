#pragma once

#include <QWidget>
#include "ui_FormTurbulence.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormTurbulenceClass; };
QT_END_NAMESPACE

class FormTurbulence : public QWidget
{
	Q_OBJECT

public:
	FormTurbulence(QWidget *parent = nullptr);
	~FormTurbulence();

private:
	Ui::FormTurbulenceClass *ui;
};
