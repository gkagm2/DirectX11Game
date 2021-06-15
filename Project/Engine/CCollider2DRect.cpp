#include "pch.h"
#include "CCollider2DRect.h"
#include "CResourceManager.h"

CCollider2DRect::CCollider2DRect()
{
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdMtrlAlphaBlend_Coverage);
	SetMesh(pMesh);
	SetMaterial(pMtrl);
}

CCollider2DRect::~CCollider2DRect()
{
}