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

class CAnimator2D;
class CAnimation2D : public CObject
{
private:
	CAnimator2D*			m_pAnimator;
	SharedPtr<CTexture>		m_pTexture;
	vector<TAnimationFrame> m_vecAnimFrame;

	int		m_iCurFrameIdx;		// ���� �ִϸ��̼� �ε���
	float	m_fAccTime;		// ���� �ð�(Accumulation Time)
	bool	m_bFinish;			// �ִϸ��̼��� �������� ����

private:
	void _SetAnimator(CAnimator2D* _pAnimator2D) { m_pAnimator = _pAnimator2D; }

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
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CAnimation2D);
	CAnimation2D();
	virtual ~CAnimation2D() override;

	friend class CAnimator2D;
};