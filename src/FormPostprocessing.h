#pragma once

#include <QWidget>
#include "ui_FormPostprocessing.h"
#include <QSvgRenderer>
#include <QPixmap>
#include <QPainter>
#include "GlobalData.h"
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class FormPostprocessingClass; };
QT_END_NAMESPACE

class FormPostprocessing : public QWidget
{
	Q_OBJECT

public:
	FormPostprocessing(QWidget *parent = nullptr);
	~FormPostprocessing();

	void loadResultData();

public slots:
	void on_pushButton_clicked();
	void on_treeView_itemClicked(const QModelIndex& index);

private:
	Ui::FormPostprocessingClass *ui;
	QStandardItemModel* treeViewModel;
};
