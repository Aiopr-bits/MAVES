﻿/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

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
	std::vector<std::string> cellZoneNames; 															// zones名称
    std::unordered_map < std::string, std::string> regionsType;                                         // 区域类型
	std::unordered_map<std::string, unordered_map<std::string, std::string>> patchType;                 // 网格patch类型(subDomainName,patchName, patchType)
	std::vector<double> timeSteps; 																		// 时间步
	std::vector<std::string> fieldName; 																// 物理量名称
	std::map<std::string, std::pair<double, double>> fieldsScalarRange;                                 // 物理量范围(fieldName ，（rangeMin ，rangeMax）)
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

