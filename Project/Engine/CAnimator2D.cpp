#include "pch.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"

CAnimator2D::CAnimator2D() :
	CComponent(E_ComponentType::Animator2D),
	m_pCurAnimation(nullptr),
	m_eAnimationState(E_AnimationState::Once)
{
}

CAnimator2D::~CAnimator2D()
{
}

CAnimation2D* CAnimator2D::FindAnimation(const tstring& _strName)
{
	unordered_map<tstring, CAnimation2D*>::iterator iter = m_unmapAnim.find(_strName);

	if (m_unmapAnim.end() == iter)
		return nullptr;
	return iter->second;
}

void CAnimator2D::CreateAnimation(const tstring& _strName, SharedPtr<CTexture> _pAtlas, const Vector2& _vLeftTop, const Vector2& _vFrameSize, float _fDuration, int _iFrameCount)
{
	CAnimation2D* pAnim = FindAnimation(_strName);
	assert(!pAnim && _T("애니메이션이 이미 존재함")); // 존재 시 오류

	pAnim = new CAnimation2D;
	pAnim->SetName(_strName);
	pAnim->Create(_pAtlas, _vLeftTop, _vFrameSize, _fDuration, _iFrameCount);

	m_unmapAnim.insert(std::make_pair(_strName, pAnim));
}