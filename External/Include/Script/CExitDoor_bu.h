#pragma once
#include <Engine\CScript.h>
class CExitDoor_bu : public CScript
{
	Vector3 m_vDoorClosePos;
	Vector3 m_vDoorOpenPos;
	float m_fTime;

	bool m_bIsPlayerIn;

	virtual void Update() override;

	virtual void Awake() override;
	virtual void OnCollisionEnter2D(CCollider2D* _pCol) override;
	virtual void OnCollisionExit2D(CCollider2D* _pCol) override;
public:
	CLONE(CExitDoor_bu);	
	CExitDoor_bu();
	virtual ~CExitDoor_bu() override {}
};