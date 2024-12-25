#pragma once

#include <QWidget>
#include "ui_ModelSlice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ModelSliceClass; };
QT_END_NAMESPACE

class ModelSlice : public QWidget
{
	Q_OBJECT

public:
	ModelSlice(QWidget *parent = nullptr);
	~ModelSlice();

private:
	Ui::ModelSliceClass *ui;
};
