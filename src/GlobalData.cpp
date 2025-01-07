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
	clearAllData();
	m_caseData->casePath = "C:\\Data\\kaisihaopeng\\OpenFOAM-2406\\OF2406\\j20-s\\case.foam";
	//m_caseData->casePath = "C:\\Data\\kaisihaopeng\\OpenFOAM-2406\\OF2406\\j20_rhosimple_2\\case.foam";	
	//m_caseData->casePath = "C:\\Data\\kaisihaopeng\\OpenFOAM-2406\\OF2406\\j20_rhosimple_3\\case.foam";	
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

void GlobalData::clearAllData()
{
	m_caseData->geometryPath = "";
	m_caseData->meshPath = "";
	m_caseData->casePath = "";
	m_caseData->meshEdgeActors.clear();
	m_caseData->meshFaceActors.clear();
	m_caseData->timeFilePairs.clear();
}

