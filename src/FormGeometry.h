#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormGeometry.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormGeometryClass; };
QT_END_NAMESPACE

class FormGeometry : public QWidget
{
	Q_OBJECT

public:
	FormGeometry(QWidget *parent = nullptr);
	~FormGeometry();

private slots:
	void on_LineEditText_Changed(const QString& text);
	void on_pushButton_clicked();
	void on_pushButton_2_clicked();

signals:
	void geometryImported(const QString& filePath);

private:
	Ui::FormGeometryClass *ui;
};
