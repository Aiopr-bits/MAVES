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
    m_caseData->geometryPath = new std::string();
    m_caseData->meshPath = new std::string();
    m_caseData->meshBoundaryActors = new std::map<QString, vtkSmartPointer<vtkActor>>();
}

// ����ȫ������
void GlobalData::setCaseData(const caseData& data)
{
    *m_caseData->geometryPath = *data.geometryPath;
    *m_caseData->meshPath = *data.meshPath;
    *m_caseData->meshBoundaryActors = *data.meshBoundaryActors;
}

// ��ȡȫ������
caseData* GlobalData::getCaseData() const
{
    return m_caseData;
}

