#include "pch.h"
#include "CLight2D.h"
#include "CResourceManager.h"
#include "CRenderManager.h"
#include "CTransform.h"

CLight2D::CLight2D() :
	CComponent(E_ComponentType::Light2D)
{
	m_pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_CircleLineMesh);
	m_pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_Collider2DMtrl);
	m_tInfo.eLightType = E_LightType::Point;
}

CLight2D::~CLight2D()
{
}

void CLight2D::FinalUpdate()
{
	m_tInfo.vLightPos = Transform()->GetPosition();
	CRenderManager::GetInstance()->RegisterLight2D(this);
}

void CLight2D::Render()
{
	Transform()->UpdateData();

	m_pMesh->UpdateData();
	m_pMtrl->UpdateData();

	m_pMesh->Render();
	m_pMtrl->Clear();
}

void CLight2D::SetRange(float _fRange)
{
	m_tInfo.fRange = _fRange;
	Transform()->SetLocalScale(Vector3(_fRange * 2.f, _fRange * 2.f, 1.f));
}

bool CLight2D::SaveToScene(FILE* _pFile)
{
	FWrite(m_tInfo, _pFile);

	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pMtrl, _pFile);
	return true;
}

bool CLight2D::LoadFromScene(FILE* _pFile)
{
	FRead(m_tInfo, _pFile);

	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMtrl, _pFile);
	return true;
}