#include "pch.h"
#include "CItem_bu.h"
#include "CPlayerController_bu.h"
#include "CCharacter_bu.h"
#include "CSoundManager_bu.h"

CItem_bu::CItem_bu() :
	CScript((UINT)SCRIPT_TYPE::ITEM_BU),
	m_bIsLifeTimeOn(false),
	m_fMaxLifeTime(10.f),
	m_fLifeTime(0.f),
	m_pItemAnimObj{ nullptr },
	m_pItemAnim{ nullptr },
	m_pTargetCharacter{ nullptr }
{
}

CItem_bu::CItem_bu(UINT _iScriptType) :
	CScript(_iScriptType),
	m_bIsLifeTimeOn(false),
	m_fMaxLifeTime(10.f),
	m_fLifeTime(0.f),
	m_pItemAnimObj{ nullptr },
	m_pItemAnim{ nullptr },
	m_pTargetCharacter{ nullptr }
{
}

CItem_bu::CItem_bu(const CItem_bu& _origin) :
	CScript((UINT)SCRIPT_TYPE::ITEM_BU),
	m_bIsLifeTimeOn(_origin.m_bIsLifeTimeOn),
	m_fMaxLifeTime(_origin.m_fMaxLifeTime),
	m_fLifeTime(0.f),
	m_pItemAnimObj{ nullptr },
	m_pItemAnim{ nullptr },
	m_pTargetCharacter{ nullptr }
{
}

CItem_bu::~CItem_bu()
{
}

void CItem_bu::Awake()
{
	m_pItemAnimObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_ItemAnim);
	assert(m_pItemAnimObj);
	m_pItemAnim = m_pItemAnimObj->Animator2D();
	assert(m_pItemAnim);
}

void CItem_bu::Update()
{
	if (m_bIsLifeTimeOn) {
		m_fLifeTime += DT;

		if (m_fLifeTime > m_fMaxLifeTime)
			CObject::DestroyGameObjectEvn(GetGameObject());
	}
}

void CItem_bu::OnCollisionEnter2D(CCollider2D* _pOther)
{
	UINT playerTag = (UINT)E_Tag::Player;
	UINT iLayer = (UINT)E_Layer::Object;
	if (iLayer == _pOther->GetGameObject()->GetLayer() &&
		playerTag == _pOther->GetGameObject()->GetTag()) {
		CPlayerController_bu* pctr = _pOther->GetGameObject()->GetComponent<CPlayerController_bu>();
		CCharacter_bu* pchar = _pOther->GetGameObject()->GetComponent<CCharacter_bu>();
		Interact(pchar);
		CSoundManager_bu* pSoundMgr = FIND_GameObject(_T("SoundManager"))->GetComponent<CSoundManager_bu>();
		if (pSoundMgr)
			pSoundMgr->m_pPickupItem->Play(1, 1, true);
		CObject::DestroyGameObjectEvn(GetGameObject());
	}
}