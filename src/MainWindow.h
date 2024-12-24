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

	void addCoordinateAxes();									    //添加左下角三维坐标轴
	void hideAllSubForm();										    //隐藏所有子窗口
	void parseOutput(const QString& output);						//解析输出
	void updateChart();											    //更新残差图

public slots:
	void handleAction1Triggered();								    //信息框
	void handleAction2Triggered();								    //x正向
	void handleAction3Triggered();								    //x负向
	void handleAction4Triggered();								    //y正向
	void handleAction5Triggered();								    //y负向
	void handleAction6Triggered();								    //z正向
	void handleAction7Triggered();								    //z负向
	void handleAction8Triggered();								    //适应窗口

	void on_pushButton_clicked();								    //几何页面
	void on_pushButton_4_clicked();								    //网格导入页面
	void on_pushButton_2_clicked();								    //网格页面
	void on_pushButton_16_clicked();								//求解计算页面
	void on_pushButton_17_clicked();								//后处理页面

	void formGeometry_import(const QString& filePath);				//几何导入
	void formMeshImport_import(const QString& filePath);			//网格导入
	void formMesh_apply(); 											//更新渲染窗口	
	void formRun_run();												//求解计算
	void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void formRun_stopRun();											//停止计算
	void formPostprocessing_loadData();								//加载结果数据		
	void formPostprocessing_apply();								//更新渲染窗口	
	void formPostprocessing_firstFrame();							//第一帧
	void formPostprocessing_previousFrame();						//上一帧
	void formPostprocessing_reverse();								//重新播放
	void formPostprocessing_play();									//播放
	void formPostprocessing_nextFrame();							//下一帧
	void formPostprocessing_lastFrame();							//最后一帧
	void formPostprocessing_loopPlay();								//循环播放
	void formPostprocessing_playPause();							//播放暂停
	void formPostprocessing_reversePause();							//反向播放暂停
	void formPostprocessing_loopPlayPause();						//循环播放暂停

	void onPlayTimerTimeout();										//播放
	void onReverseTimerTimeout(); 									//倒放
	void onLoopPlayTimerTimeout(); 									//循环播放

	void onButtonClicked();											//主控制面板按钮点击
	void onProcessOutput();											//进程输出
	void onProcessError();											//进程错误

public:
	Ui::MainWindowClass *ui;
	QTimer* playTimer;
	QTimer* reverseTimer;
	QTimer* loopPlayTimer;
	QPushButton* buttons[20];
	QPushButton* lastClickedButton;
	QProcess process;

	//渲染窗口
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderer> render;
	vtkSmartPointer<vtkOrientationMarkerWidget> axesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();//左下角三维坐标轴

	//副控制面板
	FormMesh* formMesh;
	FormPostprocessing* formPostprocessing;
	FormRun* formRun;
	FormGeometry* formGeometry;
	FormMeshImport* formMeshImport;

	//残差图相关变量
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
