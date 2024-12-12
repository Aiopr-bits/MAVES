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
}

// 设置全局数据
void GlobalData::setCaseData(const caseData& data)
{
    m_caseData->geometryPath = data.geometryPath;
    m_caseData->meshPath = data.meshPath;
    m_caseData->meshFaceActors = data.meshFaceActors;
	m_caseData->meshEdgeActors = data.meshEdgeActors;
	m_caseData->casePath = data.casePath;
	m_caseData->boundaryActors = data.boundaryActors;
}

// 获取全局数据
caseData* GlobalData::getCaseData() const
{
    return m_caseData;
}

