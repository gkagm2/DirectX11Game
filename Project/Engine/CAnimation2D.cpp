#include "pch.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTimeManager.h"
#include "CAnimation2D.h"
#include "CTexture.h"
#include "Ptr.h"

CAnimation2D::CAnimation2D() :
	m_pTexture(nullptr),
	m_iCurFrameIdx(0),
	m_fAccTime(0.f),
	m_bFinish(false)
{
}

CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::LateUpdate()
{
	if (m_bFinish)
		return;

	m_fAccTime += DeltaTime;
	while (m_fAccTime > m_vecAnimFrame[m_iCurFrameIdx].fDuration) {
		// 누적시간이 현재 프레임 유지시간을 초과한 양을 구함
		m_fAccTime = m_fAccTime - m_vecAnimFrame[m_iCurFrameIdx].fDuration;
		++m_iCurFrameIdx;

		// 최대 프레임에 도달하게 되면, 애니메이션 재생을 완료상태로 둠
		if (m_iCurFrameIdx == m_vecAnimFrame.size()) {
			--m_iCurFrameIdx;
			m_bFinish = true;
			break;
		}
	}
}

void CAnimation2D::UpdateData()
{
	static const UINT iRegisterNum = 50;
	m_pTexture->UpdateData(E_ShaderStage::Pixel, iRegisterNum);

	static const CConstBuffer* pBuffer = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Animation2D_Data);

	TAnimation2DData tData = {
		m_vecAnimFrame[m_iCurFrameIdx].vLeftTop,
		m_vecAnimFrame[m_iCurFrameIdx].vFrameSize,
		m_vecAnimFrame[m_iCurFrameIdx].vOffsetPos,
		m_vecAnimFrame[m_iCurFrameIdx].vBaseSize,
		true // Is Animating 2D
	};
	pBuffer->SetData(&tData);
	pBuffer->UpdateData();
}

void CAnimation2D::Create(TAnimation2DDesc& _tAnimation2DDesc)
{
	//assert(_tAnimation2DDesc.pAtlas.Get() && _T("애니메이션을 표현 할 텍스쳐가 null임"));
	m_pTexture = _tAnimation2DDesc.pAtlas;

	TAnimationFrame tAnimFrm= {};
	
	// Atlas형태의 Texture 좌표를 UV좌표로 변환 후 넣어줌
	Vector2 vUVLeftTopPos = _tAnimation2DDesc.vLeftTop / m_pTexture->GetDimension();
	tAnimFrm.vFrameSize = _tAnimation2DDesc.vFrameSize / m_pTexture->GetDimension();
	tAnimFrm.vBaseSize = _tAnimation2DDesc.vBaseSize / m_pTexture->GetDimension();
	tAnimFrm.fDuration = _tAnimation2DDesc.fDuration;
	// 프레임 생성
	for (int i = 0; i < _tAnimation2DDesc.iFrameCount; ++i) {
		tAnimFrm.vLeftTop = Vector2(vUVLeftTopPos.x + (float)i * tAnimFrm.vFrameSize.x, vUVLeftTopPos.y);
		m_vecAnimFrame.push_back(tAnimFrm);
	}
}

void CAnimation2D::Save(const tstring& _strRelativeFilePath, const wstring& _strFileName)
{
}

void CAnimation2D::Load(const tstring& _strRelativeFilePath)
{
}