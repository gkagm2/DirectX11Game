#include "pch.h"
#include "CCore.h"
#include "CPathManager.h"
#include "CTimeManager.h"
#include "CKeyManager.h"

CCore::CCore() :
	m_hWnd(nullptr)
{
}

CCore::~CCore() {

}

void CCore::Init(HWND _hWnd)
{
	m_hWnd = _hWnd;

	CPathManager::GetInstance()->Init();
	CTimeManager::GetInstance()->Init();
	CKeyManager::GetInstance()->Init();

}

void CCore::Progress()
{
	// Render, Update
}