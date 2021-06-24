#include "pch.h"
#include "CCollider2DRect.h"
#include "CResourceManager.h"

CCollider2DRect::CCollider2DRect()
{
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectLineMesh);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_Collider2DMtrl);
	SetMesh(pMesh);
	SetMaterial(pMtrl);
}

CCollider2DRect::~CCollider2DRect()
{
}

void CCollider2DRect::OnCollisionEnter(CCollider2D* _pOther)
{
	CCollider2D::OnCollisionEnter(_pOther);
}

void CCollider2DRect::OnCollisionStay(CCollider2D* _pOther)
{
	CCollider2D::OnCollisionStay(_pOther);
}

void CCollider2DRect::OnCollisionExit(CCollider2D* _pOther)
{
	CCollider2D::OnCollisionExit(_pOther);
}