#include "pch.h"
#include "CCore.h"

CCore::CCore() :
	m_hWnd(nullptr)
{
}

CCore::~CCore() {

}

void CCore::Init(HWND _hWnd)
{
	m_hWnd = _hWnd;
}

void CCore::Progress()
{
	// Render, Update
}