#include "GlobalData.h"

// ��ȡ����ʵ��
GlobalData& GlobalData::getInstance()
{
    static GlobalData instance;
    return instance;
}

// ˽�й��캯��
GlobalData::GlobalData() {
    m_caseData = new caseData();
}

// ����ȫ������
void GlobalData::setCaseData(const caseData& data)
{
    m_caseData->geometryPath = data.geometryPath;
    m_caseData->meshPath = data.meshPath;
    m_caseData->meshFaceActors = data.meshFaceActors;
	m_caseData->meshEdgeActors = data.meshEdgeActors;
	m_caseData->casePath = data.casePath;
	m_caseData->boundaryActors = data.boundaryActors;
}

// ��ȡȫ������
caseData* GlobalData::getCaseData() const
{
    return m_caseData;
}

