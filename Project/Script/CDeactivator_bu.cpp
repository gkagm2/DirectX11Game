#include "pch.h"
#include "CDeactivator_bu.h"
#include "CActivatorController.h"

CDeactivator_bu::CDeactivator_bu() :
	CScript((UINT)SCRIPT_TYPE::DEACTIVATOR_BU)
{
}

void CDeactivator_bu::Awake()
{
	CGameObject* pController = GetGameObject()->GetParentObject();
	assert(pController);
	m_pController = pController->GetComponent<CActivatorController>();
	assert(m_pController);
}

void CDeactivator_bu::OnCollisionEnter2D(CCollider2D* _pOther)
{
	CGameObject* pObj = _pOther->GetGameObject();
	UINT iTag = (UINT)E_Tag::Player;
	if (iTag == pObj->GetTag())
		m_bIsDeactivatorActive = true;

	if (m_bIsDeactivatorActive) {
		m_pController->Deactivate();
		GetGameObject()->SetActive(false);
	}
}