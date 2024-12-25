#pragma once

#include <QWidget>
#include "ui_FormModelSlice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormModelSliceClass; };
QT_END_NAMESPACE

class FormModelSlice : public QWidget
{
	Q_OBJECT

public:
	FormModelSlice(QWidget *parent = nullptr);
	~FormModelSlice();

public slots:
	void onLineEditChanged();

signals:
	void checkBoxToggled(bool checked);
	void lineEditsChanged();

public:
	Ui::FormModelSliceClass *ui;
};
