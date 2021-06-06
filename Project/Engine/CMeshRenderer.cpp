#include "pch.h"
#include "CMesh.h"
#include "CMeshRenderer.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

// TODO :: Test code
#include "CGameObject.h"
#include "CTransform.h"


CMeshRenderer::CMeshRenderer() :
	CRenderer(E_ComponentType::MeshRenderer),
	m_pMesh(nullptr),
	m_pShader(nullptr)
{
}

CMeshRenderer::~CMeshRenderer()
{
}

void CMeshRenderer::Render()
{
	if(nullptr == m_pMesh || nullptr == m_pShader)
		return;
	GetGameObject()->GetComponent<CTransform>()->UpdateData();

	m_pMesh->UpdateData();   // 메쉬 세팅

	if (nullptr != m_pTexture)
		m_pTexture->UpdateData(E_ShaderStage::Pixel, 0); // 텍스쳐 세팅

	m_pShader->UpdateData(); // 사용할 Graphics Shader 세팅
	m_pMesh->Render();
}