#include "pch.h"
#include "CCollider3D.h"

CCollider3D::CCollider3D() :
	CCollider(E_ComponentType::Collider3D)
{
}

CCollider3D::~CCollider3D()
{
}

void CCollider3D::Render()
{
}

void CCollider3D::FinalUpdate()
{
}

void CCollider3D::UpdateData()
{
}

// TODO (Jang) : 3D 데이터 Save,Load 구현
bool CCollider3D::SaveToScene(FILE* _pFile)
{
	CCollider::SaveToScene(_pFile);
	return true;
}

bool CCollider3D::LoadFromScene(FILE* _pFile)
{
	CCollider::LoadFromScene(_pFile);
	return true;
}