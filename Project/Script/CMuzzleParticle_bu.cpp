#include "pch.h"
#include "CMuzzleParticle_bu.h"

CMuzzleParticle_bu::CMuzzleParticle_bu() :
	CScript((UINT)SCRIPT_TYPE::MUZZLEPARTICLE_BU),
	pMuzzleAnim{ nullptr }
{
}

void CMuzzleParticle_bu::Update()
{
	if (!Animator2D())
		return;
	if (!Animator2D()->GetCurAnimation()) {
		if (Animator2D()->GetCurAnimation()->IsFinished())
			GetGameObject()->SetActive(false);
	}
}

void CMuzzleParticle_bu::OnEnable()
{
	if (!Animator2D())
		return;
	GetGameObject()->Animator2D()->Play(E_AnimationState::Once, true);
}

void CMuzzleParticle_bu::OnDisable()
{
}