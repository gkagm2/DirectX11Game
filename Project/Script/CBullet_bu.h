#pragma once
#include <Engine\CScript.h>
class CBullet_bu : public CScript
{
private:
	Vector3 m_vShootDir;
	float m_fSpeed;
	float m_fDamage;
	float m_fLifeTime;
	float m_fMaxLifeTime;


	CGameObject* m_pWallParticle;
	CGameObject* m_pHumanParticle;
	CGameObject* m_pObjParticle;

	SharedPtr<CPrefab> m_pWallParticlePref;
	SharedPtr<CPrefab> m_pHumanParticlePref;
	SharedPtr<CPrefab> m_pObjParticlePref;


public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnCollisionEnter2D(CCollider2D* _pOther) override;

	void SetShootDir(const Vector3& _vShootDir) { m_vShootDir = _vShootDir; }

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CBullet_bu);
	CBullet_bu();
	CBullet_bu(const CBullet_bu& _origin);
	virtual ~CBullet_bu() override;
};