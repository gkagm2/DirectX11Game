#pragma once
#include <Engine\CScript.h>
class CBullet_bu : public CScript
{
public:
	enum class E_BouncingParticleType_bu {
		oneBulletOneParticle,
		oneBulletMultipleParticle,
		none,
	};
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
	
	
	E_BouncingParticleType_bu m_eBouncingParticleType;

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnCollisionEnter2D(CCollider2D* _pOther) override;

	void SetShootDir(const Vector3& _vShootDir) { m_vShootDir = _vShootDir; }
	void SetBouncingParticleType(E_BouncingParticleType_bu _eType) { m_eBouncingParticleType = _eType; }

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;
	
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	float GetSpeed() { return m_fSpeed; }


private:
	void _ParticleMultiple(const Vector3& _vDir);
	void _ParticleOne(const Vector3& _vDir);


public:
	CLONE(CBullet_bu);
	CBullet_bu();
	CBullet_bu(const CBullet_bu& _origin);
	virtual ~CBullet_bu() override;
};