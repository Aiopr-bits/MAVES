#include "GlobalData.h"

// ��ȡ����ʵ��
GlobalData& GlobalData::getInstance()
{
    static GlobalData instance;
    return instance;
}

// ˽�й��캯��
GlobalData::GlobalData() {
    m_caseData = new CaseData();
	clearAllData();
}

// ����ȫ������
void GlobalData::setCaseData(const CaseData& data)
{

}

// ��ȡȫ������
CaseData* GlobalData::getCaseData() const
{
    return m_caseData;
}

void GlobalData::clearAllData()
{
	m_caseData->solverName = "";
	m_caseData->casePath = "";
	m_caseData->timeFilePairs.clear();
}

