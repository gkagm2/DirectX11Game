#include "pch.h"
#include "CConfigurationManager.h"
#include "CPathManager.h"

CConfigurationManager::CConfigurationManager() :
	m_strLayerNames{},
	m_strFilter(_T(".txt")),

	m_strLayerFileName(_T("Layer.txt"))
{
	m_strRelConfigPath = STR_DIR_PATH_Config;
	m_strAbsConfigPath = CPathManager::GetInstance()->GetContentPath() + tstring(STR_DIR_PATH_Config);

	// DevMode
	SetFormatMode(CConfigurationManager::E_FormatMode::txt);
}
CConfigurationManager::~CConfigurationManager()
{

}

void CConfigurationManager::Init()
{
	// 설정된 환경 값 불러오기
	LoadLayer();
}

void CConfigurationManager::Update()
{
}

void CConfigurationManager::Save()
{
}

void CConfigurationManager::Load()
{
}

void CConfigurationManager::SaveLayer()
{
	/*FILE* pFile;
	errno_t err;*/
	//err = _tfopen_s(&pFile, m_strLayerFileName.c_str(), _T("")
}

void CConfigurationManager::LoadLayer()
{
}

void CConfigurationManager::SetFormatMode(E_FormatMode _eFormat)
{
	m_eCurFormatMode = _eFormat;
	switch (m_eCurFormatMode) {
	case E_FormatMode::binary:
		m_strFormatRead = _T("rb");
		m_strFormatWrite = _T("wb");
		break;
	case E_FormatMode::txt:
		m_strFormatRead = _T("r");
		m_strFormatWrite = _T("w");
		break;
	default:
		assert(nullptr);
		break;
	}
}