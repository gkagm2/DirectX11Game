#pragma once
#include "CObject.h"
#include "CTexture.h"
#include "CAnimator2D.h"

struct TAnimationFrame { // Animation Frame
	Vector2 vLeftTop;	// 애니메이션 이미지의 좌-상단 시작점
	Vector2 vFrameSize;	// 좌-상단 기준으로부터 Width, Height
	Vector2 vOffsetPos; //오프셋 좌표
	Vector2 vBaseSize;	// 애니메이션 이미지의 오프셋을 이동해도 보여주기 위한 영역의 최대 사이즈 (UV)
	float fDuration;	// Frame의 유지 시간
};

class CAnimation2D : public CObject
{
private:
	SharedPtr<CTexture> m_pTexture;
	vector<TAnimationFrame> m_vecAnimFrame;

	int m_iCurFrameIdx;		// 현재 애니메이션 인덱스
	float m_fAccTime;		// 누적 시간(Accumulation Time)
	bool m_bFinish;			// 애니메이션이 끝났는지 여부

public:
	void LateUpdate();
	virtual void UpdateData() override;
	void Create(TAnimation2DDesc& _tAnimation2DDesc);

	void Reset() {
		m_iCurFrameIdx = 0;
		m_fAccTime = 0.f;
		m_bFinish = false;
	}

	bool IsFinished() { return m_bFinish; }

	void Save(const tstring& _strRelativeFilePath, const wstring& _strFileName);
	void Load(const tstring& _strRelativeFilePath);

public:
	CAnimation2D();
	virtual ~CAnimation2D() override;

	friend class CAnimator2D;
};