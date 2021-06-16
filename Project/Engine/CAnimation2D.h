#pragma once
#include "CObject.h"
#include "CTexture.h"

struct TAnimationFrame { // Animation Frame
	Vector2 vLeftTop;	// �ִϸ��̼� �̹����� ��-��� ������
	Vector2 vFrameSize;	// ��-��� �������κ��� Width, Height
	float fDuration;	// Frame�� ���� �ð�
};

class CAnimation2D : public CObject
{
private:
	SharedPtr<CTexture> m_pTexture;
	vector<TAnimationFrame> m_vecAnimFrame;
	bool m_bFinish;	// �ִϸ��̼��� �������� ����

public:
	void Create(SharedPtr<CTexture>& _pTexture, const Vector2& _vLeftTop, const Vector2& _vFrameSize, float _fDuration, int _iFrameCount);

public:
	CAnimation2D();
	virtual ~CAnimation2D() override;
};