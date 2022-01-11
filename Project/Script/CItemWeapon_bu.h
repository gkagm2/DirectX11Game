#pragma once
#include "CItem_bu.h"
#include "CWeapon_bu.h"
class CItemWeapon_bu : public CItem_bu
{
private:
	E_WeaponType_bu m_eWeaponType;

private:
	void ItemInitWeapon();
	void ItemWeapon();

public:
	virtual void Start() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CItemWeapon_bu);
	CItemWeapon_bu();
	CItemWeapon_bu(const CItemWeapon_bu& _origin);
	virtual ~CItemWeapon_bu() override;
};