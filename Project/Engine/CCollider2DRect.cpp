#include "pch.h"
#include "CCollider2DRect.h"
#include "CResourceManager.h"

CCollider2DRect::CCollider2DRect()
{
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectLineMesh);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_Collider2DMaterial);
	SetMesh(pMesh);
	SetMaterial(pMtrl);
}

CCollider2DRect::~CCollider2DRect()
{
}

void CCollider2DRect::OnCollisionEnter(CCollider2D* pOther)
{
}

void CCollider2DRect::OnCollisionStay(CCollider2D* pOther)
{
}

void CCollider2DRect::OnCollisionExit(CCollider2D* pOther)
{
}