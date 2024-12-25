#pragma once

#include <QWidget>
#include "ui_ModelSlice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ModelSliceClass; };
QT_END_NAMESPACE

class FormModelSlice : public QWidget
{
	Q_OBJECT

public:
	FormModelSlice(QWidget *parent = nullptr);
	~FormModelSlice();

private:
	Ui::ModelSliceClass *ui;
};
