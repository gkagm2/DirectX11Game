#include "pch.h"
#include "CMesh.h"
#include "CMeshRenderer.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CAnimator2D.h"
#include "CMeshRenderer.h"

CMeshRenderer::CMeshRenderer() :
	CRenderer(E_ComponentType::MeshRenderer),
	m_pMesh(nullptr),
	m_pMtrl(nullptr)
{
}

CMeshRenderer::~CMeshRenderer()
{
}

void CMeshRenderer::Render()
{
	if(nullptr == m_pMesh || nullptr == m_pMtrl)
		return;

	Transform()->UpdateData(); // ��ġ���� ����
	
	if (Animator2D())
		Animator2D()->UpdateData();
	else
		CAnimator2D::Clear();


	m_pMesh->UpdateData();   // �޽� ����
	m_pMtrl->UpdateData();	 // ���͸��� ����
	m_pMesh->Render();		 // ������

	m_pMtrl->Clear();		 // ���͸��� �������� Clear
}

SharedPtr<CMaterial> CMeshRenderer::GetCloneMaterial()
{
	if (nullptr == m_pMtrl)
		return nullptr;
	m_pMtrl = m_pMtrl->Clone();
	return m_pMtrl;
}