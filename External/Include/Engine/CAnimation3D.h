#pragma once
#include "CObject.h"

struct TAnimation3DFrame {
	Vector2 vLeftTopUV;	// 애니메이션 이미지의 좌-상단 시작점
	Vector2 vFrameSizeUV;	// 좌-상단 기준으로부터 Width, Height
	Vector2 vOffsetPosUV; //오프셋 좌표
	Vector2 vBaseSizeUV;	// 애니메이션 이미지의 오프셋을 이동해도 보여주기 위한 영역의 최대 사이즈 (UV)
	float fDuration;	// Frame의 유지 시간
};

///////////////////////////////////////////////////////
// 
// Animation 2D Description
struct TAnimation3DDesc {
	tstring		strName;
	SharedPtr<CTexture> pAtlas; // 아틀라스 텍스쳐
	Vector2		vLeftTop;		// 애니메이션 텍스쳐의 좌상단 좌표
	Vector2		vFrameSize;		// 애니메이션 텍스쳐의 크기
	Vector2		vBaseSize;		// 애니메이션 텍스쳐의 offset을 표현할 최대 범위
	Vector2		vOffsetPos;		// Offset 위치
	float		fDuration;		// 지연 시간
	int			iFrameCount;	// 애니메이션 개수
	TAnimation3DDesc() :
		strName(_T("")),
		pAtlas(nullptr),
		vLeftTop(Vector2{ 0.f,0.f }),
		vFrameSize(Vector2{ 0.f,0.f }),
		vBaseSize(Vector2{ 0.f,0.f }),
		vOffsetPos(Vector2{ 0.f,0.f }),
		fDuration(0.f),
		iFrameCount(0) {}
	TAnimation3DDesc(const tstring& _strName, SharedPtr<CTexture> _pAtlas, const Vector2& _vLeftTop, const Vector2& _vFrameSize, const Vector2& _vBaseSize, float _fDuration, int _iFrameCount, const Vector2& _vOffsetPos = Vector2(0.f, 0.f)) :
		strName(_strName),
		pAtlas(_pAtlas),
		vLeftTop(_vLeftTop),
		vFrameSize(_vFrameSize),
		vBaseSize(_vBaseSize),
		vOffsetPos(_vOffsetPos),
		fDuration(_fDuration),
		iFrameCount(_iFrameCount) {}
};


enum class E_AnimationState {
	Loop, // repeat 0 to 10 
	Once, // 0 to 10 and done
	PingPong, // repeat 0 to 10 and 10 to 0 
	Fixed,
	End,
};
tstring AnimationStateToStr(E_AnimationState _eState);


class CAnimator3D;
class CAnimation3D : public CObject
{
private:
	CAnimator3D* m_pAnimator;
	vector<SharedPtr<CTexture>> m_vecTex;
	SharedPtr<CTexture>		m_pTexture;
	vector<TAnimation3DFrame> m_vecAnimFrame;
	tstring m_strAnimPath; // 저장할 곳의 상대경로

	int		m_iCurFrameIdx;		// 현재 애니메이션 인덱스
	float	m_fAccTime;		// 누적 시간(Accumulation Time)
	bool	m_bFinish;			// 애니메이션이 끝났는지 여부
public:
	static void Clear();
	void FinalUpdate();
	virtual void UpdateData() override;
	void Create(TAnimation3DDesc& _tAnimation2DDesc);
	void Create(const vector<TAnimation3DDesc>& _vecAnimation2DDesc);

private:
	void _Play() { m_bFinish = true; }
	void _Stop() { m_bFinish = false; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CAnimation3D);
	CAnimation3D();
	virtual ~CAnimation3D() override;
};