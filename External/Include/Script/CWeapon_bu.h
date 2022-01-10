#pragma once
#include <Engine\CScript.h>

enum class E_WeaponType_bu {
	Chainsaw,
	Shotgun,
	MachineGun,
	FlameThrower,
	GrenadeLauncher,
	LaserGun,
	End
};

struct TWeaponInfo_bu {
	int iMaxBullet;
	int iCurBullet;

	int iMaxMagazine;
	int iCurMagazineBullet;

	int iGetBulletCnt;  // 아이템을 얻을 경우 획득할 양
	bool iInfinity;		// 무한성
};

class CWeapon_bu : public CScript
{
private:
	TWeaponInfo_bu m_tWeaponInfo[(UINT)E_WeaponType_bu::End];
	E_WeaponType_bu m_eCurType;

	CGameObject* m_pChainSawObj;
	CGameObject* m_pGunImageObj;


public:
	virtual void Start() override;
	virtual void Update() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	void AddWeaponItem(E_WeaponType_bu _eType); // 아이템을 먹을경우 이 함수 실행
	void ChangeWeapon(E_WeaponType_bu _eType);
	void Reload();

private:
	void _InitWeaponInfo();

public:
	E_WeaponType_bu GetCurWeaponType() { return m_eCurType; }
	TWeaponInfo_bu* GetAllWeaponInfo() { return m_tWeaponInfo; }
	TWeaponInfo_bu& GetCurWeapon() { return m_tWeaponInfo[(UINT)m_eCurType]; }

public:
	CLONE(CWeapon_bu);
	CWeapon_bu();
	CWeapon_bu(const CWeapon_bu& _origin);
	virtual ~CWeapon_bu() override;
};