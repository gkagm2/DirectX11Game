#pragma once
#include "CItem_bu.h"
#include "CWeapon_bu.h"
class CItemWeapon_bu : public CItem_bu
{
private:
	E_WeaponType_bu m_eWeaponType;
	static tstring strDescription;

private:
	virtual void InitItem();
	virtual void Interact(CCharacter_bu* _pTargetCharacter);

public:
	virtual void Start() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

	void SetItemType(E_WeaponType_bu _eType) { m_eWeaponType = _eType; InitItem();
	}

public:
	CLONE(CItemWeapon_bu);
	CItemWeapon_bu();
	CItemWeapon_bu(const CItemWeapon_bu& _origin);
	virtual ~CItemWeapon_bu() override;
};