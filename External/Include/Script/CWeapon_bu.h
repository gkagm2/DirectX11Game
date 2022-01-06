#pragma once
#include <Engine\CScript.h>

enum class E_WeaponType_bu {
	Chain,
	Shotgun,
	MachineGun,
	GrenadeLauncher,
	FlameThrower,
	LaserGun,
	End
};

class CWeapon_bu : public CScript
{
private:
	E_WeaponType_bu m_eWeaponType;

public:
	virtual void Start() override;
	virtual void Update() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	E_WeaponType_bu GetWeaponType() { return m_eWeaponType; }
	void SetWeaponType(E_WeaponType_bu _eType) { m_eWeaponType = _eType; }

public:
	CLONE(CWeapon_bu);
	CWeapon_bu();
	virtual ~CWeapon_bu() override;
};