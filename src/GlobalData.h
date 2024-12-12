#pragma once
#include <QString>
#include <QMap>
#include <QVariant>
#include <vector>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <qstring.h>

using namespace std;

struct caseData
{
	std::string geometryPath;                                                                          //����ļ����ļ�·��
	std::string meshPath;														                        //����������ļ�·��
	std::map<QString, vtkSmartPointer<vtkActor>> meshEdgeActors;					                    // ����߽�actor
	std::map<QString, vtkSmartPointer<vtkActor>> meshFaceActors;					                    // ������actor

    std::string casePath; 														                        // case·��
	std::map<double, std::map<QString, std::map<QString, vtkSmartPointer<vtkActor>>>> boundaryActors;  // �߽�actor
};

class GlobalData
{
public:
    static GlobalData& getInstance();                                                   // ��ȡ����ʵ��

    GlobalData(const GlobalData&) = delete;                                             // ��ֹ��������
    GlobalData& operator=(const GlobalData&) = delete;                                  // ��ֹ��ֵ����

    void setCaseData(const caseData& data);                                             // ����ȫ������                                      
    caseData* getCaseData() const;

private:
    GlobalData();                                                                       // ˽�й��캯��                
    caseData* m_caseData;                                                               // ȫ������                 
};

