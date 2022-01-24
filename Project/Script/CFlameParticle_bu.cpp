#include "pch.h"
#include "CFlameParticle_bu.h"

CFlameParticle_bu::CFlameParticle_bu() :
	CScript((UINT)SCRIPT_TYPE::FLAMEPARTICLE_BU)
{
}

void CFlameParticle_bu::Awake()
{
	m_pAnim = GetGameObject()->FindComponentInChilds<CAnimator2D>();
	assert(m_pAnim);

	// ·£´ý »Ñ¸®±â
	const int iKeyCnt = 4;
	static const tstring strAnimKeys[iKeyCnt] = {
		_T("Bul_Particle_Flame1_bu"),
		_T("Bul_Particle_Flame2_bu"),
		_T("Bul_Particle_Flame3_bu"),
		_T("Bul_Particle_Flame4_bu")
	};
	int idx = rand() % iKeyCnt;
	m_pAnim->Play(strAnimKeys[idx], E_AnimationState::Once, true);
}

void CFlameParticle_bu::Update()
{
	if (m_pAnim->GetCurAnimation()->IsCurFrameEnd())
		DestroyGameObjectEvn(GetGameObject());
}