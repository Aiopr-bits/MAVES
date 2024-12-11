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

	void addCoordinateAxes();									    //������½���ά������
	void hideAllSubForm();										    //���������Ӵ���

public slots:
	void handleAction1Triggered();								    //��Ϣ��
	void handleAction2Triggered();								    //x����
	void handleAction3Triggered();								    //x����
	void handleAction4Triggered();								    //y����
	void handleAction5Triggered();								    //y����
	void handleAction6Triggered();								    //z����
	void handleAction7Triggered();								    //z����
	void handleAction8Triggered();								    //��Ӧ����

	void on_pushButton_clicked();								    //���ε���
	void on_pushButton_4_clicked();								    //������
	void on_pushButton_2_clicked();								    //����
	void on_pushButton_17_clicked();								//����




public:
	Ui::MainWindowClass *ui;

	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderer> render;
	vtkSmartPointer<vtkOrientationMarkerWidget> axesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();//���½���ά������

	FormMesh* formMesh;
	FormPostprocessing* formPostprocessing;
};
