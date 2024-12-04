#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
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
#include <vtkDataSetMapper.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkExtractEdges.h>
#include <vtkProperty.h> 
#include <vtkLookupTable.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <vtkFeatureEdges.h>
#include <vtkGeometryFilter.h>
#include <vtkScalarBarActor.h>
#include <vtkTextProperty.h>
#include <QStandardItemModel.h>
#include <vtkOrientationMarkerWidget.h>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void addCoordinateAxes();									    //添加左下角三维坐标轴

public slots:
	void handleAction1Triggered();								    //信息框
	void handleAction2Triggered();								    //x正向
	void handleAction3Triggered();								    //x负向
	void handleAction4Triggered();								    //y正向
	void handleAction5Triggered();								    //y负向
	void handleAction6Triggered();								    //z正向
	void handleAction7Triggered();								    //z负向
	void handleAction8Triggered();								    //适应窗口


private:
	Ui::MainWindowClass *ui;

	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkOrientationMarkerWidget> axesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();//左下角三维坐标轴

};
