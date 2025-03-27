#pragma once

#include <QWidget>
#include "ui_FormTransportProperties.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormTransportPropertiesClass; };
QT_END_NAMESPACE

class FormTransportProperties : public QWidget
{
	Q_OBJECT

public:
	FormTransportProperties(QWidget *parent = nullptr);
	~FormTransportProperties();

private:
	Ui::FormTransportPropertiesClass *ui;
};
