#pragma once
#include "CObject.h"
#include "CTexture.h"
#include "CAnimator2D.h"

struct TAnimationFrame { // Animation Frame
	Vector2 vLeftTopUV;	// �ִϸ��̼� �̹����� ��-��� ������
	Vector2 vFrameSizeUV;	// ��-��� �������κ��� Width, Height
	Vector2 vOffsetPosUV; //������ ��ǥ
	Vector2 vBaseSizeUV;	// �ִϸ��̼� �̹����� �������� �̵��ص� �����ֱ� ���� ������ �ִ� ������ (UV)
	float fDuration;	// Frame�� ���� �ð�
};

class CAnimator2D;
class CAnimation2D : public CObject
{
private:
	CAnimator2D*			m_pAnimator;
	vector<SharedPtr<CTexture>> m_vecTex;
	SharedPtr<CTexture>		m_pTexture;
	vector<TAnimationFrame> m_vecAnimFrame;

	int		m_iCurFrameIdx;		// ���� �ִϸ��̼� �ε���
	float	m_fAccTime;		// ���� �ð�(Accumulation Time)
	bool	m_bFinish;			// �ִϸ��̼��� �������� ����

private:
	void _SetAnimator(CAnimator2D* _pAnimator2D) { m_pAnimator = _pAnimator2D; }

public:
	static void Clear();
	void FinalUpdate();
	virtual void UpdateData() override;
	void Create(TAnimation2DDesc& _tAnimation2DDesc);
	void Create(const vector<TAnimation2DDesc>& _vecAnimation2DDesc);

	void Reset() {
		m_iCurFrameIdx = 0;
		m_fAccTime = 0.f;
		_Stop();
	}

	bool IsFinished() { return m_bFinish; }

	bool Save(const tstring& _strRelativeFilePath, const wstring& _strFileName);
	bool Load(const tstring& _strRelativeFilePath);

	vector<TAnimationFrame>& GetAnimationFrame() { return m_vecAnimFrame; }
	const TAnimationFrame& GetCurAnimationFrame() { return m_vecAnimFrame[m_iCurFrameIdx]; }
	SharedPtr<CTexture> GetCurTexture() { return m_vecTex[m_iCurFrameIdx]; }
	vector<SharedPtr<CTexture>>& GetTextures() { return m_vecTex; }
	int GetCurFrameIdx() { return m_iCurFrameIdx; }



private:
	void _Play() { m_bFinish = true; }
	void _Stop() { m_bFinish = false; }
	
public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CAnimation2D);
	CAnimation2D();
	virtual ~CAnimation2D() override;

	friend class CAnimator2D;
};