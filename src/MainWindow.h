/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#pragma execution_character_set("utf-8")
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
#include "FormBoundaryConditions.h"
#include "FormPhysicalPropertyParameter.h"
#include "FormTurbulence.h"
#include "FormSolver.h"
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
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <vtkCompositeDataGeometryFilter.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkInformation.h>
#include "DialogResultMerge.h"
#include <vtkAppendFilter.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkTableBasedClipDataSet.h>
#include <vtkAppendPolyData.h>
#include <Qpixmap>
#include <QGraphicsOpacityEffect>
#include "initialize.h"

#ifdef _WIN32
#include "qt_windows.h"
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void addCoordinateAxes();									    													//添加左下角三维坐标轴
	void hideAllSubForm();										    													//隐藏所有子窗口
	void parseOutput(const QString& output);																			//解析输出

	std::vector<vtkSmartPointer<vtkActor>> createMeshPatchActor( 														//创建网格patch actor
		const std::string& casePath,
		std::vector<std::string>& patchGroup);

	std::vector<vtkSmartPointer<vtkActor>> createNephogramPatchActor( 													//创建云图patch actor
		const std::string& casePath,
		double timeValue,
		const std::string& fieldName,
		std::vector<std::string>& patchGroup,
		const std::pair<double, double>& globalRange);

	vtkSmartPointer<vtkScalarBarActor> createScalarBarActor(const std::pair<double, double>& range); 					//创建图例actor

	std::vector<vtkSmartPointer<vtkActor>> createSlicedActorFromRenderer												//创建切分actor
		(double origin[3], double normal[3], bool keepInside);		

public slots:
	//工具栏信号处理
	void handleAction1Triggered();								    													//信息框
	void handleAction2Triggered();								    													//x正向
	void handleAction3Triggered();								    													//x负向
	void handleAction4Triggered();								    													//y正向
	void handleAction5Triggered();								    													//y负向
	void handleAction6Triggered();								    													//z正向
	void handleAction7Triggered();								    													//z负向
	void handleAction8Triggered();								    													//适应窗口
	void handleAction9Triggered();								    													//模型切分
	void handleAction10Triggered();								    													//导入案例

	//控制面板按钮点击事件处理
	void on_panelPushButton_clicked(string text);								    									//控制面板按钮点击
	void on_pushButton_clicked();								    													//几何页面
	void on_pushButton_4_clicked();								    													//网格导入页面
	void on_pushButton_2_clicked();								   														//网格页面
	void on_pushButton_5_clicked();								    													//求解器页面
	void on_pushButton_6_clicked();								    													//湍流模型页面
	void on_pushButton_7_clicked();								    													//物理参数页面
	void on_pushButton_13_clicked();								    												//边界条件页面
	void on_pushButton_16_clicked();																					//求解计算页面
	void on_pushButton_17_clicked();																					//后处理页面
	void on_pushButton_3_clicked();								    													//模型切分

	//主界面其他事件处理
	void onPlayTimerTimeout();																							//播放
	void onReverseTimerTimeout(); 																						//倒放
	void onLoopPlayTimerTimeout(); 																						//循环播放
	void onButtonClicked();																								//控制面板按钮点击背景色调整
	void onProcessRunFinished(int exitCode, QProcess::ExitStatus exitStatus);											//求解计算进程结束
	void onProcessDecomposeParFinished(int exitCode, QProcess::ExitStatus exitStatus);									//分解网格进程结束
	void onProcessReconstructParFinished(int exitCode, QProcess::ExitStatus exitStatus);								//重构网格进程结束
	void onProcessRunOutput();																							//求解计算进程输出
	void onProcessDecomposeParOutput();																					//分解网格进程输出
	void onProcessReconstructParOutput();																				//重构网格进程输出
	void updateChart();											    													//更新残差图
	void updatePlaneRepModelClipValues();								    											//更新模型切分平面选择器的值
	void resizeEvent(QResizeEvent* event);																				//窗口大小改变事件
	void on_tabWidget_currentChanged(int index);																		//tabWidget切换事件

	//副控制面板事件处理
	void formGeometry_import(const QString& filePath);																	//几何导入
	void formMeshImport_import(const QString& filePath, bool isRender);													//网格导入
	void formMesh_apply(std::vector<QListView*> listViewBoundaries); 													//更新渲染窗口	
	void formMesh_itemEntered(const QString& text);																		//网格页面Item进入
	void formMesh_itemExited(const QString& text);																		//网格页面Item退出
	void formMesh_updateFormFinished();																					//更新界面完成
	void formRun_run();																									//求解计算
	void formRun_stopRun();																								//停止计算
	void formPostprocessing_apply(std::vector<QListView*> listViewBoundaries);											//应用
	void formPostprocessing_firstFrame();																				//第一帧
	void formPostprocessing_previousFrame();																			//上一帧
	void formPostprocessing_reverse();																					//重新播放
	void formPostprocessing_play();																						//播放
	void formPostprocessing_nextFrame();																				//下一帧
	void formPostprocessing_lastFrame();																				//最后一帧
	void formPostprocessing_loopPlay();																					//循环播放
	void formPostprocessing_playPause();																				//播放暂停
	void formPostprocessing_reversePause();																				//反向播放暂停
	void formPostprocessing_loopPlayPause();																			//循环播放暂停
	void formPostprocessing_loadData();																					//加载数据
	void formPostprocessing_updateFormFinished();																		//更新界面完成
	void formModelClip_checkBoxToggle();																				//模型切分页面CheckBox切换
	void formModelClip_lineEditsChanged();																				//模型切分页面LineEdit值改变
	void formModelClip_xPositive();																						//模型切分：X正向
	void formModelClip_yPositive();																						//模型切分：Y正向
	void formModelClip_zPositive();																						//模型切分：Z正向
	void formModelClip_cameraDirection();																				//模型切分：相机方向
	void formModelClip_alignView();																						//模型切分：对齐视角
	void formModelClip_resetPlane();																					//模型切分：重置平面
	void formModelClip_apply();																							//模型切分：应用
	void dialogResultMerge_interrupt();																					//结果合并弹窗:中断

signals:
	void panelPushButtonClicked(string text);																			//控制面板按钮点击

public:
	Ui::MainWindowClass *ui;
	QTimer* playTimer;
	QTimer* reverseTimer;
	QTimer* loopPlayTimer;
	QPushButton* buttons[20];
	QPushButton* lastClickedButton;
	QProcess processRun;
	QProcess processDecomposePar;
	QProcess processReconstructPar;

	//渲染窗口
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderer> render;
	vtkSmartPointer<vtkOrientationMarkerWidget> axesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();		//左下角三维坐标轴

	//副控制面板
	FormGeometry* formGeometry;
	FormMesh* formMesh;
	FormMeshImport* formMeshImport;
	FormSolver* formSolver;
	FormTurbulence* formTurbulence;
	FormPhysicalPropertyParameter* formPhysicalPropertyParameter;
	FormBoundaryConditions* formBoundaryConditions;
	FormRun* formRun;
	FormPostprocessing* formPostprocessing;
	FormModelClip* formModelClip;

	//弹出窗口
	DialogResultMerge* dialogResultMerge;

	//残差图相关变量
	QChart* chart;
	QValueAxis* axisX;
	QLogValueAxis* axisY;
	QMap<QString, QLineSeries*> seriesMap;
	QMap<QString, QPair<double, double>> seriesRangeMap;
	QTimer* chartUpdateTimer;
	QMap<QString, double> residuals;																				// 用于存储当前时间步的残差数据
	int currentTimeStep;
	int axisMinX;
	int axisMaxX;
	double axisMinY;
	double axisMaxY;

	//平面选择器相关变量
	vtkSmartPointer<vtkImplicitPlaneWidget2> planeWidgetModelClip;
	vtkSmartPointer<vtkImplicitPlaneRepresentation> planeRepModelClip;

	//全局变量
	int nWriteResults;
	int previousTabWidgetIndex;
	string previousPanelButton;
};
