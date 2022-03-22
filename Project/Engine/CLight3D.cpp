#include "pch.h"
#include "CLight3D.h"
#include "CResourceManager.h"
#include "CRenderManager.h"
#include "CTransform.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CCameraModule.h"
#include "CDevice.h"
#include "CConstBuffer.h"
CLight3D::CLight3D() :
	CComponent(E_ComponentType::Light3D),
	m_tInfo{},
	m_pLightCamObj{ nullptr },
	m_matLightWorld{}
{
	/*m_pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_CircleLineMesh);
	m_pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_Collider2DCollisionMtrl);
	m_tInfo.eLightType = E_LightType::Direction;*/
	m_tInfo.idx = -1;
	m_tInfo.tColor.vDiffuse = Vector4(0.8f, 0.8f, 0.8f, 1.f);
	m_tInfo.tColor.vEmbient= Vector4(0.2f, 0.2f, 0.2f, 1.f);
	m_tInfo.tColor.vSpecular = Vector4(0.8f, 0.8f, 0.8f, 1.f);

	SetLightType(E_LightType::Direction);
}

CLight3D::CLight3D(const CLight3D& _origin) :
	CComponent(E_ComponentType::Light3D),
	m_tInfo(_origin.m_tInfo),
	m_pLightCamObj{ nullptr }
{
	m_tInfo.idx = -1;
	SetLightType((E_LightType)_origin.m_tInfo.eLightType);
	
	if (_origin.m_pLightCamObj)
		m_pLightCamObj = _origin.m_pLightCamObj->Clone();
}

CLight3D::~CLight3D()
{
	SAFE_DELETE(m_pLightCamObj);
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

	if (m_pLightCamObj) {
		m_pLightCamObj->Transform()->SetLocalPosition(Transform()->GetPosition());
		m_pLightCamObj->Transform()->SetLocalScale(Transform()->GetScale());
		m_pLightCamObj->Transform()->SetLocalRotation(Transform()->GetRotation());
		m_pLightCamObj->FinalUpdate();
	}

	if (isRender) {
		if (m_pLightCamObj && m_pLightCamObj->IsActive() != isRender)
			m_pLightCamObj->SetActive(isRender);


		m_tInfo.vLightPos = Transform()->GetPosition();
		m_tInfo.vLightDir = Transform()->GetFrontVector(); // 정면 방향
		if (GetGameObject()->IsActive() && IsActive())
			m_tInfo.idx = CRenderManager::GetInstance()->RegisterLight3D(this);
	}

	/*Vector3 vTrans = Transform()->GetLocalPosition();
	Vector3 vScale = Transform()->GetLocalScale();
	Vector3 vRot = Transform()->GetLocalRotation();

	float fScale = m_tInfo.fRange * 2.f;
	Matrix matTrans = XMMatrixTranslation(vTrans.x, vTrans.y, vTrans.z);
	Matrix matScale = XMMatrixScaling(fScale, fScale, fScale);
	m_matLightWorld = matScale * matTrans * Transform()->GetWorldMatrix();*/
}

void CLight3D::SetLightType(E_LightType _eType)
{
	m_tInfo.eLightType = _eType;

	// TODO (Jang) : Point, Spot 작성
	switch (_eType) {
	case E_LightType::Direction: {
		m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh);
		m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_DirectionLightMtrl);
		if (!m_pLightCamObj)
			_CreateLight3DCamera();
		break;
	}
	case E_LightType::Point: {
		m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_SphereMesh);
		m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_PointLightMtrl);
		SAFE_DELETE(m_pLightCamObj);
		break;
	}
	case E_LightType::Spot: {
		m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_ConeMesh);
		m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_SpotLightMtrl);
		SAFE_DELETE(m_pLightCamObj);
		break;
	}
	default:
		assert(nullptr);
		break;
	}
	assert(m_pMesh.Get());
	assert(m_pMtrl.Get());
}


void CLight3D::SetRange(float _fRange)
{
	m_tInfo.fRange = _fRange;
	Transform()->SetLocalScale(Vector3(_fRange * 2.f, _fRange * 2.f, _fRange * 2.f));
}
void CLight3D::Render()
{
	if (nullptr == m_pMesh || 
		nullptr == m_pMtrl || 
		nullptr == m_pMtrl->GetShader() || 
		E_RenderTimePoint::Light != m_pMtrl->GetShader()->GetRenderTimePoint())
		return;

	switch (m_tInfo.eLightType) {
	case E_LightType::Direction: {
		Matrix matDirCamVP = m_pLightCamObj->Camera()->GetViewMatrix() * m_pLightCamObj->Camera()->GetProjectionMatrix();
		m_pMtrl->SetData(E_ShaderParam::Matrix_0, &matDirCamVP);
		
		break;
	}
	case E_LightType::Point: {
		Transform()->UpdateData();
		/*static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Transform);
		g_transform.matWorld = m_matLightWorld;
		g_transform.matWorldView = g_transform.matWorld * g_transform.matView;
		g_transform.matWorldViewProj = g_transform.matWorldView * g_transform.matProjection;
		pCB->SetData(&g_transform);
		pCB->UpdateData(E_ShaderStage::Vertex);*/
		break;
	}
	case E_LightType::Spot: {
		Transform()->UpdateData();
		/*static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Transform);
		g_transform.matWorld = m_matLightWorld;
		g_transform.matWorldView = g_transform.matWorld * g_transform.matView;
		g_transform.matWorldViewProj = g_transform.matWorldView * g_transform.matProjection;
		pCB->SetData(&g_transform);
		pCB->UpdateData(E_ShaderStage::Vertex);*/
		break;
	}
	default:
		assert(nullptr);
		break;
	}

	
	m_pMtrl->SetData(E_ShaderParam::Int_0, &m_tInfo.idx);
	m_pMtrl->UpdateData();
	m_pMesh->Render();
	m_pMtrl->Clear();		 // 메터리얼 레지스터 Clear
}

bool CLight3D::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);
	FRead(m_tInfo, _pFile);

	// TODO (Jang) : Light3D Camera 추가.

	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMtrl, _pFile);
	return true;
}
bool CLight3D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
	FWrite(m_tInfo, _pFile);

	// TODO (Jang) : Light3D Camera 추가.

	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pMtrl, _pFile);
	return true;
}

void CLight3D::_CreateLight3DCamera()
{
	m_pLightCamObj = new CGameObject;
	m_pLightCamObj->SetName(_T("Light3D Inner Camera"));
	m_pLightCamObj->AddComponent<CTransform>();
	m_pLightCamObj->AddComponent<CCameraModule>();

	// 모든 레이어를 확인하기위해 모두 check on
	m_pLightCamObj->Camera()->SetLayerCheckAll();

	// 방향성 광원이므로, 물체의 깊이정보를 직교투영으로 그린다.
	m_pLightCamObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	m_pLightCamObj->Camera()->SetSize(1.f);
	TClippingPlanes tCp;
	tCp.fNear = 1.f;
	tCp.fFar = 100000.f;
	m_pLightCamObj->Camera()->SetClippingPlanes(tCp);
	m_pLightCamObj->Camera()->SetViewportWidth(4000.f);
	m_pLightCamObj->Camera()->SetViewportHeight(4000.f);
}