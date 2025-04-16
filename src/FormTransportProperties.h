#pragma once
#pragma execution_character_set("utf-8")
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

public slots:
	void on_comboBox_itemActivated(int index);

private:
	Ui::FormTransportPropertiesClass *ui;
};
