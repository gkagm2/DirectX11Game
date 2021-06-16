#pragma once
#include "CComponent.h"
#include "CTexture.h"
class CAnimation2D;

enum class E_AnimationState {
	Loop, // repeat 0 to 10 
	Once, // 0 to 10 and done
	PingPong, // repeat 0 to 10 and 10 to 0 
};

class CAnimator2D : public CComponent
{
private:
	unordered_map<tstring, CAnimation2D*> m_unmapAnim;
	CAnimation2D* m_pCurAnimation;
	E_AnimationState m_eAnimationState;

public:
	CAnimation2D* FindAnimation(const tstring& _strName);
	void CreateAnimation(const tstring& _strName, SharedPtr<CTexture> _pAtlas, const Vector2& _vLeftTop, const Vector2& _vFrameSize, float _fDuration, int _iFrameCount);

public:
	void SetAnimationState(E_AnimationState _eState) { m_eAnimationState = _eState; }
	E_AnimationState GetAnimationState() { return m_eAnimationState; }

public:
	CAnimator2D();
	virtual ~CAnimator2D() override;
};