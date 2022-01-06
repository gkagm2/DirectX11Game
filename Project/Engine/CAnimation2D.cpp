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
	m_pAnimator(nullptr),
	m_pTexture(nullptr),
	m_iCurFrameIdx(0),
	m_fAccTime(0.f),
	m_bFinish(false)
{
}

CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::FinalUpdate()
{
	// (jang) :  �����Ҷ��� Animation�� ���ư����� ����� ���Ͽ� FinalUpdate�� �ƴ� LateUpdate���� ������.
	if (IsFinished())
		return;

	m_fAccTime += DT;
	while (m_fAccTime > m_vecAnimFrame[m_iCurFrameIdx].fDuration) {
		// �����ð��� ���� ������ �����ð��� �ʰ��� ���� ����
		m_fAccTime = m_fAccTime - m_vecAnimFrame[m_iCurFrameIdx].fDuration;
		++m_iCurFrameIdx;

		// �ִ� �����ӿ� �����ϰ� �Ǹ�, �ִϸ��̼� ����� �Ϸ���·� ��
		if (m_iCurFrameIdx == m_vecAnimFrame.size()) {
			--m_iCurFrameIdx;
			_Play();
			break;
		}
	}
}

void CAnimation2D::UpdateData()
{
	static const UINT iRegisterNum = 50;
	m_vecTex[m_iCurFrameIdx]->UpdateData(E_ShaderStage::Pixel, iRegisterNum);

	static const CConstBuffer* pBuffer = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Animation2D_Data);

	TAnimation2DData tData = {
		m_vecAnimFrame[m_iCurFrameIdx].vLeftTopUV,
		m_vecAnimFrame[m_iCurFrameIdx].vFrameSizeUV,
		m_vecAnimFrame[m_iCurFrameIdx].vOffsetPosUV,
		m_vecAnimFrame[m_iCurFrameIdx].vBaseSizeUV,
		true // Is Animating 2D
	};
	pBuffer->SetData(&tData);
	pBuffer->UpdateData();
}

void CAnimation2D::Clear()
{
	static const CConstBuffer* pBuffer = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Animation2D_Data);
	// FIXED(Jang) : ���� ����
	/*TAnimation2DData tData = {};
	pBuffer->SetData(&tData);
	pBuffer->UpdateData();*/
	pBuffer->Clear();
}

void CAnimation2D::Create(TAnimation2DDesc& _tAnimation2DDesc)
{
	//assert(_tAnimation2DDesc.pAtlas.Get() && _T("�ִϸ��̼��� ǥ�� �� �ؽ��İ� null��"));
	m_vecTex.clear();
	for (int i = 0; i < _tAnimation2DDesc.iFrameCount; ++i)
		m_vecTex.push_back(_tAnimation2DDesc.pAtlas);

	TAnimationFrame tAnimFrm= {};
	
	// Atlas������ Texture ��ǥ�� UV��ǥ�� ��ȯ �� �־���
	Vector2 vUVLeftTopPos = _tAnimation2DDesc.vLeftTop / _tAnimation2DDesc.pAtlas.Get()->GetResolution();
	tAnimFrm.vFrameSizeUV = _tAnimation2DDesc.vFrameSize / _tAnimation2DDesc.pAtlas.Get()->GetResolution();
	tAnimFrm.vBaseSizeUV = _tAnimation2DDesc.vBaseSize / _tAnimation2DDesc.pAtlas.Get()->GetResolution();
	tAnimFrm.fDuration = _tAnimation2DDesc.fDuration;
	tAnimFrm.vOffsetPosUV = _tAnimation2DDesc.vOffsetPos / _tAnimation2DDesc.pAtlas.Get()->GetResolution();
	// ������ ����
	for (int i = 0; i < _tAnimation2DDesc.iFrameCount; ++i) {
		tAnimFrm.vLeftTopUV = Vector2(vUVLeftTopPos.x + (float)i * tAnimFrm.vFrameSizeUV.x, vUVLeftTopPos.y);
		m_vecAnimFrame.push_back(tAnimFrm);
	}
}

void CAnimation2D::Create(const vector<TAnimation2DDesc>& _vecAnimation2DDesc)
{
	for (int i = 0; i < _vecAnimation2DDesc.size(); ++i) {
		TAnimationFrame tAnimFrm = {};
		// Atlas������ Texture ��ǥ�� UV��ǥ�� ��ȯ �� �־���
		tAnimFrm.vLeftTopUV = _vecAnimation2DDesc[i].vLeftTop / _vecAnimation2DDesc[i].pAtlas.Get()->GetResolution();
		tAnimFrm.vBaseSizeUV = _vecAnimation2DDesc[i].vBaseSize / _vecAnimation2DDesc[i].pAtlas.Get()->GetResolution();
		tAnimFrm.fDuration = _vecAnimation2DDesc[i].fDuration;
		tAnimFrm.vOffsetPosUV = _vecAnimation2DDesc[i].vOffsetPos / _vecAnimation2DDesc[i].pAtlas.Get()->GetResolution();
		tAnimFrm.vFrameSizeUV = _vecAnimation2DDesc[i].vFrameSize / _vecAnimation2DDesc[i].pAtlas.Get()->GetResolution();

		m_vecAnimFrame.push_back(tAnimFrm);
		m_vecTex.push_back(_vecAnimation2DDesc[i].pAtlas);
	}
}

bool CAnimation2D::Save(const tstring& _strRelativeFilePath, const wstring& _strFileName)
{
	tstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strRelativeFilePath + _strFileName;

	FILE* pFile = nullptr;
	_tfopen_s(&pFile, strFilePath.c_str(), _T("w"));
	assert(pFile);
	if (nullptr == pFile)
		return false;

	// fwpritnf
	_ftprintf(pFile, _T("Animation_Name\n"));
	_ftprintf(pFile, _T("%s"),GetName().c_str());
	_ftprintf(pFile, _T("\n"));
	_ftprintf(pFile, _T("Frame_Count\n"));
	_ftprintf(pFile, _T("%d\n"), (int)m_vecAnimFrame.size());

	for (int i = 0; i < m_vecAnimFrame.size(); ++i) {
		_ftprintf(pFile, _T("Left_Top\n"));
		_ftprintf(pFile, _T("%f %f\n"), m_vecAnimFrame[i].vLeftTopUV.x, m_vecAnimFrame[i].vLeftTopUV.y);

		_ftprintf(pFile, _T("Frame_Size\n"));
		_ftprintf(pFile, _T("%f %f\n"), m_vecAnimFrame[i].vFrameSizeUV.x, m_vecAnimFrame[i].vFrameSizeUV.y);

		_ftprintf(pFile, _T("Offset\n"));
		_ftprintf(pFile, _T("%f %f\n"), m_vecAnimFrame[i].vOffsetPosUV.x, m_vecAnimFrame[i].vOffsetPosUV.y);

		_ftprintf(pFile, _T("Base_Size\n"));
		_ftprintf(pFile, _T("%f %f\n"), m_vecAnimFrame[i].vBaseSizeUV.x, m_vecAnimFrame[i].vBaseSizeUV.y);

		_ftprintf(pFile, _T("Duration\n"));
		_ftprintf(pFile, _T("%f\n"), m_vecAnimFrame[i].fDuration);
		_ftprintf(pFile, _T("\n"));
	}

	//// �����ϰ� �ִ� �ؽ��� ����
	//_ftprintf(pFile, _T("Textrue_Name\n"));
	//_ftprintf(pFile, m_pTexture->GetKey().c_str());
	//_ftprintf(pFile, _T("\n"));

	//_ftprintf(pFile, _T("Texture_Path\n"));
	//_ftprintf(pFile, m_pTexture->GetRelativePath().c_str());
	//_ftprintf(pFile, _T("\n"));

	for (size_t i = 0; i < m_vecTex.size(); ++i) {
		_ftprintf(pFile, _T("Texture_Name %d\n"), (int)i);
		_ftprintf(pFile, _T("%s"), m_vecTex[i]->GetKey().c_str());
		_ftprintf(pFile, _T("\n"));

		_ftprintf(pFile, _T("Texture_Path %d\n"), (int)i);
		_ftprintf(pFile, _T("%s"), m_vecTex[i]->GetRelativePath().c_str());
		_ftprintf(pFile, _T("\n"));
	}

	fclose(pFile);
	return true;
}

bool CAnimation2D::Load(const tstring& _strRelativeFilePath)
{
	tstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strRelativeFilePath;

	int const iBufferSize = 1024;
	TCHAR szBuffer[iBufferSize] = {};

	FILE* pFile = nullptr;
	_tfopen_s(&pFile, strFilePath.c_str(), _T("r"));
	assert(pFile);
	if (nullptr == pFile)
		return false;

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
		_ftscanf_s(pFile, _T("%f%f"), &tAnimFrame.vLeftTopUV.x, &tAnimFrame.vLeftTopUV.y);

		// Frame Size
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		_ftscanf_s(pFile, _T("%f%f"), &tAnimFrame.vFrameSizeUV.x, &tAnimFrame.vFrameSizeUV.y);

		// Offset
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		_ftscanf_s(pFile, _T("%f%f"), &tAnimFrame.vOffsetPosUV.x, &tAnimFrame.vOffsetPosUV.y);

		// Base Sizes
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		_ftscanf_s(pFile, _T("%f%f"), &tAnimFrame.vBaseSizeUV.x, &tAnimFrame.vBaseSizeUV.y);

		// Durations
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		_ftscanf_s(pFile, _T("%f"), &tAnimFrame.fDuration);
		m_vecAnimFrame.push_back(tAnimFrame);
	}

	// �����ϰ� �ִ� �ؽ��� ����

	//// �ؽ��� �̸�
	//_ftscanf_s(pFile, _T("%s"), szBuffer,iBufferSize);
	//_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
	//m_pTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(szBuffer);
	//tstring strKey = szBuffer;

	//// �ؽ��� ���
	//_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
	//_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
	//if (nullptr == m_pTexture)
	//	m_pTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(strKey, szBuffer);

	m_vecTex.clear();
	for (size_t i = 0; i < iFrameCount; ++i) {
		// �ؽ��� �̸�
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		SharedPtr<CTexture> pTex = nullptr;
		pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(szBuffer);
		m_vecTex.push_back(pTex);
		tstring strKey = szBuffer;

		// �ؽ��� ���
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		_ftscanf_s(pFile, _T("%s"), szBuffer, iBufferSize);
		if (nullptr == m_vecTex[i]) {
			m_vecTex[i] = CResourceManager::GetInstance()->LoadRes<CTexture>(strKey, szBuffer);
		}
	}

	fclose(pFile);
	return true;
}

bool CAnimation2D::SaveToScene(FILE* _pFile)
{
	CObject::SaveToScene(_pFile);
	//SaveResourceToFile(m_pTexture, _pFile);
	UINT iTexCnt = (UINT)m_vecTex.size();
	FWrite(iTexCnt, _pFile);
	for (UINT i = 0; i < iTexCnt; ++i)
		SaveResourceToFile(m_vecTex[i], _pFile);


	UINT iFrameCount = (UINT)m_vecAnimFrame.size();
	FWrite(iFrameCount, _pFile);
	for (UINT i = 0; i < iFrameCount; ++i)
		FWrite(m_vecAnimFrame[i], _pFile);

	FWrite(m_iCurFrameIdx, _pFile);
	SaveStringToFile(m_strAnimPath, _pFile);

	return true;
}

bool CAnimation2D::LoadFromScene(FILE* _pFile)
{
	CObject::LoadFromScene(_pFile);
	//LoadResourceFromFile(m_pTexture, _pFile);
	UINT iTexCnt = 0;
	FRead(iTexCnt, _pFile);

	m_vecTex.clear();
	for (UINT i = 0; i < iTexCnt; ++i) {
		SharedPtr<CTexture> pTex = nullptr;
		LoadResourceFromFile(pTex, _pFile);
		m_vecTex.push_back(pTex);
	}

	UINT iFrameCount = 0;
	FRead(iFrameCount, _pFile);
	m_vecAnimFrame.clear();
	for (UINT i = 0; i < iFrameCount; ++i) {
		TAnimationFrame tAnimFrame = {};
		FRead(tAnimFrame, _pFile);
		m_vecAnimFrame.push_back(tAnimFrame);
	}

	FRead(m_iCurFrameIdx, _pFile);
	LoadStringFromFile(m_strAnimPath, _pFile);

	return true;
}