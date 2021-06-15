#pragma once
#include "CScript.h"
class CEnemyScript_sh : public CScript
{
private:
	float m_fHp;
	float m_fMoveSpeed;
	float m_fBulletSpeed;

	SharedPtr<CMaterial> m_pSharedMtrl;
	SharedPtr<CMaterial> m_pCloneMtrl;
	int m_iColorFlag = 0;

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

public:
	void Move();
	void ColorChange();

public:
	CEnemyScript_sh();
	virtual~CEnemyScript_sh() override;
};