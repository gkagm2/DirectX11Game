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

	int iGetBulletCnt;  // �������� ���� ��� ȹ���� ��
	float fRpm; // �д� �߻� �ӵ�
	bool bInfinity;		// ���Ѽ�
};

class CWeapon_bu : public CScript
{
private:
	TWeaponInfo_bu m_tWeaponInfo[(UINT)E_WeaponType_bu::End];
	bool m_arrWeaponUse[(UINT)E_WeaponType_bu::End];
	E_WeaponType_bu m_eCurType;

	CGameObject* m_pChainSawObj;
	CGameObject* m_pGunImageObj;

	float m_fFireTime;
	float m_fMaxFireTime;
	float m_fReloadDelayTime;

	bool m_bIsEnableFire;

public:
	virtual void Start() override;
	virtual void Update() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	void AddWeaponBulletItem(E_WeaponType_bu _eType); // �������� ������� �� �Լ� ����
	void ChangeWeapon(E_WeaponType_bu _eType);

private:
	void _InitWeaponInfo();

public:
	E_WeaponType_bu GetCurWeaponType() { return m_eCurType; }
	TWeaponInfo_bu* GetAllWeaponInfo() { return m_tWeaponInfo; }
	TWeaponInfo_bu& GetCurWeapon() { return m_tWeaponInfo[(UINT)m_eCurType]; }

	bool IsEnableFire() { return m_bIsEnableFire; }
	void Fire();
	void SetUseableWeapon(E_WeaponType_bu _eType, bool _bUse){ m_arrWeaponUse[(UINT)_eType] = _bUse; }
	bool IsUseableWeapon(E_WeaponType_bu _eType) { return m_arrWeaponUse[(UINT)_eType]; }

public:
	CLONE(CWeapon_bu);
	CWeapon_bu();
	CWeapon_bu(const CWeapon_bu& _origin);
	virtual ~CWeapon_bu() override;
};