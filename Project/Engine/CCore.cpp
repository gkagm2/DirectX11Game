#include "pch.h"
#include "CCore.h"
#include "CDevice.h"
#include "CPathManager.h"
#include "CTimeManager.h"
#include "CKeyManager.h"

CCore::CCore() :
	m_hWnd(nullptr)
{
}

CCore::~CCore() {

}

int CCore::Init(HWND _hWnd)
{
	m_hWnd = _hWnd;

	if (FAILED(CDevice::GetInstance()->Init(m_hWnd, true))) {
		MessageBox(nullptr, STR_MSG_FailDeviceInitializing, STR_MSG_InitError, MB_OK);
		return E_FAIL;
	}
		
	CPathManager::GetInstance()->Init();
	CTimeManager::GetInstance()->Init();
	CKeyManager::GetInstance()->Init();

	return S_OK;
}

void CCore::Progress()
{
	// Render, Update
}