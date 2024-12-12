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

using namespace std;

struct caseData
{
    std::string geometryPath;                                                                          //����ļ����ļ�·��
    std::string meshPath;														                        //����������ļ�·��
    std::map<QString, vtkSmartPointer<vtkActor>> meshEdgeActors;					                    // ����߽�actor
    std::map<QString, vtkSmartPointer<vtkActor>> meshFaceActors;					                    // ������actor

    std::string casePath; 														                        // case·��
    std::vector<double> times;                                                                          // ʱ�䲽
    std::vector<QString> variableNames;                                                                 // ����������
    std::vector<QString> meshPartName;																	// ���񲿼�����

    vtkSmartPointer<vtkLookupTable> lookupTable;														// ��ɫӳ���
    std::array<double, 2> range;                                                                        // ��ɫӳ�䷶Χ
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

