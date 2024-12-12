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
	std::string geometryPath;                                                                          //导入的几何文件路径
	std::string meshPath;														                        //导入的网格文件路径
	std::map<QString, vtkSmartPointer<vtkActor>> meshEdgeActors;					                    // 网格边界actor
	std::map<QString, vtkSmartPointer<vtkActor>> meshFaceActors;					                    // 网格面actor

    std::string casePath; 														                        // case路径
	std::map<double, std::map<QString, std::map<QString, vtkSmartPointer<vtkActor>>>> boundaryActors;  // 边界actor
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

