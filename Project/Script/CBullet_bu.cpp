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
	UINT iWallTag = (UINT)E_Tag::Wall;
	UINT iPlayerTag = (UINT)(E_Tag::Player);
	CGameObject* pObj = _pOther->GetGameObject();
	bool bTouched = false;

	if (iTag == iEnemyTag || iTag == iPlayerTag) {
		CCharacter_bu* pcha= pObj->GetComponent<CCharacter_bu>();
		pcha->DamagedMe(m_fDamage);
		if (0.f == pcha->GetHp()) {
			pcha->OnDead();
		}
		bTouched = true;
	}
	if (iTag == iObjectTag) {
		bTouched = true;
	}
	if (iTag == iWallTag) {
		bTouched = true;
	}
	if (bTouched) {
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
