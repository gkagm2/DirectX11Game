#include "pch.h"
#include "CCore.h"
#include "CDevice.h"
#include "CPathManager.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CEventManager.h"
#include "CRenderManager.h"
#include "CMouseManager.h"
#include "CConfigurationManager.h"

CCore::CCore() :
	m_hWnd(nullptr),
	m_vWindowResolution{}
{
#ifdef _DEBUG
	AllocConsole(); // 能贾芒 积己
	//_tcprintf(_T("Test")); // Using
#endif
}

CCore::~CCore() {

#ifdef _DEBUG
#endif
}

int CCore::Init(HWND _hOutputWnd, const Vector2& _vWindowResolution, const Vector2& _vRenderResolution)
{
	m_hWnd = _hOutputWnd;
	m_vWindowResolution = _vWindowResolution;

	RECT rt = { 0, 0, (int)m_vWindowResolution.x , (int)m_vWindowResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

	if (FAILED(CDevice::GetInstance()->Init(m_hWnd, _vRenderResolution, true))) {
		MessageBox(nullptr, STR_MSG_FailDeviceInitializing, STR_MSG_InitError, MB_OK);
		return E_FAIL;
	}
	CConfigurationManager::GetInstance()->Init();
	CPathManager::GetInstance()->Init();
	CTimeManager::GetInstance()->Init();
	CKeyManager::GetInstance()->Init();
	CRenderManager::GetInstance()->Init(); 
	CResourceManager::GetInstance()->Init();
	CSceneManager::GetInstance()->Init();

	return S_OK;
}

void CCore::Progress()
{
	// Update
	CTimeManager::GetInstance()->Update();
	CKeyManager::GetInstance()->Update();
	CMouseManager::GetInstance()->Update();
	CResourceManager::GetInstance()->Update();
	CSceneManager::GetInstance()->Progress();

	CRenderManager::GetInstance()->Render();

	// Event 荐青
	CEventManager::GetInstance()->Update();
}