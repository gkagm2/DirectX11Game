#include "pch.h"
#include "CItemArmor_bu.h"
#include "CCharacter_bu.h"

tstring CItemArmor_bu::strDescription = _T("0 Lv1 Armor\n1 Lv2 Armor\n2 Lv3 Armor\n3 Lv4 Armor");

CItemArmor_bu::CItemArmor_bu() :
	CItem_bu((UINT)SCRIPT_TYPE::ITEMARMOR_BU),
	m_fArmor(0.f),
	m_eArmorType(E_ItemArmorType_bu::Armor1)
{
	AddParam(TScriptParam{ _T("Armor type"), E_ScriptParam::INT, &m_eArmorType });
	int iType = CMyMath::Clamp((int)m_eArmorType, 0, (int)E_ItemArmorType_bu::End);
	m_eArmorType = (E_ItemArmorType_bu)iType;
	
	AddParam(TScriptParam{ _T("Armor Description"), E_ScriptParam::STRING_PRINT, &strDescription });
}

CItemArmor_bu::CItemArmor_bu(const CItemArmor_bu& _origin) :
	CItem_bu((UINT)SCRIPT_TYPE::ITEMARMOR_BU),
	m_fArmor(_origin.m_fArmor),
	m_eArmorType(_origin.m_eArmorType)
{
	AddParam(TScriptParam{ _T("Armor type"), E_ScriptParam::INT, &m_eArmorType });
	int iType = CMyMath::Clamp((int)m_eArmorType, 0, (int)E_ItemArmorType_bu::End);
	m_eArmorType = (E_ItemArmorType_bu)iType;

	AddParam(TScriptParam{ _T("Armor Description"), E_ScriptParam::STRING_PRINT, &strDescription });
}

CItemArmor_bu::~CItemArmor_bu()
{
}

void CItemArmor_bu::ItemInitArmor()
{
	if (E_ItemArmorType_bu::Armor1 == m_eArmorType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemArmor1);
		m_fArmor = 15.f;
	}
	else if (E_ItemArmorType_bu::Armor2 == m_eArmorType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemArmor2);
		m_fArmor = 20.f;
	}
	else if (E_ItemArmorType_bu::Armor3 == m_eArmorType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemArmor3);
		m_fArmor = 25.f;
	}
	else if (E_ItemArmorType_bu::Armor4 == m_eArmorType) {
		m_pItemAnim->Play(BUTCHER_AnimName_ItemArmor4);
		m_fArmor = 30.f;
	}
	else
		assert(nullptr);
}

void CItemArmor_bu::ItemArmor()
{
	float fArmor = m_pTargetCharacter->GetArmor();
	fArmor += m_fArmor;
	float fMaxArmor = m_pTargetCharacter->GetMaxArmor();
	fArmor = min(fArmor, fMaxArmor);
	m_pTargetCharacter->SetArmor(fArmor);
	DestroyGameObjectEvn(GetGameObject());
}

void CItemArmor_bu::Start()
{
	_SetItemCallBack(&CItemArmor_bu::ItemInitArmor, &CItemArmor_bu::ItemArmor, this);
	InitItem();
}

bool CItemArmor_bu::SaveToScene(FILE* _pFile)
{
	FWrite(m_fArmor, _pFile);
	FWrite(m_eArmorType, _pFile);
	return true;
}

bool CItemArmor_bu::LoadFromScene(FILE* _pFile)
{
	FRead(m_fArmor, _pFile);
	FRead(m_eArmorType, _pFile);
	return true;
}