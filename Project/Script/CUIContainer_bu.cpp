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
	AddParam(TScriptParam{_T("Main Panel Obj"), E_ScriptParam::GAMEOBJ, (void**)&m_pMainPanelObj});
	AddParam(TScriptParam{ _T("InGame Panel Obj"), E_ScriptParam::GAMEOBJ, (void**)&m_pInGamePanelObj});
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
	FWriteLinkObj(m_pMainPanelObj, _pFile);
	FWriteLinkObj(m_pInGamePanelObj, _pFile);

	return true;
}

bool CUIContainer_bu::LoadFromScene(FILE* _pFile)
{
	FReadLinkObj((CObject**)&m_pMainPanelObj, _pFile);
	FReadLinkObj((CObject**)&m_pInGamePanelObj, _pFile);
	return true;
}
