#pragma once

#include <QWidget>
#include "ui_FormPostprocessing.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormPostprocessingClass; };
QT_END_NAMESPACE

class FormPostprocessing : public QWidget
{
	Q_OBJECT

public:
	FormPostprocessing(QWidget *parent = nullptr);
	~FormPostprocessing();

private:
	Ui::FormPostprocessingClass *ui;
};
