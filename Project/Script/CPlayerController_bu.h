#pragma once
#include "CCharacter_bu.h"
class CPlayerController_bu : public CCharacter_bu
{
public:
	class CRigidbody2D* m_pRigid;
	CAnimator2D* m_pAnim;

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	CLONE(CPlayerController_bu);
	CPlayerController_bu();
	virtual ~CPlayerController_bu() override;
};