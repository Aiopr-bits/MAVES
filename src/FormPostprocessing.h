#pragma once

#include <QWidget>
#include "ui_FormPostprocessing.h"
#include <QSvgRenderer>
#include <QPixmap>
#include <QPainter>
#include "GlobalData.h"
#include <QStandardItemModel>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkXMLPolyDataReader.h>
#include <tuple>
#include <vtkUnstructuredGridReader.h>
#include <vtkDataSetMapper.h>
#include <QMessageBox>
#include <QFileDialog>
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPolyDataReader.h>
#include <vtkExtractEdges.h>
#include <vtkProperty.h> 
#include <vtkLookupTable.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <vtkFeatureEdges.h>
#include <vtkGeometryFilter.h>
#include <vtkScalarBarActor.h>
#include <vtkTextProperty.h>

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

signals:
	void resultDataLoaded();

private:
	Ui::FormPostprocessingClass *ui;
	QStandardItemModel* treeViewModel;
};
