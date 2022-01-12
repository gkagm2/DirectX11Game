#pragma once
#include "CComponent.h"
#include "CTexture.h"
class CAnimation2D;

///////////////////////////////////////////////////////
// 
// Animation 2D Description
struct TAnimation2DDesc {
	tstring		strName;
	SharedPtr<CTexture> pAtlas; // 아틀라스 텍스쳐
	Vector2		vLeftTop;		// 애니메이션 텍스쳐의 좌상단 좌표
	Vector2		vFrameSize;		// 애니메이션 텍스쳐의 크기
	Vector2		vBaseSize;		// 애니메이션 텍스쳐의 offset을 표현할 최대 범위
	Vector2		vOffsetPos;		// Offset 위치
	float		fDuration;		// 지연 시간
	int			iFrameCount;	// 애니메이션 개수
	TAnimation2DDesc() :
		strName(_T("")),
		pAtlas(nullptr),
		vLeftTop(Vector2{ 0.f,0.f }),
		vFrameSize(Vector2{ 0.f,0.f }),
		vBaseSize(Vector2{ 0.f,0.f }),
		vOffsetPos(Vector2{0.f,0.f}),
		fDuration(0.f),
		iFrameCount(0) {}
	TAnimation2DDesc(const tstring& _strName, SharedPtr<CTexture> _pAtlas, const Vector2& _vLeftTop, const Vector2& _vFrameSize, const Vector2& _vBaseSize, float _fDuration, int _iFrameCount, const Vector2& _vOffsetPos = Vector2(0.f,0.f)) :
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

class CAnimator2D : public CComponent
{
private:
	unordered_map<tstring, CAnimation2D*> m_unmapAnim;
	CAnimation2D* m_pCurAnimation;
	E_AnimationState m_eAnimationState;

	bool m_bPlayOnSceneStart; // Scene 이 시작할때 셋팅된 애니메이션 즉시 재생여부
	int m_iCurAnimCurFrameIdx; // state가 Fixed일 경우 고정된 애니메이션 프레임 인덱스를 저장, 불러오기 위함.

public:
	virtual void Start() override;
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;

	// Animation Name, Animation State
	void Play(const tstring& _strAnimName, E_AnimationState _eAnimationState = E_AnimationState::Loop);
	// Current Animation Play
	void Play(E_AnimationState _eAnimationState = E_AnimationState::Loop, bool _bReset = true);

	void LoadAnimation(const tstring& _strRelativeFilePath);


	CAnimation2D* FindAnimation(const tstring& _strName);
	void DeleteAnimation(const tstring& _strName);
	void CreateAnimation(TAnimation2DDesc& _tAnimation2DDesc);
	void CreateAnimation(const vector <TAnimation2DDesc>& _vecAnimation2DDesc); // 다른 종류의 텍스쳐를 설정 할 경우
	
public:
	void SetAnimationState(E_AnimationState _eState) { m_eAnimationState = _eState; }
	E_AnimationState GetAnimationState() { return m_eAnimationState; }
	CAnimation2D* GetCurAnimation() { return m_pCurAnimation; }
	unordered_map<tstring, CAnimation2D*>& GetAnimationList() { return m_unmapAnim; }
	void GetAnimationNamesFromList(vector<tstring>& _vecNameList_out);
	int GetCurAnimationIdxFromNamesList(const vector<tstring>& _vecNameList);
	void SetCurAnimationFrame(int _iIdx);

	void PlayOnSceneStart(bool _bStartFlag) { m_bPlayOnSceneStart = _bStartFlag; }
	bool IsPlayOnSceneStart() { return m_bPlayOnSceneStart; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CAnimator2D);
	CAnimator2D();
	CAnimator2D(const CAnimator2D& _origin);
	virtual ~CAnimator2D() override;
};