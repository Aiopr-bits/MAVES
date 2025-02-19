#pragma once
#pragma execution_character_set("utf-8")
#include <QString>
#include <QMap>
#include <QVariant>
#include <vector>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <qstring.h>
#include <vtkLookupTable.h>
#include <array>
#include <vtkScalarBarActor.h>
using namespace std;

struct CaseData
{
    std::string casePath; 														                        // case·��
	std::string solverName;                                                                             //���������
	std::map<std::string, vtkSmartPointer<vtkActor>> meshPatchActors;					                //����patch actor
    std::map<QString, vtkSmartPointer<vtkActor>> meshEdgeActors;					                    //����߽�actor
    std::map<QString, vtkSmartPointer<vtkActor>> meshFaceActors;					                    //������actor

	QList<QPair<double, QStringList>> timeFilePairs; 													// ʱ��-VTK�ļ�·����
	QStringList fieldNames; 																			// ����������
    QMap<QString, vtkSmartPointer<vtkScalarBarActor>> scalarBars;                                       // ��������Ӧ��ͼ��
};

class GlobalData
{
public:
    static GlobalData& getInstance();                                                   // ��ȡ����ʵ��

    GlobalData(const GlobalData&) = delete;                                             // ��ֹ��������
    GlobalData& operator=(const GlobalData&) = delete;                                  // ��ֹ��ֵ����

    void setCaseData(const CaseData& data);                                             // ����ȫ������                                      
	CaseData* getCaseData() const;													    // ��ȡȫ������
	void clearAllData(); 															    // ���ȫ������

private:
    GlobalData();                                                                       // ˽�й��캯��                
    CaseData* m_caseData;                                                               // ȫ������                 
};

