#include "pch.h"
#include "CUIContainer_bu.h"

#include "CMainPanel_bu.h"
#include "CInGamePanel_bu.h"

CUIContainer_bu::CUIContainer_bu() :
	CScript((UINT)SCRIPT_TYPE::UICONTAINER_BU),
	m_pMainPanelObj(nullptr),
	m_pInGamePanelObj(nullptr),
	m_pMainPanel(nullptr),
	m_pInGamePanel(nullptr)
{
	AddParam(TScriptParam{_T("Main Panel Obj"), E_ScriptParam::GAMEOBJ, &m_pMainPanelObj});
	AddParam(TScriptParam{ _T("InGame Panel Obj"), E_ScriptParam::GAMEOBJ, &m_pInGamePanelObj});
}
CUIContainer_bu::~CUIContainer_bu() {
}

void CUIContainer_bu::Awake()
{
	m_pMainPanel = m_pMainPanelObj->GetComponent<CMainPanel_bu>();
	m_pInGamePanel = m_pInGamePanelObj->GetComponent<CInGamePanel_bu>();
	assert(m_pMainPanel);
	assert(m_pInGamePanel);
}

void CUIContainer_bu::Update()
{
}

bool CUIContainer_bu::SaveToScene(FILE* _pFile)
{
	FWriteObj(m_pMainPanelObj, _pFile);
	FWriteObj(m_pInGamePanelObj, _pFile);

	return true;
}

bool CUIContainer_bu::LoadFromScene(FILE* _pFile)
{
	FReadObj(&m_pMainPanelObj, _pFile);
	FReadObj(&m_pInGamePanelObj, _pFile);
	return true;
}
