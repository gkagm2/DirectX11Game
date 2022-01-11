#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CRectTransform.h"
#include "CRenderManager.h"
#include "CCamera.h"

#include "CGameObject.h"
// Test
CTransform::CTransform() :
	CComponent(E_ComponentType::Transform),
	m_vLocalPosition{},
	m_vLocalScale{ 1.f, 1.f, 1.f },
	m_vLocalRotation{},
	m_matLocal{},
	m_matWorld{},

	m_fWidth(100.f),
	m_fHeight(100.f),
	m_matLocal_noParentScale{},
	m_matWorld_noParentScale{},
	m_vLocalScale_RT{Vector3(1.f,1.f,1.f)}
{
}

CTransform::CTransform(E_ComponentType _eComponentType) :
	CComponent(_eComponentType),
	m_vLocalPosition{},
	m_vLocalScale{ 1.f, 1.f, 1.f },
	m_vLocalRotation{},
	m_matLocal{},
	m_matWorld{},

	m_fWidth(100.f),
	m_fHeight(100.f),
	m_matLocal_noParentScale{},
	m_matWorld_noParentScale{},
	m_vLocalScale_RT{ Vector3(1.f,1.f,1.f) }
{
}

CTransform::~CTransform()
{
}

Vector3 CTransform::GetScale()
{
	Vector3 vWorldScale = m_vLocalScale;
	CGameObject* pGameObj = GetGameObject();

	while (pGameObj->GetParentObject()) {
		if (pGameObj->GetParentObject()->RectTransform())
			vWorldScale *= pGameObj->GetParentObject()->RectTransform()->GetLocalScale();
		else
			vWorldScale *= pGameObj->GetParentObject()->Transform()->GetLocalScale();
		pGameObj = pGameObj->GetParentObject();
	}
	return vWorldScale;
}

Vector3 CTransform::GetRotation()
{
	Vector3 vWorldRotation = m_vLocalRotation;
	CGameObject* pGameObj = GetGameObject();

	while (pGameObj->GetParentObject()) {
		if (pGameObj->GetParentObject()->RectTransform())
			vWorldRotation += pGameObj->GetParentObject()->RectTransform()->GetLocalRotation();
		else
			vWorldRotation += pGameObj->GetParentObject()->Transform()->GetLocalRotation();
		pGameObj = pGameObj->GetParentObject();
	}
	return vWorldRotation;
}

Vector3 CTransform::GetRotationDegree()
{
	return GetRotation() * CMyMath::Rad2Deg();
}

void CTransform::LookAt2D(const Vector2& m_vWorldPos)
{
	Vector3 vCurPos = GetPosition();
	Vector2 vToTargetDir = m_vWorldPos - vCurPos;
	vToTargetDir.Normalize();
	float randian = atan2f(vToTargetDir.y, vToTargetDir.x);
	Vector3 vParentRot {};
	if (GetGameObject()->GetParentObject()) 
		vParentRot = GetGameObject()->GetParentObject()->Transform()->GetRotation();
	Vector3 vLocalRot = GetLocalRotation();
	float x = GetGameObject()->Transform()->GetScale().x;
	if(x > 0)
		
		vLocalRot.z = randian - vParentRot.z;
	else {
		vLocalRot.z = (PI * 2.f - randian + vParentRot.z);
	}
	
	SetLocalRotation(vLocalRot);
}

void CTransform::_LinkParent()
{
	// Scale
	Vector3 vLocalScale = GetLocalScale();
	Vector3 fDefaultWorldScale = Vector3::One;
	if (GetGameObject()->GetParentObject())
		fDefaultWorldScale = GetScale();
	Vector3 vResultLocalScale = (vLocalScale / fDefaultWorldScale) * vLocalScale;
	SetLocalScale(vResultLocalScale);


	// Rotation
	Vector3 vRot = GetRotation();
	Vector3 vParentWorldRot = Vector3::Zero;
	if (GetGameObject()->GetParentObject())
		vParentWorldRot = GetGameObject()->GetParentObject()->Transform()->GetRotation();
	Vector3 vDiffRot = vRot - vParentWorldRot;

	// 부모 포지션을 중점으로부터 회전해야 한다.
	// TODO (Jang) : 여기서
	SetLocalRotation(vDiffRot);


	// 월드 좌표를 로컬 좌표로 넣는다.
	Vector3 vParentWorldPos = GetPosition();
	Vector3 vParentScalePos = GetGameObject()->GetParentObject()->Transform()->GetScale();
	Vector3 vChildPos = GetPosition();
	if (GetGameObject()->GetParentObject()) {
		vParentWorldPos = GetGameObject()->GetParentObject()->Transform()->GetPosition();
	}
	Vector3 vLocalPosition = vChildPos - vParentWorldPos;
	vLocalPosition = vLocalPosition / vParentScalePos;
	SetLocalPosition(vLocalPosition);

}

void CTransform::_UnlinkParent(const Vector3& vParentLocalScale, const Vector3& vParentLocalRotation)
{
	Vector3 vScale;
	vScale.x = 1.f / vParentLocalScale.x;
	vScale.y = 1.f / vParentLocalScale.y;
	vScale.z = 1.f / vParentLocalScale.z;
	m_vLocalScale = m_vLocalScale / vScale;

	// 월드 좌표를 로컬좌표로 넣는다.
	Vector3 vWorldPos = GetPosition();
	SetLocalPosition(vWorldPos);

	// 월드 회전을 로컬 회전으로 넣는다.
	Vector3 vWorldRot = vParentLocalRotation + GetLocalRotation();
	SetLocalRotation(vWorldRot);
}

void CTransform::FinalUpdate()
{
	// 크기 행렬
	Matrix matScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	// 회전 행렬
	Matrix matRotX = XMMatrixRotationX(m_vLocalRotation.x);
	Matrix matRotY = XMMatrixRotationY(m_vLocalRotation.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vLocalRotation.z);

	Matrix matRot = matRotX * matRotY * matRotZ;
	// 이동 행렬
	Matrix matTrans = XMMatrixTranslation(m_vLocalPosition.x, m_vLocalPosition.y, m_vLocalPosition.z);

	m_matWorld = m_matLocal = matScale * matRot * matTrans;

	// 회전
	// Local 방향 업데이트 (우 상 전)
	m_vWorldRightDir = m_vLocalRightDir = XMVector3Normalize(XMVector3TransformNormal(Vector3::Right, matRot));
	m_vWorldUpDir = m_vLocalUpDir = XMVector3Normalize(XMVector3TransformNormal(Vector3::Up, matRot));
	m_vWorldFrontDir = m_vLocalFrontDir = XMVector3Normalize(XMVector3TransformNormal(Vector3::Front, matRot));

	CGameObject* pParentObj = GetGameObject()->GetParentObject();
	if (pParentObj) {
		if (pParentObj->RectTransform()) {
			const Matrix& matParentWorld = pParentObj->RectTransform()->GetWorldMatrix();
			m_matWorld *= matParentWorld;
		}
		else {
			const Matrix& matParentWorld = pParentObj->Transform()->GetWorldMatrix();
			m_matWorld *= matParentWorld;
		}

		// 부모상태까지 적용시킨 월드 방향 정보
		m_vWorldRightDir = XMVector3Normalize(XMVector3TransformNormal(Vector3::Right, matRot));
		m_vWorldUpDir = XMVector3Normalize(XMVector3TransformNormal(Vector3::Up, matRot));
		m_vWorldFrontDir = XMVector3Normalize(XMVector3TransformNormal(Vector3::Front, matRot));
	}

	// Canvas Renderer, 2D Sprite Renderer
	float fSize = 1.f;
	//if (CRenderManager::GetInstance()->GetUICamera()) {
	//	CCamera* pMainCam = CRenderManager::GetInstance()->GetMainCamera();
	//	if (E_ProjectionType::Orthographic == pMainCam->GetProjectionType()) {
	//		fSize = pMainCam->GetSize();
	//	}
	//}

	m_vLocalScale_RT.x = m_vLocalScale.x * m_fWidth;
	m_vLocalScale_RT.y = m_vLocalScale.y * m_fHeight;
	m_vLocalScale_RT.z = m_vLocalScale.z;
	Matrix matScaleWH = XMMatrixScaling(m_vLocalScale_RT.x, m_vLocalScale_RT.y, m_vLocalScale_RT.z);
	
	m_matLocal_noParentScale = matScaleWH * matRot * matTrans * fSize;
	m_matWorld_noParentScale = m_matLocal_noParentScale;
	pParentObj = GetGameObject()->GetParentObject();
	if (pParentObj) {
		if (pParentObj->RectTransform()) {
			const Matrix& matParentWorld = pParentObj->RectTransform()->GetWorldMatrix();
			m_matWorld_noParentScale *= matParentWorld;
		}
		else {
			const Matrix& matParentWorld = pParentObj->Transform()->GetWorldMatrix();
			m_matWorld_noParentScale *= matParentWorld; 
		}
	}
}

void CTransform::UpdateData()
{
	// b0 레지스터에 상수버퍼 바인딩
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Transform);

	g_transform.matWorld = m_matWorld;
	g_transform.matWorldView = g_transform.matWorld * g_transform.matView;
	g_transform.matWorldViewProj = g_transform.matWorldView * g_transform.matProjection;

	pCB->SetData(&g_transform);
	pCB->UpdateData(E_ShaderStage::NonePixel);

	// b4 레지스터에 상수버퍼 바인딩
	static const CConstBuffer* pRectCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::RectTransform);

	g_rectTransform.matWorld = m_matWorld_noParentScale;
	g_rectTransform.matView = g_transform.matView;
	g_rectTransform.matProjection = g_transform.matProjection;

	pRectCB->SetData(&g_rectTransform);
	pRectCB->UpdateData(E_ShaderStage::NonePixel);
}

bool CTransform::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
	FWrite(m_vLocalPosition, _pFile);
	FWrite(m_vLocalRotation, _pFile);
	FWrite(m_vLocalScale, _pFile);

	FWrite(m_fWidth, _pFile);
	FWrite(m_fHeight, _pFile);

	return true;
}

bool CTransform::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);
	FRead(m_vLocalPosition, _pFile);
	FRead(m_vLocalRotation, _pFile);
	FRead(m_vLocalScale, _pFile);

	FRead(m_fWidth, _pFile);
	FRead(m_fHeight, _pFile);
	return true;
}