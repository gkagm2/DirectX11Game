#pragma once
#include "CObject.h"

struct TAnimation3DFrame {
	Vector2 vLeftTopUV;	// �ִϸ��̼� �̹����� ��-��� ������
	Vector2 vFrameSizeUV;	// ��-��� �������κ��� Width, Height
	Vector2 vOffsetPosUV; //������ ��ǥ
	Vector2 vBaseSizeUV;	// �ִϸ��̼� �̹����� �������� �̵��ص� �����ֱ� ���� ������ �ִ� ������ (UV)
	float fDuration;	// Frame�� ���� �ð�
};

///////////////////////////////////////////////////////
// 
// Animation 2D Description
struct TAnimation3DDesc {
	tstring		strName;
	SharedPtr<CTexture> pAtlas; // ��Ʋ�� �ؽ���
	Vector2		vLeftTop;		// �ִϸ��̼� �ؽ����� �»�� ��ǥ
	Vector2		vFrameSize;		// �ִϸ��̼� �ؽ����� ũ��
	Vector2		vBaseSize;		// �ִϸ��̼� �ؽ����� offset�� ǥ���� �ִ� ����
	Vector2		vOffsetPos;		// Offset ��ġ
	float		fDuration;		// ���� �ð�
	int			iFrameCount;	// �ִϸ��̼� ����
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
	tstring m_strAnimPath; // ������ ���� �����

	int		m_iCurFrameIdx;		// ���� �ִϸ��̼� �ε���
	float	m_fAccTime;		// ���� �ð�(Accumulation Time)
	bool	m_bFinish;			// �ִϸ��̼��� �������� ����
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