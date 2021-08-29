#include "pch.h"
#include "CCanvasRenderer.h"
#include "CResourceManager.h"
CCanvasRenderer::CCanvasRenderer() :
	CRenderer(E_ComponentType::CanvasRenderer)
{
	SetMesh(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh
		));
	SetMaterial(CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_CanvasMtrl));
	assert(GetMesh().Get());
	assert(GetSharedMaterial().Get());
}

CCanvasRenderer::~CCanvasRenderer()
{
}