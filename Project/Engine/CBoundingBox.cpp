#include "pch.h"
#include "CBoundingBox.h"
#include "CRenderManager.h"
#include "CTransform.h"
#include "CResourceManager.h"

CBoundingBox::CBoundingBox() :
	CComponent(E_ComponentType::BoundingBox)
{
	m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_DebugSphereMesh);
	m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_DebugRenderingMtrl);
}

CBoundingBox::~CBoundingBox()
{
}

void CBoundingBox::Render()
{
	Transform()->UpdateData();
	Vector3 vColor = Vector3(0.f, 0.5f, 0.5f);
	
	m_pMtrl->SetData(E_ShaderParam::Vector4_0, &vColor);
	m_pMtrl->UpdateData();
	m_pMesh->Render(0);
}