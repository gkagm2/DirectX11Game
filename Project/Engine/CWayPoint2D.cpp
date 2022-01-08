#include "pch.h"
#include "CWayPoint2D.h"

CWayPoint2D::CWayPoint2D()
{
}

CWayPoint2D::~CWayPoint2D()
{
}

void CWayPoint2D::Init()
{
}

void CWayPoint2D::Update()
{
}

void CWayPoint2D::AddWayPoint(const Vector2& _vPoint)
{
	m_vecWays.push_back(_vPoint);
}

void CWayPoint2D::DeleteWayPoint(int _iIdx)
{
	if (_iIdx < 0 || _iIdx >= m_vecWays.size())
		return;
	auto iter = m_vecWays.begin();
	m_vecWays.erase(iter + _iIdx);
}