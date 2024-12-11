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
    std::string* geometryPath;
    std::string* meshPath;
	std::map<QString, vtkSmartPointer<vtkActor>>* meshEdgeActors;
	std::map<QString, vtkSmartPointer<vtkActor>>* meshFaceActors;
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

