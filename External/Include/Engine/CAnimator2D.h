#pragma once
#include "CComponent.h"
#include "CTexture.h"
class CAnimation2D;

///////////////////////////////////////////////////////
// 
// Animation 2D Description
struct TAnimation2DDesc {
	tstring		strName;
	SharedPtr<CTexture> pAtlas; // ��Ʋ�� �ؽ���
	Vector2		vLeftTop;		// �ִϸ��̼� �ؽ����� �»�� ��ǥ
	Vector2		vFrameSize;		// �ִϸ��̼� �ؽ����� ũ��
	Vector2		vBaseSize;		// �ִϸ��̼� �ؽ����� offset�� ǥ���� �ִ� ����
	Vector2		vOffsetPos;		// Offset ��ġ
	float		fDuration;		// ���� �ð�
	int			iFrameCount;	// �ִϸ��̼� ����
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
	End,
};
tstring AnimationStateToStr(E_AnimationState _eState);

class CAnimator2D : public CComponent
{
private:
	unordered_map<tstring, CAnimation2D*> m_unmapAnim;
	CAnimation2D* m_pCurAnimation;
	E_AnimationState m_eAnimationState;

	bool m_bPlayOnSceneStart; // Scene �� �����Ҷ� ���õ� �ִϸ��̼� ��� �������

public:
	virtual void Start() override;
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;

	// Animation Name, Animation State
	void Play(const tstring& _strAnimName, E_AnimationState _eAnimationState = E_AnimationState::Loop);

	void LoadAnimation(const tstring& _strRelativeFilePath);


	CAnimation2D* FindAnimation(const tstring& _strName);
	void DeleteAnimation(const tstring& _strName);
	void CreateAnimation(TAnimation2DDesc& _tAnimation2DDesc);
	void CreateAnimation(const vector <TAnimation2DDesc>& _vecAnimation2DDesc); // �ٸ� ������ �ؽ��ĸ� ���� �� ���
	
	static void Clear();

public:
	void SetAnimationState(E_AnimationState _eState) { m_eAnimationState = _eState; }
	E_AnimationState GetAnimationState() { return m_eAnimationState; }
	CAnimation2D* GetCurAnimation() { return m_pCurAnimation; }
	unordered_map<tstring, CAnimation2D*>& GetAnimationList() { m_unmapAnim; }
	void GetAnimationNamesFromList(vector<tstring>& _vecNameList_out);

	void PlayOnSceneStart(bool _bStartFlag) { m_bPlayOnSceneStart = _bStartFlag; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CAnimator2D);
	CAnimator2D();
	CAnimator2D(const CAnimator2D& _origin);
	virtual ~CAnimator2D() override;
};