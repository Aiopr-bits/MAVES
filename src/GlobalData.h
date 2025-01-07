#pragma once
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

struct caseData
{
    std::string geometryPath;                                                                           //����ļ����ļ�·��
    std::string meshPath;														                        //����������ļ�·��
    std::map<QString, vtkSmartPointer<vtkActor>> meshEdgeActors;					                    // ����߽�actor
    std::map<QString, vtkSmartPointer<vtkActor>> meshFaceActors;					                    // ������actor

    std::string casePath; 														                        // case·��
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

    void setCaseData(const caseData& data);                                             // ����ȫ������                                      
	caseData* getCaseData() const;													    // ��ȡȫ������
	void clearAllData(); 															    // ���ȫ������

private:
    GlobalData();                                                                       // ˽�й��캯��                
    caseData* m_caseData;                                                               // ȫ������                 
};

