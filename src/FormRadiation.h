#pragma once

#include <QWidget>
#include "ui_FormRadiation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormRadiationClass; };
QT_END_NAMESPACE

class FormRadiation : public QWidget
{
	Q_OBJECT

public:
	FormRadiation(QWidget *parent = nullptr);
	~FormRadiation();

private:
	Ui::FormRadiationClass *ui;
};
