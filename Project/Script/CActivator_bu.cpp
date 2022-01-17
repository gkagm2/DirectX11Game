#include "pch.h"
#include "CActivator_bu.h"
#include "CActivatorController.h"

CActivator_bu::CActivator_bu() :
	CScript((UINT)SCRIPT_TYPE::ACTIVATOR_BU),
	m_bIsActivatorActive(false)
{
}

void CActivator_bu::Awake()
{
	CGameObject* pController = GetGameObject()->GetParentObject();
	assert(pController);
	m_pController =  pController->GetComponent<CActivatorController>();
	assert(m_pController);
}

void CActivator_bu::OnCollisionEnter2D(CCollider2D* _pOther)
{
	CGameObject* pObj = _pOther->GetGameObject();
	UINT iTag = (UINT)E_Tag::Player;
	if (iTag == pObj->GetTag())
		m_bIsActivatorActive = true;

	if (m_bIsActivatorActive) {
		m_pController->Activate();
		GetGameObject()->SetActive(false);
	}
}