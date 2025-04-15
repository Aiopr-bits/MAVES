#pragma once

#include <QMainWindow>
#include "ui_FormThermoListWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormThermoListWidgetClass; };
QT_END_NAMESPACE

class FormThermoListWidget : public QMainWindow
{
	Q_OBJECT

public:
	FormThermoListWidget(QWidget *parent = nullptr, QString widgetType = "fluid");
	~FormThermoListWidget();

	void initialize(QString widgetType);
	void hideAllSubPanel();

public slots:
	void on_comboBox_6_itemActivated(int index);
	void on_comboBox_7_itemActivated(int index);
	void on_comboBox_8_itemActivated(int index);
	void on_spinBox_valueChanged(int value);
	void on_spinBox_2_valueChanged(int value);
	void on_spinBox_3_valueChanged(int value);
	void on_spinBox_4_valueChanged(int value);
	void on_spinBox_5_valueChanged(int value);

private:
	Ui::FormThermoListWidgetClass *ui;
	std::vector<std::string> equationOfState1;
	std::vector<std::string> equationOfState2;
	std::vector<std::string> thermodynamics1;
	std::vector<std::string> thermodynamics2;
	std::vector<std::string> thermodynamics3;
	std::vector<std::string> thermodynamics4;
	std::vector<std::string> thermodynamics5;
	std::vector<std::string> transport1;
	std::vector<std::string> transport2;
	std::vector<std::string> transport3;
	std::vector<std::string> transport4;
	std::vector<std::string> transport5;
	QString widgetType;
};
