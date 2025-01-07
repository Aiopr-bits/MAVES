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
	clearAllData();
	m_caseData->casePath = "C:\\Data\\kaisihaopeng\\OpenFOAM-2406\\OF2406\\j20-s\\case.foam";
	//m_caseData->casePath = "C:\\Data\\kaisihaopeng\\OpenFOAM-2406\\OF2406\\j20_rhosimple_2\\case.foam";	
	//m_caseData->casePath = "C:\\Data\\kaisihaopeng\\OpenFOAM-2406\\OF2406\\j20_rhosimple_3\\case.foam";	
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

void GlobalData::clearAllData()
{
	m_caseData->geometryPath = "";
	m_caseData->meshPath = "";
	m_caseData->casePath = "";
	m_caseData->meshEdgeActors.clear();
	m_caseData->meshFaceActors.clear();
	m_caseData->timeFilePairs.clear();
}

