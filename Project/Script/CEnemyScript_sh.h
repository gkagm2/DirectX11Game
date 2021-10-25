#pragma once
#include <Engine\CScript.h>
class CEnemyScript_sh : public CScript
{
private:
	float m_fHp;
	float m_fMoveSpeed;
	float m_fBulletSpeed;

	float fAngle;

	SharedPtr<CMaterial> m_pSharedMtrl;
	SharedPtr<CMaterial> m_pCloneMtrl;
	int m_iColorFlag = 0;

	Vector3 m_vOriginalScale;
	float m_fOriginalHp;
	float m_fBackDistance; // 총알을 맞았을 때 뒤로 물러난 거리

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;


	virtual void OnCollisionEnter2D(CCollider2D* _pOther) override;
	virtual void OnCollisionStay2D(CCollider2D* _pOther) override;
	virtual void OnCollisionExit2D(CCollider2D* _pOther) override;


public:
	void Move();
	
public:
	float GetHP() { return m_fHp; }
	void SetHp(float _fHp) { m_fHp = _fHp; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CEnemyScript_sh);
	CEnemyScript_sh();
	virtual~CEnemyScript_sh() override;
};