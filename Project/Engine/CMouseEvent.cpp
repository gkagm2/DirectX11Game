#include "pch.h"
#include "CMouseEvent.h"

CMouseEvent::CMouseEvent() :
	m_eEventType(E_MouseEventType::Invalid),
	m_iX(0),
	m_iY(0)
{
}

CMouseEvent::CMouseEvent(const E_MouseEventType type, const int x, const int y) :
	m_eEventType(type),
	m_iX(x),
	m_iY(y)
{
}

CMouseEvent::~CMouseEvent()
{
}