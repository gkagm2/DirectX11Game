#include "pch.h"
#include "CBullet_bu.h"
#include "CCharacter_bu.h"
#include "CBulletBouncingParticle_bu.h"

CBullet_bu::CBullet_bu() :
	CScript((UINT)SCRIPT_TYPE::BULLET_BU),
	m_vShootDir(Vector3::Right),
	m_fSpeed(10.f),
	m_fDamage(5.f),
	m_fLifeTime(0.f),
	m_fMaxLifeTime(4.f),
	m_eBouncingParticleType{E_BouncingParticleType_bu::oneBulletOneParticle}
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
	m_fMaxLifeTime(_origin.m_fMaxLifeTime),
	m_eBouncingParticleType{_origin.m_eBouncingParticleType}
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
	strPrefabName = str + _T("ObjBulletBouncingParticle_bu") + STR_EXTENSION_Prefab;
	m_pObjParticlePref = CResourceManager::GetInstance()->FindRes<CPrefab>(strPrefabName);
	strPrefabName = str + _T("HumanBulletBouncingParticle_bu") + STR_EXTENSION_Prefab;
	m_pHumanParticlePref = CResourceManager::GetInstance()->FindRes<CPrefab>(strPrefabName);
	strPrefabName = str + _T("WallBulletBouncingParticle_bu") + STR_EXTENSION_Prefab;
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
	if (iMyTag == iEnemyBulletTag) {
		if (iTag == iPlayerTag) {
			CCharacter_bu* pChar = pObj->GetComponent<CCharacter_bu>();
			pChar->DamagedMe(m_fDamage);
			bManTouched = true;
		}
		if (iTag == iObjectTag) {
			bManTouched = true;
		}
	}
	if (iWallLayer == pObj->GetLayer()) {
		bWallTouched = true;
	}
	

	UINT iParticleLayer = (UINT)E_Layer::Object;
	if (bManTouched) {
		CGameObject* pParticle = CObject::InstantiateEvn(m_pHumanParticlePref, Transform()->GetPosition(), iParticleLayer);
			_ParticleOne();
		DestroyGameObjectEvn(GetGameObject());
	}
	else if (bObjTouched) {
		CGameObject* pParticle = CObject::InstantiateEvn(m_pObjParticlePref, Transform()->GetPosition(), iParticleLayer);
		DestroyGameObjectEvn(GetGameObject());
	}
	else if (bWallTouched) {
		if (E_BouncingParticleType_bu::oneBulletMultipleParticle == m_eBouncingParticleType)
			_ParticleMultiple();
		else if (E_BouncingParticleType_bu::oneBulletOneParticle == m_eBouncingParticleType)
			_ParticleOne();
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

void CBullet_bu::_ParticleMultiple()
{
	UINT iParticleLayer = (UINT)E_Layer::Object;
	if (m_eBouncingParticleType == E_BouncingParticleType_bu::oneBulletMultipleParticle) {
		float degree = 360 / 12;
		for (int i = 0; i < 12; ++i) {
			CGameObject* pParticle = CObject::InstantiateEvn(m_pWallParticlePref, Transform()->GetPosition(), iParticleLayer);

			Vector3 vShootDir = ::Rotate(-m_vShootDir, degree * i + rand() % 25);
			CBulletBouncingParticle_bu* pbp = pParticle->GetComponent< CBulletBouncingParticle_bu>();
			pbp->SetParticle(vShootDir);

		}
	}
}

void CBullet_bu::_ParticleOne()
{
	UINT iParticleLayer = (UINT)E_Layer::Object;
	CGameObject* pParticle = CObject::InstantiateEvn(m_pWallParticlePref, Transform()->GetPosition(), iParticleLayer);
	CBulletBouncingParticle_bu* pbp = pParticle->GetComponent< CBulletBouncingParticle_bu>();
	pbp->SetParticle(-m_vShootDir);
}
