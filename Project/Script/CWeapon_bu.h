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
tstring WeaponTypeToStr_bu(E_WeaponType_bu _eType);

struct TWeaponInfo_bu {
	int iMaxBullet;
	int iCurBullet;

	int iGetBulletCnt;  // 아이템을 얻을 경우 획득할 양
	float fRpm; // 분당 발사 속도
	bool bInfinity;		// 무한성
};


class CWeapon_bu : public CScript
{
private:
	class CCharacter_bu* m_pCharacter;

	TWeaponInfo_bu m_tWeaponInfo[(UINT)E_WeaponType_bu::End];
	bool m_arrWeaponUse[(UINT)E_WeaponType_bu::End];
	E_WeaponType_bu m_eCurType;

	CGameObject* m_pChainSawObj;
	CGameObject* m_pGunImageObj;
	tstring m_strCurWeaponName;

	float m_fFireTime;
	float m_fMaxFireTime;
	float m_fReloadDelayTime;

	bool m_bIsEnableFire;


	SharedPtr<CPrefab> m_pMainBullet; // 출력할 총알

	// 전기톱같은경우 처리
	CGameObject* m_pChainSawColObj;
	// 샷건 총알
	SharedPtr<CPrefab> m_pShotgunBullet;
	// 머신건 총알
	SharedPtr<CPrefab> m_pMachinegunBullet;
	// 파이어 총알
	SharedPtr<CPrefab> m_pFlameBullet;
	// 레이저 총알
	//SharedPtr<CPrefab> m_pLaserBullet;
	// 유탄 총알
	//SharedPtr<CPrefab> m_GranadeBullet;

public:
	virtual void Start() override;
	virtual void Update() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	void AddWeaponBulletItem(E_WeaponType_bu _eType); // 아이템을 먹을경우 이 함수 실행
	void ChangeWeapon(E_WeaponType_bu _eType);

private:
	void _InitWeaponInfo();

public:
	E_WeaponType_bu GetCurWeaponType() { return m_eCurType; }
	TWeaponInfo_bu* GetAllWeaponInfo() { return m_tWeaponInfo; }
	TWeaponInfo_bu& GetCurWeapon() { return m_tWeaponInfo[(UINT)m_eCurType]; }

	bool IsEnableFire() { return m_bIsEnableFire; }
	// 발사 위치, 로컬 위치의 발사 방향, 회전각도. return : 발사 성공 여부
	bool Fire(const Vector3& _vMuzzlePos, const Vector3& _vRot, const Vector3& _vShootDir, UINT _iTag);
	void SetUseableWeapon(E_WeaponType_bu _eType, bool _bUse){ m_arrWeaponUse[(UINT)_eType] = _bUse; }
	bool IsUseableWeapon(E_WeaponType_bu _eType) { return m_arrWeaponUse[(UINT)_eType]; }

public:
	CLONE(CWeapon_bu);
	CWeapon_bu();
	CWeapon_bu(const CWeapon_bu& _origin);
	virtual ~CWeapon_bu() override;
};