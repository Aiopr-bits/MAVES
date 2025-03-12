/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#pragma once
#pragma execution_character_set("utf-8")
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
#include "CustomHoverPushButton.h"
#include "QListView.h"
#include "CustomCheckBoxDelegate.h"
#include <vtkCompositeDataGeometryFilter.h>
#include <vtkOpenFOAMReader.h>
#include <sstream>
#include <QPropertyAnimation>

QT_BEGIN_NAMESPACE
namespace Ui { class FormPostprocessingClass; };
QT_END_NAMESPACE

class FormPostprocessing : public QWidget
{
	Q_OBJECT

public:
	FormPostprocessing(QWidget *parent = nullptr);
	~FormPostprocessing();

	void split(const std::string& s,								//字符串分割
		char delimiter, 
		std::vector<std::string>& tokens);

	std::unordered_map<std::string, std::vector<std::string>>		//解析网格patch名称
		analysismeshPatchNames
		(const std::vector<std::string>& meshPatchNames);

	QListView* createBoundariesListView(std::string regionName,		//创建边界listView
		std::vector<std::string> patchNames);

	void getMeshPatchData(const std::string& casePath); 			//获取网格patch数据
	void getNephogramPatchData(const std::string& casePath);		//获取云图patch数据
	void updateForm();												//更新界面listView

public slots:
	void on_pushButton_2_clicked();									//应用按钮
	void on_pushButton_3_clicked();									//第一帧
	void on_pushButton_4_clicked();									//上一帧
	void on_pushButton_5_clicked();									//重新播放
	void on_pushButton_6_clicked();									//播放
	void on_pushButton_7_clicked();									//下一帧
	void on_pushButton_8_clicked();									//最后一帧
	void on_pushButton_9_clicked();									//循环播放
	void on_pushButton_10_clicked();								//加载数据
	void on_pushButtonPlayTimerPause_clicked();						//播放暂停
	void on_pushButtonReverseTimerPause_clicked();					//反向播放
	void on_pushButtonLoopPlayTimerPause_clicked();					//循环播放

	void onListViewClicked(const QModelIndex& index); 				//listView点击事件
	void onToggleRegionSecondAnimation();							//切换区域第二动画	

signals:
	void loadData();
	void apply(std::vector<QListView*> listViewBoundaries);
	void firstFrame();
	void previousFrame();
	void reverse();
	void play();
	void nextFrame();
	void lastFrame();
	void loopPlay();
	void playPause();
	void reversePause();
	void loopPlayPause();
	void toggleRegionSecondAnimation();
	void updateFormFinished();

public:
	Ui::FormPostprocessingClass *ui;
	QStandardItemModel* listViewModel;
	QModelIndex lastIndex;
	std::vector<QListView*> listViewBoundaries;

	CustomHoverPushButton* pushButtonPlayTimerPause;
	CustomHoverPushButton* pushButtonReverseTimerPause;
	CustomHoverPushButton* pushButtonLoopPlayTimerPause;
};
