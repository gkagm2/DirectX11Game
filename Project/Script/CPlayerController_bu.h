#pragma once
#include "CCharacter_bu.h"
class CRigidbody2D;
class CAnimator2D;
class CPlayerController_bu : public CCharacter_bu
{
private:
private:
	CRigidbody2D* m_pRigid;
	CAnimator2D* m_pAnim;

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	virtual void Interaction() {}
	virtual void Attack();
	virtual void Jump();
	virtual void Move();

	void Aim(const Vector3& _vPos, CTransform* _pTr);

public:
	CLONE(CPlayerController_bu);
	CPlayerController_bu();
	virtual ~CPlayerController_bu() override;
};