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

QT_BEGIN_NAMESPACE
namespace Ui { class FormPostprocessingClass; };
QT_END_NAMESPACE

class FormPostprocessing : public QWidget
{
	Q_OBJECT

public:
	FormPostprocessing(QWidget *parent = nullptr);
	~FormPostprocessing();

public slots:
	void on_pushButton_2_clicked();									//Ӧ�ð�ť
	void on_treeView_itemClicked(const QModelIndex& index);			//treeView����¼�
	void on_pushButton_3_clicked();									//��һ֡
	void on_pushButton_4_clicked();									//��һ֡
	void on_pushButton_5_clicked();									//���²���
	void on_pushButton_6_clicked();									//����
	void on_pushButton_7_clicked();									//��һ֡
	void on_pushButton_8_clicked();									//���һ֡
	void on_pushButton_9_clicked();									//ѭ������
	void on_pushButtonPlayTimerPause_clicked();						//������ͣ
	void on_pushButtonReverseTimerPause_clicked();					//���򲥷���ͣ
	void on_pushButtonLoopPlayTimerPause_clicked();					//ѭ��������ͣ


signals:
	void resultDataLoaded(const QString& caseFilePath);
	void apply();
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

public:
	Ui::FormPostprocessingClass *ui;
	QStandardItemModel* listViewModel;

	CustomHoverPushButton* pushButtonPlayTimerPause;
	CustomHoverPushButton* pushButtonReverseTimerPause;
	CustomHoverPushButton* pushButtonLoopPlayTimerPause;
};
