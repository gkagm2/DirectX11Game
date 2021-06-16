#pragma once
#include "CObject.h"
#include "CTexture.h"

struct TAnimationFrame { // Animation Frame
	Vector2 vLeftTop;	// 애니메이션 이미지의 좌-상단 시작점
	Vector2 vFrameSize;	// 좌-상단 기준으로부터 Width, Height
	float fDuration;	// Frame의 유지 시간
};

class CAnimation2D : public CObject
{
private:
	SharedPtr<CTexture> m_pTexture;
	vector<TAnimationFrame> m_vecAnimFrame;
	bool m_bFinish;	// 애니메이션이 끝났는지 여부

public:
	void Create(SharedPtr<CTexture>& _pTexture, const Vector2& _vLeftTop, const Vector2& _vFrameSize, float _fDuration, int _iFrameCount);

public:
	CAnimation2D();
	virtual ~CAnimation2D() override;
};