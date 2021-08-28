#include "pch.h"
#include "CMeshRenderer.h"
#include "CResourceManager.h"

CMeshRenderer::CMeshRenderer() :
	CRenderer(E_ComponentType::MeshRenderer)
{
	SetMesh(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh));
	SetMaterial(CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl));
}

CMeshRenderer::~CMeshRenderer()
{
}