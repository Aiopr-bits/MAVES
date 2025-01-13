#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_FormModelClip.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FormModelClipClass; };
QT_END_NAMESPACE

class FormModelClip : public QWidget
{
	Q_OBJECT

public:
	FormModelClip(QWidget *parent = nullptr);
	~FormModelClip();

public slots:
	void onLineEditChanged();
	void on_pushButton_2_clicked();							//X����
	void on_pushButton_3_clicked();							//Y����
	void on_pushButton_4_clicked();							//Z����
	void on_pushButton_5_clicked();							//�������
	void on_pushButton_6_clicked();							//�����ӽ�
	void on_pushButton_7_clicked();							//����ƽ��
	void on_pushButton_clicked();							//Ӧ��

signals:
	void checkBoxToggled(bool checked);
	void lineEditsChanged();
	void xPositive();										//X����
	void yPositive();										//Y����
	void zPositive();										//Z����
	void cameraDirection();									//�������
	void alignView();										//�����ӽ�
	void resetPlane();										//����ƽ��
	void apply();											//Ӧ��




public:
	Ui::FormModelClipClass *ui;
};
