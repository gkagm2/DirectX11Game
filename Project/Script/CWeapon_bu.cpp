#include "pch.h"
#include "CWeapon_bu.h"
#include "CBullet_bu.h"
#include "CCharacter_bu.h"

tstring CWeapon_bu::m_strExplain = _T("0 : Chainsaw\n1 : Shotgun\n2 : MachineGun\n3 : FlameThrower\n4 : GrenadeLauncher\n5 : LaserGun");

CWeapon_bu::CWeapon_bu() :
	CScript((UINT)SCRIPT_TYPE::WEAPON_BU),
	m_tWeaponInfo{},
	m_eCurType(E_WeaponType_bu::Chainsaw),
	m_pChainSawObj{ nullptr },
	m_pGunImageObj{ nullptr },
	m_fReloadDelayTime{0.3f},
	m_arrWeaponUse{}
{
	_InitWeaponInfo();

	AddParam(TScriptParam{ _T("Weapon Type"),E_ScriptParam::INT, &m_eCurType });
	AddParam(TScriptParam{ _T("Description\n"), E_ScriptParam::STRING_PRINT, &m_strExplain });
}

CWeapon_bu::CWeapon_bu(const CWeapon_bu& _origin) :
	CScript((UINT)SCRIPT_TYPE::WEAPON_BU),
	m_tWeaponInfo{},
	m_eCurType(E_WeaponType_bu::Chainsaw),
	m_pChainSawObj{ nullptr },
	m_pGunImageObj{ nullptr },
	m_fFireTime{_origin.m_fFireTime},
	m_fMaxFireTime{_origin.m_fMaxFireTime},
	m_fReloadDelayTime{_origin.m_fReloadDelayTime},
	m_arrWeaponUse{}
{
	for (int i = 0; i < (int)E_WeaponType_bu::End; ++i)
		m_arrWeaponUse[i] = _origin.m_arrWeaponUse[i];
	_InitWeaponInfo();

	AddParam(TScriptParam{ _T("Weapon Type"),E_ScriptParam::INT, &m_eCurType });
	AddParam(TScriptParam{ _T("Description\n"), E_ScriptParam::STRING_PRINT, &m_strExplain });
}

CWeapon_bu::~CWeapon_bu()
{
}

void CWeapon_bu::Start()
{
	m_pCharacter = GetGameObject()->GetComponent<CCharacter_bu>();
	m_pChainSawObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_Chainsaw);
	m_pGunImageObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_GunImage);
	m_pChainSawColObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_ChainsawCol);
	assert(m_pChainSawObj);
	assert(m_pGunImageObj);
	assert(m_pChainSawColObj);

	tstring strPath = CPathManager::GetInstance()->GetContentPath();
	m_pShotgunBullet = CResourceManager::GetInstance()->LoadRes<CPrefab>(_T("prefab\\BulletShotgun_bu.pref"));
	m_pMachinegunBullet = CResourceManager::GetInstance()->LoadRes<CPrefab>(_T("prefab\\BulletRifle_bu.pref"));
	m_pFlameBullet =CResourceManager::GetInstance()->LoadRes<CPrefab>(_T("prefab\\BulletFlame_bu.pref"));
	assert(m_pShotgunBullet.Get());
	assert(m_pMachinegunBullet.Get());
	assert(m_pFlameBullet.Get());

	SetUseableWeapon(m_eCurType, true);
	ChangeWeapon(m_eCurType);
}

void CWeapon_bu::Update()
{
	m_fFireTime += DT;
	if (m_fFireTime >= m_fMaxFireTime) {
		if (GetCurWeapon().bInfinity)
			m_bIsEnableFire = true;
		else {
			if (GetCurWeapon().iCurBullet > 0)
				m_bIsEnableFire = true;
			else
				m_bIsEnableFire = false;
		}
	}
	else
		m_bIsEnableFire = false;

	if (m_bIsEnableFire) {
		if (E_WeaponType_bu::Chainsaw == GetCurWeaponType())
			if(m_pChainSawColObj->IsActive())
				m_pChainSawColObj->SetActive(false);
	}
}

bool CWeapon_bu::SaveToScene(FILE* _pFile)
{
	FWrite(m_eCurType, _pFile);
	return true;
}

bool CWeapon_bu::LoadFromScene(FILE* _pFile)
{
	FRead(m_eCurType, _pFile);
	return true;
}

bool CWeapon_bu::Fire(const Vector3& _vMuzzlePos, const Vector3& _Rot, const Vector3& _vShootDir, UINT _iTag)
{
	bool isEnableFire = IsEnableFire();

	if (isEnableFire) {
		if (!GetCurWeapon().bInfinity) {
			
		}
		m_fFireTime = 0.f;
	}

	if (isEnableFire) {
		//타입에 따라서 프리펩을 생성하는걸 다르게 한다.
		if (m_eCurType == E_WeaponType_bu::Chainsaw)
			m_pChainSawColObj->SetActive(true);
		else if(m_eCurType == E_WeaponType_bu::MachineGun) {
			CGameObject* pBulletObj = m_pMainBullet->Instantiate();
			CBullet_bu* pBul = pBulletObj->GetComponent<CBullet_bu>();
			pBulletObj->SetTag(_iTag, true);
			pBul->Transform()->SetLocalPosition(_vMuzzlePos);
			pBul->SetShootDir(_vShootDir);
			pBul->Transform()->SetLocalRotation(_Rot);
			UINT iLayer = (UINT)E_Layer::Object;
			CObject::CreateGameObjectEvn(pBulletObj, iLayer);
			--GetCurWeapon().iCurBullet;
			GetCurWeapon().iCurBullet = max(0, GetCurWeapon().iCurBullet);
		}
		else if (m_eCurType == E_WeaponType_bu::Shotgun) {
			const int iShotgunBulletCnt = 8;
			for (int i = 0; i < iShotgunBulletCnt; ++i) {
				CGameObject* pBulletObj = m_pMainBullet->Instantiate();
				CBullet_bu* pBul = pBulletObj->GetComponent<CBullet_bu>();
				pBul->SetSpeed(pBul->GetSpeed() - (rand() % 10) * 0.9f + 3.f);
				pBul->SetBouncingParticleType(CBullet_bu::E_BouncingParticleType_bu::oneBulletOneParticle);
				pBulletObj->SetTag(_iTag, true);

				pBul->Transform()->SetLocalPosition(_vMuzzlePos);
				Vector3 vShootDir = ::Rotate(_vShootDir, (i - iShotgunBulletCnt * 0.5f));
				// 각 사이값을 구한다.
				pBul->SetShootDir(vShootDir);
				pBul->Transform()->SetLocalRotation(_Rot);
				UINT iLayer = (UINT)E_Layer::Object;
				CObject::CreateGameObjectEvn(pBulletObj, iLayer);
			}
			--GetCurWeapon().iCurBullet;
			GetCurWeapon().iCurBullet = max(0, GetCurWeapon().iCurBullet);
		}
		else {
			// TODO :
		}
		
		// Muzzle 파티클 Action
		if (m_eCurType == E_WeaponType_bu::MachineGun ||
			m_eCurType == E_WeaponType_bu::Shotgun ||
			m_eCurType == E_WeaponType_bu::GrenadeLauncher) {
			m_pCharacter->GetMuzzleParticleObj()->SetActive(true,true);
		}

	}
	return isEnableFire;
}

void CWeapon_bu::AddWeaponBulletItem(E_WeaponType_bu _eType)
{
	TWeaponInfo_bu& tWeapon = m_tWeaponInfo[(UINT)_eType];
	int totalBullet = min(tWeapon.iMaxBullet, tWeapon.iCurBullet + tWeapon.iGetBulletCnt);
	tWeapon.iCurBullet = totalBullet;
}

void CWeapon_bu::ChangeWeapon(E_WeaponType_bu _eType)
{
	if (!m_pChainSawObj) {
		m_pChainSawObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_Chainsaw);
		assert(m_pChainSawObj);
	}
	if (!m_pGunImageObj) {
		m_pGunImageObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_GunImage);
		assert(m_pGunImageObj);

	}
	if (!m_pChainSawColObj) {
		m_pChainSawColObj = GetGameObject()->FindGameObjectInChilds(BUTCHER_ObjName_ChainsawCol);
		assert(m_pChainSawColObj);
	}

	m_eCurType = _eType;
	if (_eType == E_WeaponType_bu::Chainsaw) {
		m_pGunImageObj->SetActive(false);
		m_pChainSawObj->SetActive(true);
		m_pChainSawColObj->Collider2D()->SetActive(false);
	}
	else {
		m_pGunImageObj->SetActive(true);
		m_pChainSawObj->SetActive(false);
		m_pGunImageObj->Animator2D()->SetCurAnimationFrame((UINT)m_eCurType);
	}
	m_fMaxFireTime = m_tWeaponInfo[(UINT)m_eCurType].fRpm;
	m_fFireTime = m_fMaxFireTime - m_fReloadDelayTime;

	switch (_eType) {
	case E_WeaponType_bu::Shotgun:
		m_pMainBullet = m_pShotgunBullet;
		break;
	case E_WeaponType_bu::MachineGun:
		m_pMainBullet = m_pMachinegunBullet;
		break;
	case E_WeaponType_bu::FlameThrower:
		m_pMainBullet = m_pFlameBullet;
		break;
	}
	m_strCurWeaponName = WeaponTypeToStr_bu(m_eCurType);
}

void CWeapon_bu::_InitWeaponInfo()
{
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::Chainsaw];
		tInfo = {
			tInfo.iMaxBullet = 0,
			tInfo.iCurBullet = 0,
			tInfo.iGetBulletCnt = 0,
			tInfo.fRpm = 0.1f,
			tInfo.bInfinity = true
		};
	}
	
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::Shotgun];
		tInfo = {
			tInfo.iMaxBullet = 24,
			tInfo.iCurBullet = 0,
			tInfo.iGetBulletCnt = 8,
			tInfo.fRpm = 1.f,
			tInfo.bInfinity = false
		};
	}
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::MachineGun];
		tInfo = {
			tInfo.iMaxBullet = 75,
			tInfo.iCurBullet = 0,
			tInfo.iGetBulletCnt = 25,
			tInfo.fRpm = 0.1f,
			tInfo.bInfinity = false
		};
	}
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::FlameThrower];
		tInfo = {
			tInfo.iMaxBullet = 90,
			tInfo.iCurBullet = 0,
			tInfo.iGetBulletCnt = 8,
			tInfo.fRpm = 0.8f,
			tInfo.bInfinity = false
		};
	}
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::GrenadeLauncher];
		tInfo = {
			tInfo.iMaxBullet = 20,
			tInfo.iCurBullet = 0,
			tInfo.iGetBulletCnt = 5,
			tInfo.fRpm = 2.f,
			tInfo.bInfinity = false
		};
	}
	{
		TWeaponInfo_bu& tInfo = m_tWeaponInfo[(UINT)E_WeaponType_bu::LaserGun];
		tInfo = {
			tInfo.iMaxBullet = 15,
			tInfo.iCurBullet = 0,
			tInfo.iGetBulletCnt = 2,
			tInfo.fRpm = 2.f,
			tInfo.bInfinity = false
		};
	}
}

tstring WeaponTypeToStr_bu(E_WeaponType_bu _eType)
{
	tstring strName{};
	switch (_eType) {
	case E_WeaponType_bu::Chainsaw:
		strName = _T("Chainsaw");
		break;
	case E_WeaponType_bu::FlameThrower:
		strName = _T("FlameThrower");
		break;
	case E_WeaponType_bu::GrenadeLauncher:
		strName = _T("GrenadeLauncher");
		break;
	case E_WeaponType_bu::LaserGun:
		strName = _T("LaserGun");
		break;
	case E_WeaponType_bu::MachineGun:
		strName = _T("MachineGun");
		break;
	case E_WeaponType_bu::Shotgun:
		strName = _T("Shotgun");
		break;
	default:
		assert(nullptr);
		break;
	}
	return strName;
}
