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
    std::string casePath; 														                        // case路径
	std::string solverName;                                                                             //求解器名称
	std::map<std::string, vtkSmartPointer<vtkActor>> meshPatchActors;					                //网格patch actor
    std::map<QString, vtkSmartPointer<vtkActor>> meshEdgeActors;					                    //网格边界actor
    std::map<QString, vtkSmartPointer<vtkActor>> meshFaceActors;					                    //网格面actor

	QList<QPair<double, QStringList>> timeFilePairs; 													// 时间-VTK文件路径对
	QStringList fieldNames; 																			// 物理量名称
    QMap<QString, vtkSmartPointer<vtkScalarBarActor>> scalarBars;                                       // 物理量对应的图例
};

class GlobalData
{
public:
    static GlobalData& getInstance();                                                   // 获取单例实例

    GlobalData(const GlobalData&) = delete;                                             // 禁止拷贝构造
    GlobalData& operator=(const GlobalData&) = delete;                                  // 禁止赋值构造

    void setCaseData(const CaseData& data);                                             // 设置全局数据                                      
	CaseData* getCaseData() const;													    // 获取全局数据
	void clearAllData(); 															    // 清空全局数据

private:
    GlobalData();                                                                       // 私有构造函数                
    CaseData* m_caseData;                                                               // 全局数据                 
};

