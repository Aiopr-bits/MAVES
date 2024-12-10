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
    m_caseData->geometryPath = new std::string();
    m_caseData->meshPath = new std::string();
    m_caseData->meshBoundaryActors = new std::map<QString, vtkSmartPointer<vtkActor>>();
}

// 设置全局数据
void GlobalData::setCaseData(const caseData& data)
{
    *m_caseData->geometryPath = *data.geometryPath;
    *m_caseData->meshPath = *data.meshPath;
    *m_caseData->meshBoundaryActors = *data.meshBoundaryActors;
}

// 获取全局数据
caseData* GlobalData::getCaseData() const
{
    return m_caseData;
}

