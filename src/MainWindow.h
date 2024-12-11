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
#include "FormMesh.h"
#include "FormPostprocessing.h"
#include "GlobalData.h"

#pragma execution_character_set("utf-8")

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
	void hideAllSubForm();										    //隐藏所有子窗口

public slots:
	void handleAction1Triggered();								    //信息框
	void handleAction2Triggered();								    //x正向
	void handleAction3Triggered();								    //x负向
	void handleAction4Triggered();								    //y正向
	void handleAction5Triggered();								    //y负向
	void handleAction6Triggered();								    //z正向
	void handleAction7Triggered();								    //z负向
	void handleAction8Triggered();								    //适应窗口

	void on_pushButton_clicked();								    //几何导入
	void on_pushButton_4_clicked();								    //网格导入
	void on_pushButton_2_clicked();								    //网格
	void on_pushButton_17_clicked();								//后处理




public:
	Ui::MainWindowClass *ui;

	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderer> render;
	vtkSmartPointer<vtkOrientationMarkerWidget> axesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();//左下角三维坐标轴

	FormMesh* formMesh;
	FormPostprocessing* formPostprocessing;
};
