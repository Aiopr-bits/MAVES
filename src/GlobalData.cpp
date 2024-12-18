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
	m_caseData->geometryPath = "";
	m_caseData->meshPath = "";
	m_caseData->casePath = "C:\\Data\\kaisihaopeng\\OpenFOAM-2406\\OF2406\\j20-s\\case.foam";	m_caseData->times = {};
	m_caseData->variableNames = {};
	m_caseData->meshPartName = {};
}

// ����ȫ������
void GlobalData::setCaseData(const caseData& data)
{

}

// ��ȡȫ������
caseData* GlobalData::getCaseData() const
{
    return m_caseData;
}

