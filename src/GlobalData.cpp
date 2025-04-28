/*--------------------------------*- C++ -*----------------------------------*\
| MAVES：Multi-source Adaptability Verification and Evaluation Software       |
| Author: Zhiyong Zeng                                                        |
| Date: December 01, 2024                                                     |
| Email: mailzengzhiyong@gamil.com                                            |
\*---------------------------------------------------------------------------*/

#include "GlobalData.h"

// 获取单例实例
GlobalData& GlobalData::getInstance()
{
    static GlobalData instance;
    return instance;
}

// 私有构造函数
GlobalData::GlobalData() {
    m_caseData = new CaseData();
	clearAllData();
}

// 设置全局数据
void GlobalData::setCaseData(const CaseData& data)
{

}

// 获取全局数据
CaseData* GlobalData::getCaseData() const
{
    return m_caseData;
}

void GlobalData::clearAllData()
{
	m_caseData->solverName = "";
	m_caseData->casePath = "";
}

