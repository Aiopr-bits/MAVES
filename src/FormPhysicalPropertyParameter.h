#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormPhysicalPropertyParameter.h"
#include "FormSolver.h"
#include "GlobalData.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormPhysicalPropertyParameterClass; };
QT_END_NAMESPACE

class FormPhysicalPropertyParameter : public QWidget
{
	Q_OBJECT

public:
	FormPhysicalPropertyParameter(QWidget *parent = nullptr);
	~FormPhysicalPropertyParameter();
	
	bool importParameter();
	bool exportParameter();

public slots:
	void solverChanged(const QString& newText);

private:
	Ui::FormPhysicalPropertyParameterClass *ui;
};
