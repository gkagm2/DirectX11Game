#include "pch.h"
#include "CBullet_bu.h"
#include "CCharacter_bu.h"

CBullet_bu::CBullet_bu() :
	CScript((UINT)SCRIPT_TYPE::BULLET_BU),
	m_vShootDir(Vector3::Right),
	m_fSpeed(10.f),
	m_fDamage(5.f),
	m_fLifeTime(0.f),
	m_fMaxLifeTime(4.f)
{
	AddParam(TScriptParam{ _T("Speed"), E_ScriptParam::FLOAT, &m_fSpeed });
	AddParam(TScriptParam{ _T("Damage"), E_ScriptParam::FLOAT, &m_fDamage});
}

CBullet_bu::CBullet_bu(const CBullet_bu& _origin) :
	CScript((UINT)SCRIPT_TYPE::BULLET_BU),
	m_vShootDir(_origin.m_vShootDir),
	m_fSpeed(_origin.m_fSpeed),
	m_fDamage(_origin.m_fDamage),
	m_fLifeTime(_origin.m_fLifeTime),
	m_fMaxLifeTime(_origin.m_fMaxLifeTime)
{
	AddParam(TScriptParam{ _T("Speed"), E_ScriptParam::FLOAT, &m_fSpeed });
	AddParam(TScriptParam{ _T("Damage"), E_ScriptParam::FLOAT, &m_fDamage });
}

CBullet_bu::~CBullet_bu()
{
}

void CBullet_bu::Awake()
{
	tstring str = STR_DIR_PATH_Prefab;
	tstring strPrefabName;
	strPrefabName = str + _T("ObjBulletBouncingParticle_bu");
	m_pObjParticlePref = CResourceManager::GetInstance()->FindRes<CPrefab>(strPrefabName);
	strPrefabName = str + _T("HumanBulletBouncingParticle_bu");
	m_pHumanParticlePref = CResourceManager::GetInstance()->FindRes<CPrefab>(strPrefabName);
	strPrefabName = str + _T("WallBulletBouncingParticle_bu");
	m_pWallParticlePref = CResourceManager::GetInstance()->FindRes<CPrefab>(strPrefabName);
	assert(m_pObjParticlePref.Get());
	assert(m_pHumanParticlePref.Get());
	assert(m_pWallParticlePref.Get());
}

void CBullet_bu::Start()
{
}

void CBullet_bu::Update()
{
	m_fLifeTime += DT;
	if (m_fLifeTime >= m_fMaxLifeTime) {
		CObject::DestroyGameObjectEvn(GetGameObject());
		return;
	}

	Vector3 vPosition = Transform()->GetLocalPosition();
	vPosition = vPosition  + m_fSpeed * DT * m_vShootDir;
	Transform()->SetLocalPosition(vPosition);
	
}

void CBullet_bu::OnCollisionEnter2D(CCollider2D* _pOther)
{
	UINT iTag = _pOther->GetGameObject()->GetTag();
	UINT iEnemyTag = (UINT)E_Tag::Enemy;
	UINT iObjectTag = (UINT)E_Tag::Object;
	UINT iPlayerTag = (UINT)(E_Tag::Player);
	UINT iEnemyBulletTag = (UINT)E_Tag::Enemy_Bullet;
	UINT iPlayerBulletTag = (UINT)E_Tag::Player_Bullet;
	UINT iWallLayer = (UINT)E_Layer::Wall;
	CGameObject* pObj = _pOther->GetGameObject();
	bool bManTouched = false;
	bool bWallTouched = false;
	bool bObjTouched = false;
	

	UINT iMyTag = GetGameObject()->GetTag();

	if (iMyTag == iPlayerBulletTag) {
		if (iTag == iEnemyTag) {
			CCharacter_bu* pChar = pObj->GetComponent<CCharacter_bu>();
			pChar->DamagedMe(m_fDamage);
			bManTouched = true;
		}
		if (iTag == iObjectTag) {
			bManTouched = true;
		}
	}
	else if (iMyTag == iEnemyBulletTag) {
		if (iTag == iPlayerTag) {
			CCharacter_bu* pChar = pObj->GetComponent<CCharacter_bu>();
			pChar->DamagedMe(m_fDamage);
			bManTouched = true;
		}
		if (iTag == iObjectTag) {
			bManTouched = true;
		}
	}
	else if (iWallLayer == GetGameObject()->GetLayer()) {
		bWallTouched = true;
	}

	UINT iParticleLayer = (UINT)E_Layer::Object;
	if (bManTouched) {
		CGameObject* pParticle = CObject::InstantiateEvn(m_pHumanParticlePref, Transform()->GetPosition(), iParticleLayer);
		DestroyGameObjectEvn(GetGameObject());
	}
	else if (bObjTouched) {
		CGameObject* pParticle = CObject::InstantiateEvn(m_pObjParticlePref, Transform()->GetPosition(), iParticleLayer);
		DestroyGameObjectEvn(GetGameObject());
	}
	else if (bWallTouched) {
		CGameObject* pParticle = CObject::InstantiateEvn(m_pWallParticlePref, Transform()->GetPosition(), iParticleLayer);
		DestroyGameObjectEvn(GetGameObject());
	}
}

bool CBullet_bu::SaveToScene(FILE* _pFile)
{
	FWrite(m_fSpeed, _pFile);
	FWrite(m_fDamage, _pFile);
	return true;
}

bool CBullet_bu::LoadFromScene(FILE* _pFile)
{
	FRead(m_fSpeed, _pFile);
	FRead(m_fDamage, _pFile);
	return true;
}
