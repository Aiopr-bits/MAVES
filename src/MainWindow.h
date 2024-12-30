#pragma once
#include <QMainWindow>
#include <QMap>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
QT_CHARTS_USE_NAMESPACE
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
#include <tuple>
#include "ui_FormPostprocessing.h"
#include "GlobalData.h"
#include "FormMesh.h"
#include "FormPostprocessing.h"
#include "FormRun.h"
#include "FormGeometry.h"
#include "FormMeshImport.h"
#include "FormModelClip.h"
#include <QProcess>
#include <QLogValueAxis>
#include <QChart>
#include <QTimer>
#include <QDateTimeAxis>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkClipPolyData.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkPolyData.h>
#include <vtkCamera.h>
#include <vtkLight.h>
#include <vtkPropCollection.h>
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>
#include <iostream>
#include <vtkOBJReader.h>
#include <vtkPLYReader.h>
#include <vtkXMLPolyDataReader.h>
#include <qdebug.h>
#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkWorldPointPicker.h>
#include <IGESControl_Reader.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <QBoxLayout>
#include <QDockWidget>
#include <QLineEdit>
#include <QMenuBar>
#include <QTextBrowser>
#include <QToolBar>
#include <QTreeWidget>
#include <STEPControl_Reader.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <QMetaType>
#include <QMessageBox.h>
#include <vtkOpenFOAMReader.h>
#include <QString>
#include <vtkColorTransferFunction.h>
#include <QStringList>
#include <QScrollBar>
#include <vtkClipClosedSurface.h>
#include <vtkPlaneCollection.h>
#include <vtkNamedColors.h>
#include <vtkContourTriangulator.h> 
#include <vtkProbeFilter.h>
#include <vtkClipDataSet.h>
#include <vtkDataSetSurfaceFilter.h>

#ifdef _WIN32
#include "qt_windows.h"
#endif

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

	void addCoordinateAxes();									    													//������½���ά������
	void hideAllSubForm();										    													//���������Ӵ���
	void parseOutput(const QString& output);																			//�������

public slots:
	//�������źŴ���
	void handleAction1Triggered();								    													//��Ϣ��
	void handleAction2Triggered();								    													//x����
	void handleAction3Triggered();								    													//x����
	void handleAction4Triggered();								    													//y����
	void handleAction5Triggered();								    													//y����
	void handleAction6Triggered();								    													//z����
	void handleAction7Triggered();								    													//z����
	void handleAction8Triggered();								    													//��Ӧ����
	void handleAction9Triggered();								    													//ģ���з�

	//������尴ť����¼�����
	void on_pushButton_clicked();								    													//����ҳ��
	void on_pushButton_4_clicked();								    													//������ҳ��
	void on_pushButton_2_clicked();								   														//����ҳ��
	void on_pushButton_16_clicked();																					//������ҳ��
	void on_pushButton_17_clicked();																					//����ҳ��
	void on_pushButton_3_clicked();								    													//ģ���з�

	//�����������¼�����
	void onPlayTimerTimeout();																							//����
	void onReverseTimerTimeout(); 																						//����
	void onLoopPlayTimerTimeout(); 																						//ѭ������
	void onButtonClicked();																								//������尴ť�������ɫ����
	void onProcessRunFinished(int exitCode, QProcess::ExitStatus exitStatus);											//��������̽���
	void onProcessFoamToVTKFinished(int exitCode, QProcess::ExitStatus exitStatus);										//foamToVTK���̽���
	void onProcessRunOutput();																							//������������
	void onprocessFoamToVTKOutput(); 																					//foamToVTK�������
	void onProcessOutput(); 																							//�������
	void onProcessError();																								//���̴������
	void updateChart();											    													//���²в�ͼ
	void updatePlaneRepModelClipValues();								    											//����ģ���з�ƽ��ѡ������ֵ

	//����������¼�����
	void formGeometry_import(const QString& filePath);																	//���ε���
	void formMeshImport_import(const QString& filePath);																//������
	void formMesh_apply(); 																								//������Ⱦ����	
	void formRun_run();																									//������
	void formRun_stopRun();																								//ֹͣ����
	void formPostprocessing_loadData();																					//���ؽ������		
	void formPostprocessing_apply();																					//������Ⱦ����	
	void formPostprocessing_firstFrame();																				//��һ֡
	void formPostprocessing_previousFrame();																			//��һ֡
	void formPostprocessing_reverse();																					//���²���
	void formPostprocessing_play();																						//����
	void formPostprocessing_nextFrame();																				//��һ֡
	void formPostprocessing_lastFrame();																				//���һ֡
	void formPostprocessing_loopPlay();																					//ѭ������
	void formPostprocessing_playPause();																				//������ͣ
	void formPostprocessing_reversePause();																				//���򲥷���ͣ
	void formPostprocessing_loopPlayPause();																			//ѭ��������ͣ
	void formModelClip_checkBoxToggle();																				//ģ���з�ҳ��CheckBox�л�
	void formModelClip_lineEditsChanged();																				//ģ���з�ҳ��LineEditֵ�ı�
	void formModelClip_xPositive();																						//ģ���з֣�X����
	void formModelClip_yPositive();																						//ģ���з֣�Y����
	void formModelClip_zPositive();																						//ģ���з֣�Z����
	void formModelClip_cameraDirection();																				//ģ���з֣��������
	void formModelClip_alignView();																						//ģ���з֣������ӽ�
	void formModelClip_resetPlane();																					//ģ���з֣�����ƽ��
	void formModelClip_apply();																							//ģ���з֣�Ӧ��

public:
	Ui::MainWindowClass *ui;
	QTimer* playTimer;
	QTimer* reverseTimer;
	QTimer* loopPlayTimer;
	QPushButton* buttons[20];
	QPushButton* lastClickedButton;
	QProcess processRun;
	QProcess processFoamToVTK;
	QProcess process;

	//��Ⱦ����
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderer> render;
	vtkSmartPointer<vtkOrientationMarkerWidget> axesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();		//���½���ά������

	//���������
	FormMesh* formMesh;
	FormPostprocessing* formPostprocessing;
	FormRun* formRun;
	FormGeometry* formGeometry;
	FormMeshImport* formMeshImport;
	FormModelClip* formModelClip;

	//�в�ͼ��ر���
	QChart* chart;
	QValueAxis* axisX;
	QLogValueAxis* axisY;
	QMap<QString, QLineSeries*> seriesMap;
	QMap<QString, QPair<double, double>> seriesRangeMap;
	QTimer* chartUpdateTimer;
	QMap<QString, double> residuals;																				// ���ڴ洢��ǰʱ�䲽�Ĳв�����
	double currentTimeStep;
	int axisMinX;
	int axisMaxX;
	double axisMinY;
	double axisMaxY;

	//ƽ��ѡ������ر���
	vtkSmartPointer<vtkImplicitPlaneWidget2> planeWidgetModelClip;
	vtkSmartPointer<vtkImplicitPlaneRepresentation> planeRepModelClip;
};
