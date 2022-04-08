#include "pch.h"
#include "CMeshRenderer.h"
#include "CResourceManager.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CTexture.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CRectTransform.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"


CMeshRenderer::CMeshRenderer() :
	CRenderer(E_ComponentType::MeshRenderer),
	m_pMesh(nullptr),
	m_iColor{ COLOR_RGBA(0,0,0,255) }
{
	SetMesh(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh));
	m_vecMtrls.resize(1);
	SetMaterial(CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl), 0);
}

CMeshRenderer::~CMeshRenderer()
{
	for (size_t i = 0; i < m_vecMtrls.size(); ++i) {
		SAFE_DELETE_PTR(m_vecMtrls[i].pClonedMtrl);
	}
}

void CMeshRenderer::Render()
{
	if (nullptr == m_pMesh || m_vecMtrls.empty())
		return;

	for (size_t i = 0; i < m_vecMtrls.size(); ++i) {
		// ��ġ���� ����
		if (Transform())
			Transform()->UpdateData();

		if (Animator2D())
			Animator2D()->UpdateData();
		else
			CAnimation2D::Clear();

		m_vecMtrls[i].pMtrl->UpdateData();	 // ���͸��� ����
		m_pMesh->Render(i);		 // ������
		m_vecMtrls[i].pMtrl->Clear(); // ���͸��� �������� Clear
	}
}

void CMeshRenderer::SetMesh(SharedPtr<CMesh> _pMesh)
{
	m_pMesh = _pMesh;

	int iResizeCnt = 1;

	if (nullptr != m_pMesh)
		iResizeCnt = m_pMesh->GetSubsetCount(); // index buffer count

	vector<tMtrlSet> vec;
	vec.resize(iResizeCnt);
	m_vecMtrls.swap(vec);
}

void CMeshRenderer::SetMaterial(SharedPtr<CMaterial> _pMtrl, UINT _iIdx)
{
	if (m_vecMtrls.size() <= _iIdx) {
		assert(nullptr);
		return;
	}

	m_vecMtrls[_iIdx].pSharedMtrl = _pMtrl;
	m_vecMtrls[_iIdx].pMtrl = _pMtrl;

	if (nullptr != m_vecMtrls[_iIdx].pClonedMtrl) {
		SAFE_DELETE_PTR(m_vecMtrls[_iIdx].pClonedMtrl);
	}
}

SharedPtr<CMaterial> CMeshRenderer::GetCloneMaterial(UINT _iIdx)
{
	if (m_vecMtrls.size() <= _iIdx) {
		assert(nullptr);
		return nullptr;
	}
	if (nullptr != m_vecMtrls[_iIdx].pSharedMtrl) {
		if (nullptr == m_vecMtrls[_iIdx].pClonedMtrl) {
			m_vecMtrls[_iIdx].pClonedMtrl = m_vecMtrls[_iIdx].pSharedMtrl->Clone();
		}
		m_vecMtrls[_iIdx].pMtrl = m_vecMtrls[_iIdx].pClonedMtrl;
	}
	return m_vecMtrls[_iIdx].pClonedMtrl;
}

SharedPtr<CMaterial> CMeshRenderer::GetClone_NoSave(UINT _iIdx)
{
	if (m_vecMtrls.size() <= _iIdx) {
		assert(nullptr);
		return nullptr;
	}

	if (nullptr != m_vecMtrls[_iIdx].pSharedMtrl) {
		if (nullptr == m_vecMtrls[_iIdx].pClonedMtrl) {
			m_vecMtrls[_iIdx].pClonedMtrl = m_vecMtrls[_iIdx].pSharedMtrl->Clone_NoAddInResMgr();
		}
		m_vecMtrls[_iIdx].pMtrl = m_vecMtrls[_iIdx].pClonedMtrl;
	}
	return m_vecMtrls[_iIdx].pClonedMtrl;
}

SharedPtr<CMaterial> CMeshRenderer::GetSharedMaterial(UINT _iIdx)
{
	m_vecMtrls[_iIdx].pMtrl = m_vecMtrls[_iIdx].pSharedMtrl;
	return m_vecMtrls[_iIdx].pMtrl;
}

bool CMeshRenderer::SaveToScene(FILE* _pFile)
{
	CRenderer::SaveToScene(_pFile);
	SaveResourceToFile(m_pMesh, _pFile);
	
	UINT iMtrlCount = (UINT)m_vecMtrls.size();
	FWrite(iMtrlCount, _pFile);
	for (UINT i = 0; i < (UINT)m_vecMtrls.size(); ++i) {
		SaveResourceToFile(m_vecMtrls[i].pSharedMtrl, _pFile);
	}

	FWrite(m_iColor, _pFile);
	return true;
}

bool CMeshRenderer::LoadFromScene(FILE* _pFile)
{
	CRenderer::LoadFromScene(_pFile);
	LoadResourceFromFile(m_pMesh, _pFile);

	UINT iMtrlCount = (UINT)m_vecMtrls.size();
	FRead(iMtrlCount, _pFile);
	for (UINT i = 0; i < iMtrlCount; ++i) {
		LoadResourceFromFile(m_vecMtrls[i].pSharedMtrl, _pFile);
		SetMaterial(m_vecMtrls[i].pSharedMtrl, i);
	}

	FRead(m_iColor, _pFile);
	return true;
}