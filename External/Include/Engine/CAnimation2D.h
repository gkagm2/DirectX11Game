#pragma once
#include "CObject.h"
#include "CTexture.h"
#include "CAnimator2D.h"

struct TAnimationFrame { // Animation Frame
	Vector2 vLeftTop;	// �ִϸ��̼� �̹����� ��-��� ������
	Vector2 vFrameSize;	// ��-��� �������κ��� Width, Height
	Vector2 vOffsetPos; //������ ��ǥ
	Vector2 vBaseSize;	// �ִϸ��̼� �̹����� �������� �̵��ص� �����ֱ� ���� ������ �ִ� ������ (UV)
	float fDuration;	// Frame�� ���� �ð�
};

class CAnimation2D : public CObject
{
private:
	SharedPtr<CTexture> m_pTexture;
	vector<TAnimationFrame> m_vecAnimFrame;

	int m_iCurFrameIdx;		// ���� �ִϸ��̼� �ε���
	float m_fAccTime;		// ���� �ð�(Accumulation Time)
	bool m_bFinish;			// �ִϸ��̼��� �������� ����

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

	vector<TAnimationFrame>& GetAnimationFrame() { return m_vecAnimFrame; }

public:
	CAnimation2D();
	virtual ~CAnimation2D() override;

	friend class CAnimator2D;
};