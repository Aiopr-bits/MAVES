#pragma once

#include <QWidget>
#include "ui_FormModelClip.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormModelClipClass; };
QT_END_NAMESPACE

class FormModelClip : public QWidget
{
	Q_OBJECT

public:
	FormModelClip(QWidget *parent = nullptr);
	~FormModelClip();

signals:
	void checkBoxToggled(bool checked);

public:
	Ui::FormModelClipClass *ui;
};
