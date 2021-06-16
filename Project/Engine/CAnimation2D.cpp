#include "pch.h"
#include "CAnimation2D.h"

CAnimation2D::CAnimation2D() :
	m_pTexture(nullptr),
	m_bFinish(false)
{
}

CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::Create(SharedPtr<CTexture>& _pTexture, const Vector2& _vLeftTop, const Vector2& _vFrameSize, float _fDuration, int _iFrameCount)
{
	assert(_pTexture.Get() && _T("�ִϸ��̼��� ǥ�� �� �ؽ��İ� null��"));
	m_pTexture = _pTexture;

	TAnimationFrame tAnimFrm= {};
	
	// Atlas������ Texture ��ǥ�� UV��ǥ�� ��ȯ �� �־���
	Vector2 vUVLeftTopPos = _vLeftTop / m_pTexture->GetDimension();
	tAnimFrm.vFrameSize = _vFrameSize / m_pTexture->GetDimension();
	tAnimFrm.fDuration = _fDuration;
	// ������ ����
	for (int i = 0; i < _iFrameCount; ++i) {
		tAnimFrm.vLeftTop = Vector2(vUVLeftTopPos.x + (float)i * tAnimFrm.vFrameSize.x, vUVLeftTopPos.y);
		m_vecAnimFrame.push_back(tAnimFrm);
	}
}