#include "pch.h"
#include "CLight3D.h"
#include "CResourceManager.h"
#include "CRenderManager.h"
#include "CTransform.h"
#include "CMesh.h"
#include "CMaterial.h"
CLight3D::CLight3D() :
	CComponent(E_ComponentType::Light3D)
{
	m_pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_CircleLineMesh);
	m_pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_Collider2DCollisionMtrl);
	m_tInfo.eLightType = E_LightType::Point;

	m_tInfo.fAngle_Radian = PI * 0.5f;
	m_tInfo.fRange = 2.f;
	m_tInfo.tColor.vDiffuse = Vector4(0.8f, 0.8f, 0.8f, 1.f);
	m_tInfo.tColor.vEmbient = Vector4(0.8f, 0.8f, 0.8f, 1.f);
	m_tInfo.tColor.vSpecular = Vector4(0.8f, 0.8f, 0.8f, 1.f);
}

CLight3D::~CLight3D()
{
}

void CLight3D::FinalUpdate()
{
	CGameObject* pParentObj = GetGameObject();
	bool isRender = true;
	while (nullptr != pParentObj) {
		if (!pParentObj->IsActive()) {
			isRender = false;
			break;
		}
		pParentObj = pParentObj->GetParentObject();
	}
	if (isRender) {
		m_tInfo.vLightPos = Transform()->GetPosition();
		m_tInfo.vLightDir = Transform()->GetUpVector();
		if (GetGameObject()->IsActive() && IsActive())
			m_tInfo.idx = CRenderManager::GetInstance()->RegisterLight3D(this);
	}
}
void CLight3D::SetRange(float _fRange)
{
	m_tInfo.fRange = _fRange;
	Transform()->SetLocalScale(Vector3(_fRange * 2.f, _fRange * 2.f, 1.f));
}
bool CLight3D::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);
	FRead(m_tInfo, _pFile);

	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMtrl, _pFile);
	return true;
}
bool CLight3D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
	FWrite(m_tInfo, _pFile);

	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pMtrl, _pFile);
	return true;
}