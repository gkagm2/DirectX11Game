#include "pch.h"
#include "CCore.h"
#include "CDevice.h"
#include "CPathManager.h"
#include "CTimeManager.h"
#include "CKeyManager.h"

// Test
#include "RenderTest.h"

CCore::CCore() :
	m_hWnd(nullptr),
	m_vWindowResolution{}
{
}

CCore::~CCore() {
	Render_Test::TestRelease();
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
		
	CPathManager::GetInstance()->Init();
	CTimeManager::GetInstance()->Init();
	CKeyManager::GetInstance()->Init();
	Render_Test::TestInit();

	return S_OK;
}

void CCore::Progress()
{
	CTimeManager::GetInstance()->Update();
	CKeyManager::GetInstance()->Update();

	// Update
	Render_Test::TestUpdate();

	//////////
	// Render
	//////////
	// 1. 타겟 클리어
	CDevice::GetInstance()->ClearTarget();

	// 2. 물체 렌더링
	Render_Test::TestRender();

	// 3. 백버퍼 교체
	CDevice::GetInstance()->Present();
}