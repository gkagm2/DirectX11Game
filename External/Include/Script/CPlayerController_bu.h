#pragma once
#include "CCharacter_bu.h"
class CRigidbody2D;
class CAnimator2D;
class CPlayerController_bu : public CCharacter_bu
{
private:
	CRigidbody2D* m_pRigid;
	CAnimator2D* m_pLegAnim;
	CAnimator2D* m_pTorsoAnimSprite;
	SharedPtr<CPrefab> m_pBulletPref;

	float m_fShotTime;
	float m_fMaxShotTime;

	CGameObject* m_pMuzzleObj;
	CGameObject* m_pGunRotationPosObj;

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

	virtual void OnCollisionStay2D(CCollider2D* pCol) override;

public:
	virtual void Interaction() override {}


	virtual void OnBehavior();

	virtual void OnMoveStart() override;
	virtual void OnMoveUpdate() override;
	virtual void OnMoveEnd() override;

	virtual void OnIdleStart() override;
	virtual void OnIdleUpdate() override;
	virtual void OnIdleEnd() override;

	virtual void OnJumpStart() override;
	virtual void OnJumpUpdate() override;
	virtual void OnJumpEnd() override;

	virtual void OnDeadStart() override;
	virtual void OnDeadUpdate() override;
	virtual void OnDeadEnd() override;

public:
	virtual bool SaveToScene(FILE* _pFile);
	virtual bool LoadFromScene(FILE* _pFile);


public:
	CLONE(CPlayerController_bu);
	CPlayerController_bu();
	virtual ~CPlayerController_bu() override;
};