#include "pch.h"
#include "CItemHp_bu.h"
#include "CCharacter_bu.h"

CItemHp_bu::CItemHp_bu() :
	CItem_bu((UINT)SCRIPT_TYPE::ITEMHP_BU),
	m_fHp(0.f),
	m_eHpType(E_ItemHpType_bu::Hp1)
{
	AddParam(TScriptParam{ _T("HP type"), E_ScriptParam::INT, &m_eHpType });
	int iType = CMyMath::Clamp((int)m_eHpType, 0, (int)E_ItemHpType_bu::End);
	m_eHpType = (E_ItemHpType_bu)iType;
	ItemInitHp();
}

CItemHp_bu::CItemHp_bu(const CItemHp_bu& _origin):
	CItem_bu((UINT)SCRIPT_TYPE::ITEMHP_BU),
	m_fHp(_origin.m_fHp),
	m_eHpType(_origin.m_eHpType)
{
	AddParam(TScriptParam{ _T("HP type"), E_ScriptParam::INT, &m_eHpType });
	int iType = CMyMath::Clamp((int)m_eHpType, 0, (int)E_ItemHpType_bu::End);
	m_eHpType = (E_ItemHpType_bu)iType;
	ItemInitHp();
}

CItemHp_bu::~CItemHp_bu()
{
}

void CItemHp_bu::Start()
{
	_SetItemCallBack(&CItemHp_bu::ItemInitHp, &CItemHp_bu::ItemHp, this);
	InitItem();
}

void CItemHp_bu::ItemInitHp()
{
	if (E_ItemHpType_bu::Hp1 == m_eHpType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemHp1);
		m_fHp = 15.f;
	}
	else if (E_ItemHpType_bu::Hp2 == m_eHpType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemHp2);
		m_fHp = 20.f;
	}
	else if (E_ItemHpType_bu::Hp3 == m_eHpType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemHp3);
		m_fHp = 25.f;
	}
	else if (E_ItemHpType_bu::Hp4 == m_eHpType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemHp4);
		m_fHp = 30.f;
	}
	else
		assert(nullptr);
}

void CItemHp_bu::ItemHp()
{
	// Sound Ãâ·Â
	float fHp = m_pTargetCharacter->GetHp();
	fHp += m_fHp;
	float fMaxHp = m_pTargetCharacter->GetMaxHp();
	fHp = min(fHp, fMaxHp);
	m_pTargetCharacter->SetHp(fHp);
	DestroyGameObjectEvn(GetGameObject());
}

bool CItemHp_bu::SaveToScene(FILE* _pFile)
{
	FWrite(m_fHp, _pFile);
	FWrite(m_eHpType, _pFile);
	return true;
}

bool CItemHp_bu::LoadFromScene(FILE* _pFile)
{
	FRead(m_fHp, _pFile);
	FRead(m_eHpType, _pFile);
	return true;
}