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
#include <QProcess>
#include <QLogValueAxis>
#include <QChart>
#include <QTimer>
#include <QDateTimeAxis>

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
	void parseOutput(const QString& output);						//�������
	void updateChart();											    //���²в�ͼ

public slots:
	void handleAction1Triggered();								    //��Ϣ��
	void handleAction2Triggered();								    //x����
	void handleAction3Triggered();								    //x����
	void handleAction4Triggered();								    //y����
	void handleAction5Triggered();								    //y����
	void handleAction6Triggered();								    //z����
	void handleAction7Triggered();								    //z����
	void handleAction8Triggered();								    //��Ӧ����

	void on_pushButton_clicked();								    //����ҳ��
	void on_pushButton_4_clicked();								    //������ҳ��
	void on_pushButton_2_clicked();								    //����ҳ��
	void on_pushButton_16_clicked();								//������ҳ��
	void on_pushButton_17_clicked();								//����ҳ��

	void formGeometry_import(const QString& filePath);				//���ε���
	void formMeshImport_import(const QString& filePath);			//������
	void formMesh_apply(); 											//������Ⱦ����	
	void formRun_run();												//������
	void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void formRun_stopRun();											//ֹͣ����
	void formPostprocessing_loadData();								//���ؽ������		
	void formPostprocessing_apply();								//������Ⱦ����	
	void formPostprocessing_firstFrame();							//��һ֡
	void formPostprocessing_previousFrame();						//��һ֡
	void formPostprocessing_reverse();								//���²���
	void formPostprocessing_play();									//����
	void formPostprocessing_nextFrame();							//��һ֡
	void formPostprocessing_lastFrame();							//���һ֡
	void formPostprocessing_loopPlay();								//ѭ������
	void formPostprocessing_playPause();							//������ͣ
	void formPostprocessing_reversePause();							//���򲥷���ͣ
	void formPostprocessing_loopPlayPause();						//ѭ��������ͣ

	void onPlayTimerTimeout();										//����
	void onReverseTimerTimeout(); 									//����
	void onLoopPlayTimerTimeout(); 									//ѭ������

	void onButtonClicked();											//��������尴ť���
	void onProcessOutput();											//�������
	void onProcessError();											//���̴���

public:
	Ui::MainWindowClass *ui;
	QTimer* playTimer;
	QTimer* reverseTimer;
	QTimer* loopPlayTimer;
	QPushButton* buttons[20];
	QPushButton* lastClickedButton;
	QProcess process;

	//��Ⱦ����
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderer> render;
	vtkSmartPointer<vtkOrientationMarkerWidget> axesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();//���½���ά������

	//���������
	FormMesh* formMesh;
	FormPostprocessing* formPostprocessing;
	FormRun* formRun;
	FormGeometry* formGeometry;
	FormMeshImport* formMeshImport;

	//�в�ͼ��ر���
	QChart* chart;
	QValueAxis* axisX;
	QLogValueAxis* axisY;
	QMap<QString, QLineSeries*> seriesMap;
	QMap<QString, QPair<double, double>> seriesRangeMap;
	QTimer* chartUpdateTimer;
	double currentTimeStep;
	int axisMinX;
	int axisMaxX;
	double axisMinY;
	double axisMaxY;
};
