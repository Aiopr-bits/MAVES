#include "GlobalData.h"

// 获取单例实例
GlobalData& GlobalData::getInstance()
{
    static GlobalData instance;
    return instance;
}

// 私有构造函数
GlobalData::GlobalData() {
    m_caseData = new caseData();
	m_caseData->geometryPath = "";
	m_caseData->meshPath = "";
	m_caseData->casePath = "C:\\Data\\kaisihaopeng\\OpenFOAM-2406\\OF2406\\j20-s\\case.foam";	m_caseData->times = {};
	m_caseData->variableNames = {};
	m_caseData->meshPartName = {};
}

// 设置全局数据
void GlobalData::setCaseData(const caseData& data)
{

}

// 获取全局数据
caseData* GlobalData::getCaseData() const
{
    return m_caseData;
}

