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
	assert(_pTexture.Get() && _T("애니메이션을 표현 할 텍스쳐가 null임"));
	m_pTexture = _pTexture;

	TAnimationFrame tAnimFrm= {};
	
	// Atlas형태의 Texture 좌표를 UV좌표로 변환 후 넣어줌
	Vector2 vUVLeftTopPos = _vLeftTop / m_pTexture->GetDimension();
	tAnimFrm.vFrameSize = _vFrameSize / m_pTexture->GetDimension();
	tAnimFrm.fDuration = _fDuration;
	// 프레임 생성
	for (int i = 0; i < _iFrameCount; ++i) {
		tAnimFrm.vLeftTop = Vector2(vUVLeftTopPos.x + (float)i * tAnimFrm.vFrameSize.x, vUVLeftTopPos.y);
		m_vecAnimFrame.push_back(tAnimFrm);
	}
}