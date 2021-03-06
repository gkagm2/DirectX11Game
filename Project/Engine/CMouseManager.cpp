#include "pch.h"
#include "CMouseManager.h"

CMouseManager::CMouseManager() :
	leftIsDown(false),
	rightIsDown(false),
	mbuttonDown(false),
	x(0),
	y(0),
	m_CurMouseEvent{}
{
}
CMouseManager::~CMouseManager()
{
}

LRESULT CMouseManager::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	//Mouse Messages
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			m_fWheelAccumulator = 1.f * DT;
			_OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			_OnWheelDown(x, y);
			m_fWheelAccumulator = -1.f * DT;
		}
		
		return 0;
	}
	default:
		//return DefWindowProc(hwnd, uMsg, wParam, lParam);
		return 0;
	}
}


void CMouseManager::Update()
{
	//m_fWheelAccumulator = 0.f;
	m_CurMouseEvent = {};
	while(!_EventBufferIsEmpty())
		m_CurMouseEvent = _ReadEvent();

	
}

void CMouseManager::_OnLeftPressed(int x, int y)
{
	leftIsDown = true;
	CMouseEvent me(E_MouseEventType::LPress, x, y);
	eventBuffer.push(me);
}

void CMouseManager::_OnLeftReleased(int x, int y)
{
	leftIsDown = false;
	eventBuffer.push(CMouseEvent(E_MouseEventType::LRelease, x, y));
}

void CMouseManager::_OnRightPressed(int x, int y)
{
	rightIsDown = true;
	eventBuffer.push(CMouseEvent(E_MouseEventType::RPress, x, y));
}

void CMouseManager::_OnRightReleased(int x, int y)
{
	rightIsDown = false;
	eventBuffer.push(CMouseEvent(E_MouseEventType::RRelease, x, y));
}

void CMouseManager::_OnMiddlePressed(int x, int y)
{
	mbuttonDown = true;
	eventBuffer.push(CMouseEvent(E_MouseEventType::MPress, x, y));
}

void CMouseManager::_OnMiddleReleased(int x, int y)
{
	mbuttonDown = false;
	eventBuffer.push(CMouseEvent(E_MouseEventType::MRelease, x, y));
}

void CMouseManager::_OnWheelUp(int x, int y)
{
	eventBuffer.push(CMouseEvent(E_MouseEventType::WheelUp, x, y));
}

void CMouseManager::_OnWheelDown(int x, int y)
{
	eventBuffer.push(CMouseEvent(E_MouseEventType::WheelDown, x, y));
}

void CMouseManager::_OnMouseMove(int x, int y)
{
	x = x;
	y = y;
	eventBuffer.push(CMouseEvent(E_MouseEventType::Move, x, y));
}

bool CMouseManager::_IsLeftDown()
{
	return leftIsDown;
}

bool CMouseManager::_IsMiddleDown()
{
	return mbuttonDown;
}

bool CMouseManager::_IsRightDown()
{
	return rightIsDown;
}

int CMouseManager::GetPosX()
{
	return x;
}

int CMouseManager::GetPosY()
{
	return y;
}

TMousePoint CMouseManager::GetPos()
{
	return{ x, y };
}

bool CMouseManager::_EventBufferIsEmpty()
{
	return eventBuffer.empty();
}

CMouseEvent CMouseManager::_ReadEvent()
{
	if (eventBuffer.empty())
		return CMouseEvent();
	else
	{
		CMouseEvent e = eventBuffer.front(); //Get first event from buffer
		eventBuffer.pop(); //Remove first event from buffer
		return e;
	}
}