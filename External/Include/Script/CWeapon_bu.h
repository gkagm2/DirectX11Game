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


	SharedPtr<CPrefab> m_pMainBullet; // ����� �Ѿ�

	// �����鰰����� ó��
	CGameObject* m_pChainSawColObj;
	// ���� �Ѿ�
	SharedPtr<CPrefab> m_pShotgunBullet;
	// �ӽŰ� �Ѿ�
	SharedPtr<CPrefab> m_pMachinegunBullet;
	// ���̾� �Ѿ�
	SharedPtr<CPrefab> m_pFlameBullet;
	// ������ �Ѿ�
	//SharedPtr<CPrefab> m_pLaserBullet;
	// ��ź �Ѿ�
	//SharedPtr<CPrefab> m_GranadeBullet;



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
	// �߻� ��ġ, ���� ��ġ�� �߻� ����, ȸ������
	void Fire(const Vector3& _vMuzzlePos, const Vector3& _vRot, const Vector3& _vShootDir, UINT _iTag);
	void SetUseableWeapon(E_WeaponType_bu _eType, bool _bUse){ m_arrWeaponUse[(UINT)_eType] = _bUse; }
	bool IsUseableWeapon(E_WeaponType_bu _eType) { return m_arrWeaponUse[(UINT)_eType]; }

public:
	CLONE(CWeapon_bu);
	CWeapon_bu();
	CWeapon_bu(const CWeapon_bu& _origin);
	virtual ~CWeapon_bu() override;
};