#include "pch.h"
#include "CMesh.h"
#include "CMeshRenderer.h"
#include "CGraphicsShader.h"

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

	m_pMesh->UpdateData();   // �޽� ����
	m_pShader->UpdateData(); // ����� Graphics Shader ����
	m_pMesh->Render();
}