#pragma once
#include "CItem_bu.h"
#include "CWeapon_bu.h"
class CItemBullet_bu : public CItem_bu
{
private:
	E_WeaponType_bu m_eWeaponType;

	static tstring strDescription;
private:
	virtual void InitItem() override;
	virtual void Interact(CCharacter_bu* _pTargetCharacter) override;

public:	
	virtual void Start() override;
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

	void SetItemType(E_WeaponType_bu _eType) { m_eWeaponType = _eType; InitItem(); }

public:
	CLONE(CItemBullet_bu);
	CItemBullet_bu();
	CItemBullet_bu(const CItemBullet_bu& _origin);
	virtual ~CItemBullet_bu() override;
};