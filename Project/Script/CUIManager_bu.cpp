#include "pch.h"
#include "CUIManager_bu.h"
#include "CUIContainer_bu.h"
#include <Engine\CSceneManager.h>
#include <Engine\CScene.h>
#include <Engine\CGameObject.h>
#include <Engine\CComponent.h>
SINGLETON_SCRIPT_CPP(CUIManager_bu);

CUIManager_bu::CUIManager_bu() :
	CScript((UINT)SCRIPT_TYPE::UIMANAGER_BU),
	m_pContainer(nullptr)
{
}

CUIManager_bu::~CUIManager_bu() {

}

void CUIManager_bu::Awake()
{
	FIND_Component(m_pContainer, CUIContainer_bu);
	assert(m_pContainer);
}
