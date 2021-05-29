#include "pch.h"
#include "CMesh.h"
#include "CMeshRenderer.h"
#include "CGraphicsShader.h"


CMeshRenderer::CMeshRenderer() :
	CRenderer(E_ComponentType::meshRenderer),
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

	m_pMesh->UpdateData();   // �޽� ����
	m_pShader->UpdateData(); // ����� Graphics Shader ����
	m_pMesh->Render();
}