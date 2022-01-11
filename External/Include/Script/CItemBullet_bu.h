#pragma once
#include "CItem_bu.h"
#include "CWeapon_bu.h"
class CItemBullet_bu : public CItem_bu
{
private:
	E_WeaponType_bu m_eWeaponType;

	static tstring strDescription;
private:
	void ItemInitBullet();
	void ItemBullet();

public:	
	virtual void Start() override;
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CItemBullet_bu);
	CItemBullet_bu();
	CItemBullet_bu(const CItemBullet_bu& _origin);
	virtual ~CItemBullet_bu() override;
};