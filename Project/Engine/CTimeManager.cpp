 #include "pch.h"
#include "CTimeManager.h"
#include "CCore.h"
#include "CFontManager.h"
#include "CKeyManager.h"

CTimeManager::CTimeManager() :
	m_llFrequency{},
	m_llCurFrameCount{},
	m_llPrevFrameCount{},
	m_llPrevCount{},
	m_fDeltaTime(.0f),
	m_iFPS(0),
	m_iFrameCount(0),
	m_timeScale(1.0f),
	m_bIsRender(false),
	m_bFPSShow{true}
{
	m_bIsRender = true;
}
CTimeManager::~CTimeManager() {
}

void CTimeManager::Init()
{
	QueryPerformanceFrequency(&m_llFrequency); // 초당 빈도수
	QueryPerformanceCounter(&m_llPrevCount); // 프레임의 시간을 얻어옴  (현재까지 진행한 카운터)
	m_llCurFrameCount = m_llPrevCount;
	m_llPrevFrameCount = m_llPrevCount;
}

void CTimeManager::Update()
{
	++m_iFrameCount;
	m_llPrevFrameCount = m_llCurFrameCount;
	QueryPerformanceCounter(&m_llCurFrameCount); // 현재 프레임의 시간을 얻어옴.

	if (m_llCurFrameCount.QuadPart - m_llPrevCount.QuadPart > m_llFrequency.QuadPart) {
		QueryPerformanceFrequency(&m_llFrequency); // 초당 빈도수 갱신
		m_llPrevCount = m_llCurFrameCount;
		m_iFPS = m_iFrameCount;

		// Print FPS
		//TCHAR szFPSBuffer[255] = _T("");
		//_stprintf_s(szFPSBuffer, 255, _T("FPS:%d"), m_iFPS);
		//SetWindowText(CCore::GetInstance()->GetWndHandle(), szFPSBuffer);

		TCHAR szBuffer[255] = _T("");
		_stprintf_s(szBuffer,255, _T("FPS : %d"), m_iFPS);
		m_strFPS = szBuffer;

		m_iFrameCount = 0;
	}
	m_fDeltaTime = (float)(m_llCurFrameCount.QuadPart - m_llPrevFrameCount.QuadPart) / (float)(m_llFrequency.QuadPart);  // (현재 틱 - 이전 틱) / 빈도 수(초당 틱)

#ifdef _DEBUG
	if (m_fDeltaTime > 0.166666f)
		m_fDeltaTime = 0.166666f;
#endif

	// 전역상수 세팅
	g_globalConst.fDeltaTime = m_fDeltaTime;
	g_globalConst.fAccTime += m_fDeltaTime;
}

void CTimeManager::Render()
{
	//tstring tile = _T("Hello World 안뇽.");
	if (InputKeyPress(E_Key::F11)) {
		m_bFPSShow = m_bFPSShow == true ? false : true;
	}
	if (m_bFPSShow) {
		CFontManager::GetInstance()->DrawFont(m_strFPS.c_str(), 10, 30, 12, FONT_RGBA(200, 30, 30, 255), FW1_TEXT_FLAG::FW1_LEFT);
	}
}