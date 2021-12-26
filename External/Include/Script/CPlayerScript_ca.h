#pragma once
#include <Engine\CScript.h>
class CPlayerScript_ca : public CScript
{
public:
	float m_fSpeed;
	Vector3 m_vTempVec;

	CGameObject* pTargetObj;
public:
	virtual void Start() override;
	virtual void Update() override;

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CPlayerScript_ca);
	CPlayerScript_ca();
	virtual ~CPlayerScript_ca() override;
};