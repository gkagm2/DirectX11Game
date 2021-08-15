#include "pch.h"
#include "CCamera2D.h"
#include "CKeyManager.h"

CCamera2D::CCamera2D()
{
}

CCamera2D::~CCamera2D()
{
}

const Vector2& CCamera2D::GetScreenToWorldPosition()
{
	//MousePosition
	return Vector2(0.f, 0.f);
}