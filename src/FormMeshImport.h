#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormMeshImport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormMeshImportClass; };
QT_END_NAMESPACE

class FormMeshImport : public QWidget
{
	Q_OBJECT

public:
	FormMeshImport(QWidget *parent = nullptr);
	~FormMeshImport();

private slots:
	void on_LineEditText_Changed(const QString& text);
	void on_pushButton_clicked();
	void on_pushButton_2_clicked();

signals:
	void meshImported(const QString& filePath);

public:
	Ui::FormMeshImportClass *ui;
};
