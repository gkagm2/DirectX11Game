#include "pch.h"
#include "CManagerContainer_ca.h"
#include "CGameManager_ca.h"
#include "CObjectDragAndDropScript_ca.h"

CManagerContainer_ca::CManagerContainer_ca() :
	CScript((UINT)SCRIPT_TYPE::MANAGERCONTAINER_CA),
	m_pGameManager{nullptr},
	m_pObjDragAndDrop{ nullptr }
{
}

CManagerContainer_ca::~CManagerContainer_ca()
{
}

void CManagerContainer_ca::Awake() {
	m_pGameManager = (CGameManager_ca*)FIND_GameObject(_T("GameManager"))->GetComponent<CGameManager_ca>();
	m_pObjDragAndDrop = (CObjectDragAndDropScript_ca*)FIND_GameObject(_T("ObjectDragAndDrop"))->GetComponent<CObjectDragAndDropScript_ca>();

	assert(m_pGameManager);
	assert(m_pObjDragAndDrop);
}