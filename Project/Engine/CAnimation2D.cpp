#include "pch.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTimeManager.h"
#include "CPathManager.h"
#include "CResourceManager.h"
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
	tstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strRelativeFilePath + _strFileName;

	FILE* pFile = nullptr;
	_tfopen_s(&pFile, strFilePath.c_str(), _T("w"));
	assert(pFile);

	// fwpritnf
	_ftprintf(pFile, _T("Animation_Name\n"));
	_ftprintf(pFile, GetName().c_str());
	_ftprintf(pFile, _T("\n"));
	_ftprintf(pFile, _T("Frame_Count\n"));
	_ftprintf(pFile, _T("%d\n"), (int)m_vecAnimFrame.size());

	for (int i = 0; i < m_vecAnimFrame.size(); ++i) {
		_ftprintf(pFile, _T("Left_Top\n"));
		_ftprintf(pFile, _T("%f %f\n"), m_vecAnimFrame[i].vLeftTop.x, m_vecAnimFrame[i].vLeftTop.y);

		_ftprintf(pFile, _T("Frame_Size\n"));
		_ftprintf(pFile, _T("%f %f\n"), m_vecAnimFrame[i].vFrameSize.x, m_vecAnimFrame[i].vFrameSize.y);

		_ftprintf(pFile, _T("Offset\n"));
		_ftprintf(pFile, _T("%f %f\n"), m_vecAnimFrame[i].vOffsetPos.x, m_vecAnimFrame[i].vOffsetPos.y);

		_ftprintf(pFile, _T("Base_Size\n"));
		_ftprintf(pFile, _T("%f %f\n"), m_vecAnimFrame[i].vBaseSize.x, m_vecAnimFrame[i].vBaseSize.y);

		_ftprintf(pFile, _T("Duration\n"));
		_ftprintf(pFile, _T("%f\n"), m_vecAnimFrame[i].fDuration);
		_ftprintf(pFile, _T("\n"));
	}
	 
	// 참조하고 있는 텍스쳐 정보
	_ftprintf(pFile, _T("Textrue_Name\n"));
	_ftprintf(pFile, m_pTexture->GetKey().c_str());
	_ftprintf(pFile, _T("\n"));

	_ftprintf(pFile, _T("Texture_Path\n"));
	_ftprintf(pFile, m_pTexture->GetRelativePath().c_str());
	_ftprintf(pFile, _T("\n"));

	fclose(pFile);
}

void CAnimation2D::Load(const tstring& _strRelativeFilePath)
{
	tstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strRelativeFilePath;

	int const iBufferSize = 1024;
	TCHAR szBuffer[iBufferSize] = {};

	FILE* pFile = nullptr;
	_tfopen_s(&pFile, strFilePath.c_str(), _T("r"));
	assert(pFile);

	// fwscanf_s
	_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);

	// Animation Name
	_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
	SetName(szBuffer);

	//Frame Count
	_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
	int iFrameCount = 0;
	_ftscanf_s(pFile, _T("%d"), &iFrameCount);

	TAnimationFrame tAnimFrame = {};
	for (int i = 0; i < iFrameCount; ++i) {
		// Left Top
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		_ftscanf_s(pFile, _T("%f%f"), &tAnimFrame.vLeftTop.x, &tAnimFrame.vLeftTop.y);

		// Frame Size
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		_ftscanf_s(pFile, _T("%f%f"), &tAnimFrame.vFrameSize.x, &tAnimFrame.vFrameSize.y);

		// Offset
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		_ftscanf_s(pFile, _T("%f%f"), &tAnimFrame.vOffsetPos.x, &tAnimFrame.vOffsetPos.y);

		// Base Sizes
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		_ftscanf_s(pFile, _T("%f%f"), &tAnimFrame.vBaseSize.x, &tAnimFrame.vBaseSize.y);

		// Durations
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		_ftscanf_s(pFile, _T("%f"), &tAnimFrame.fDuration);
		m_vecAnimFrame.push_back(tAnimFrame);
	}

	// 참조하고 있는 텍스쳐 정보

	// 텍스쳐 이름
	_ftscanf_s(pFile, _T("%s"), szBuffer,iBufferSize);
	_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
	m_pTexture = CResourceManager::GetInstance()->FindRes<CTexture>(szBuffer);
	wstring strKey = szBuffer;

	// 텍스쳐 경로
	_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
	_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
	if (nullptr == m_pTexture)
		m_pTexture = CResourceManager::GetInstance()->Load<CTexture>(strKey, szBuffer);

	fclose(pFile);
}