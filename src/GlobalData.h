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
    static GlobalData& getInstance();                                                   // 获取单例实例

    GlobalData(const GlobalData&) = delete;                                             // 禁止拷贝构造
    GlobalData& operator=(const GlobalData&) = delete;                                  // 禁止赋值构造

    void setCaseData(const caseData& data);                                             // 设置全局数据                                      
    caseData* getCaseData() const;

private:
    GlobalData();                                                                       // 私有构造函数                
    caseData* m_caseData;                                                               // 全局数据                 
};

