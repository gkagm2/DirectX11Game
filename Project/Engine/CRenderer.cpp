#include "pch.h"
#include "CRenderer.h"

#include "CMesh.h"
#include "CResourceManager.h"
#include "CGraphicsShader.h"

#include "CTexture.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CRectTransform.h"
#include "CAnimator2D.h"
CRenderer::CRenderer(E_ComponentType _eType) :
	CComponent(_eType),
	m_pMesh(nullptr),
	m_pMtrl(nullptr)
{
}

CRenderer::~CRenderer()
{
}



void CRenderer::Render()
{
	if (nullptr == m_pMesh || nullptr == m_pMtrl)
		return;

	// ��ġ���� ����
	if (RectTransform())
		RectTransform()->UpdateData();
	else
		Transform()->UpdateData();

	if (Animator2D())
		Animator2D()->UpdateData();
	else
		CAnimator2D::Clear();


	m_pMesh->UpdateData();   // �޽� ����
	m_pMtrl->UpdateData();	 // ���͸��� ����
	m_pMesh->Render();		 // ������

	m_pMtrl->Clear();		 // ���͸��� �������� Clear
}

SharedPtr<CMaterial> CRenderer::GetCloneMaterial()
{
	if (nullptr == m_pMtrl)
		return nullptr;
	m_pMtrl = m_pMtrl->Clone();
	return m_pMtrl;
}

void CRenderer::SetTexSize(const Vector2& _vSize)
{
}

const Vector2& CRenderer::GetTexSize()
{
	SharedPtr<CTexture> pTexture = nullptr;
	m_pMtrl->GetData(E_ShaderParam::Texture_0, pTexture.Get());
	return pTexture->GetDimension();
}

bool CRenderer::SaveToScene(FILE* _pFile)
{
	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pMtrl, _pFile);
	return true;
}

bool CRenderer::LoadFromScene(FILE* _pFile)
{
	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMtrl, _pFile);
	return true;
}