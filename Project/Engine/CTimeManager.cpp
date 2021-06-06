 #include "pch.h"
#include "CTimeManager.h"
#include "CCore.h"

CTimeManager::CTimeManager() :
	m_llFrequency{},
	m_llCurFrameCount{},
	m_llPrevFrameCount{},
	m_llPrevCount{},
	m_fDeltaTime(.0f),
	m_iFPS(0),
	m_iFrameCount(0),
	m_timeScale(1.0f),
	m_bIsRender(false)
{
	m_bIsRender = true;
}
CTimeManager::~CTimeManager() {
}

void CTimeManager::Init()
{
	QueryPerformanceFrequency(&m_llFrequency); // �ʴ� �󵵼�
	QueryPerformanceCounter(&m_llPrevCount); // �������� �ð��� ����  (������� ������ ī����)
	m_llCurFrameCount = m_llPrevCount;
	m_llPrevFrameCount = m_llPrevCount;
}

void CTimeManager::Update()
{
	++m_iFrameCount;
	m_llPrevFrameCount = m_llCurFrameCount;
	QueryPerformanceCounter(&m_llCurFrameCount); // ���� �������� �ð��� ����.

	if (m_llCurFrameCount.QuadPart - m_llPrevCount.QuadPart > m_llFrequency.QuadPart) {
		QueryPerformanceFrequency(&m_llFrequency); // �ʴ� �󵵼� ����
		m_llPrevCount = m_llCurFrameCount;
		m_iFPS = m_iFrameCount;

		// Print FPS
		TCHAR szFPSBuffer[255] = _T("");
		
		_stprintf_s(szFPSBuffer, 255, _T("FPS:%d"), m_iFPS);
		SetWindowText(CCore::GetInstance()->GetWndHandle(), szFPSBuffer);

		m_iFrameCount = 0;
	}
	m_fDeltaTime = (float)(m_llCurFrameCount.QuadPart - m_llPrevFrameCount.QuadPart) / (float)(m_llFrequency.QuadPart);  // (���� ƽ - ���� ƽ) / �� ��(�ʴ� ƽ)

#ifdef _DEBUG
	if (m_fDeltaTime > 0.166666f)
		m_fDeltaTime = 0.166666f;
#endif
}

void CTimeManager::Render(HDC _hdc)
{
	if (!m_bIsRender)
		return;
}