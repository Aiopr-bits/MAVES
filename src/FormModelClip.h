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

public slots:
	void onLineEditChanged();

signals:
	void checkBoxToggled(bool checked);
	void lineEditsChanged();

public:
	Ui::FormModelClipClass *ui;
};
