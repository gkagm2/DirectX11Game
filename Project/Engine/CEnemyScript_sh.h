#pragma once
#include "CScript.h"
class CEnemyScript_sh : public CScript
{
	float m_fHp;
	float m_fMoveSpeed;
	float m_fBulletSpeed;

public:
	CEnemyScript_sh();
	virtual~CEnemyScript_sh() override;
};