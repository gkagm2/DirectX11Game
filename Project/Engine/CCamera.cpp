#include "pch.h"
#include "CCamera.h"
#include "CDevice.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CRenderManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CLayer.h"
#include "CMeshRenderer.h"
#include "CSpriteRenderer.h"
#include "CCanvasRenderer.h"
#include "CCore.h"

#include "CCollider2D.h"
#include "CParticleSystem.h"
#include "CSkybox.h"

// Test
#include "CKeyManager.h"
#include "CTimeManager.h"


CCamera::CCamera() :
	CComponent(E_ComponentType::Camera),
	m_eProjectionType(E_ProjectionType::Perspective),
	m_tFOVAxis{60.f, 67.f}, // XM_PI / 3.f 45도 60도
	m_fSize{ 0.02f },
	m_tClippingPlanes{ 0.3f, 999999.f},
	m_tViewportRect{0.f,0.f,1.f,1.f},
	m_matView{},
	m_matProjection{},
	m_iLayerCheck(0)
{
	SetLayerCheckAll();
}

CCamera::~CCamera()
{
}

void CCamera::FinalUpdate()
{
	CalculateViewMatrix();
	CalculateProjectionMatrix();

	g_transform.matView = GetViewMatrix();
	g_transform.matProjection = GetProjectionMatrix();
	CRenderManager::GetInstance()->RegisterCamera(this);
}

Vector3 CCamera::GetScreenToWorld2DPosition(const Vector2& _vPosition)
{
	// orthographic일경우에만 사용 가능 
	if (E_ProjectionType::Perspective == GetProjectionType()) {
		assert(nullptr && _T("Perspectie type임"));
		return Vector3(0.f, 0.f, 0.f);
	}
		

	Vector3 vWorldPos = Transform()->GetPosition();
	Vector2 vScreenHalfSize = CCore::GetInstance()->GetWindowResolution();
	vScreenHalfSize /= 2.f;
	Vector2 vPos;
	vPos.x = (-vScreenHalfSize.x + _vPosition.x) * GetSize();
	vPos.y = (vScreenHalfSize.y - _vPosition.y) * GetSize();

	vWorldPos.x = vWorldPos.x + vPos.x;
	vWorldPos.y = vWorldPos.y + vPos.y;
	return vWorldPos;
}

Vector2 CCamera::GetWorldToScreen2DPosition(const Vector3& _vWorldPosition)
{
	Vector3 vLTPos = GetScreenToWorld2DPosition(Vector2(0.f, 0.f));
	Vector3 vRBPos = GetScreenToWorld2DPosition(Vector2(CCore::GetInstance()->GetWindowResolution()));

	// 영역 안에 있는지 에러 처리하는 

	Vector3 vWH{};
	vWH.x = vRBPos.x - vLTPos.x;
	vWH.y = vLTPos.y - vRBPos.y;

	Vector3 vCursorWHPos{};
	vCursorWHPos.x = _vWorldPosition.x - vLTPos.x;
	vCursorWHPos.y = vLTPos.y - _vWorldPosition.y;

	Vector3 vCursorResultPos{};
	vCursorResultPos.x = vCursorWHPos.x / vWH.x;
	vCursorResultPos.y = vCursorWHPos.y / vWH.y;
	
	Vector2 vScreenPos{};
	vScreenPos = CCore::GetInstance()->GetWindowResolution();

	Vector2 vFinalScreenPos = vCursorResultPos * vScreenPos;

	return vFinalScreenPos;
}

float CCamera::GetDistancePerFixel()
{
	Vector3 v1 = GetScreenToWorld2DPosition(Vector2(1.f, 0.f));
	Vector3 v2 = GetScreenToWorld2DPosition(Vector2(0.f, 0.f));
	float vDitance = Vector3::Distance(v1, v2);
	return vDitance;
}

void CCamera::CalculateViewMatrix()
{
	// dept(RotMat) * LocalRotMat * OtherRotMat = IdentityMat
	// same
	// transpose(RotMat) * LocalRotMat * OtherRotMat = IdentityMat

	//const Vector3& vPos = GetGameObject()->Transform()->GetLocalPosition();

	//Matrix matTrans = XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);

	//Vector3 vRight = Transform()->GetRightVector();
	//Vector3 vUp =	 Transform()->GetUpVector();
	//Vector3 vFront = Transform()->GetFrontVector();

	//Matrix matRevolution = XMMatrixIdentity(); // 공전 (회전 량)

	//// 전치된 행렬임 (역행렬 역할을 함. 서로 직교인 벡터이기 때문에 전치 이용함)
	//matRevolution._11 = vRight.x; matRevolution._12 = vUp.x; matRevolution._13 = vFront.x;
	//matRevolution._21 = vRight.y; matRevolution._22 = vUp.y; matRevolution._23 = vFront.y;
	//matRevolution._31 = vRight.z; matRevolution._32 = vUp.z; matRevolution._33 = vFront.z;

	//// 회전 행렬 구하기
	//m_matView = matTrans * matRevolution; // 카메라는 원점으로 이동 후 회전시켜야되므로 위치 * 회전임.

	
#pragma region 똑같은 기능의 다른 버전
	Vector3 vPos = Transform()->GetLocalPosition();
	m_matView = XMMatrixIdentity();
	Vector3 vRight = Transform()->GetRightVector();
	Vector3 vUp = Transform()->GetUpVector();
	Vector3 vFront = Transform()->GetFrontVector();

	// Translation 과 Rotation을 한꺼번에 넣어줌.
	m_matView._11 = vRight.x;			m_matView._12 = vUp.x;			m_matView._13 = vFront.x;
	m_matView._21 = vRight.y;			m_matView._22 = vUp.y;			m_matView._23 = vFront.y;
	m_matView._31 = vRight.z;			m_matView._32 = vUp.z;			m_matView._33 = vFront.z;
	m_matView._41 = -vPos.Dot(vRight);  m_matView._42 = -vPos.Dot(vUp); m_matView._43 = -vPos.Dot(vFront);
#pragma endregion
	
	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
}

void CCamera::CalculateProjectionMatrix()
{
	const Vector2& vRenderResolution = CDevice::GetInstance()->GetRenderResolution();
	if (E_ProjectionType::Orthographic == m_eProjectionType) {
		m_matProjection = XMMatrixOrthographicLH(vRenderResolution.x * m_fSize, vRenderResolution.y * m_fSize, m_tClippingPlanes.fNear, m_tClippingPlanes.fFar);
	}
	else if (E_ProjectionType::Perspective == m_eProjectionType) {
		// 화면 종횡비

		float fFov = XM_PI / 3.f;
		float fAspectRatio = vRenderResolution.x / vRenderResolution.y;
		m_matProjection = XMMatrixPerspectiveFovLH(fFov, fAspectRatio, m_tClippingPlanes.fNear, m_tClippingPlanes.fFar);
	}
}

bool CCamera::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
	FWrite(m_eProjectionType, _pFile);
	FWrite(m_tFOVAxis, _pFile);
	FWrite(m_fSize, _pFile);
	FWrite(m_tClippingPlanes, _pFile);
	FWrite(m_tViewportRect, _pFile);
	FWrite(m_iLayerCheck, _pFile);
	
	return true;
}

bool CCamera::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);
	FRead(m_eProjectionType, _pFile);
	FRead(m_tFOVAxis, _pFile);
	FRead(m_fSize, _pFile);
	FRead(m_tClippingPlanes, _pFile);
	FRead(m_tViewportRect, _pFile);
	FRead(m_iLayerCheck, _pFile);

	return true;
}

// Rendering

void CCamera::_SortObjects()
{
	m_vecDeferred.clear();
	m_vecForward.clear();
	m_vecParticle.clear();
	m_vecPostEffect.clear();
	m_vecCollider2D.clear();
	m_vecCanvas.clear();

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();

	for (UINT i = 0; i < MAX_SIZE_LAYER; ++i) {
		if ((m_iLayerCheck & (1 << i)) > 0) { // 체크된 레이어면
			CLayer* pLayer = pCurScene->GetLayer(i);

			const vector<CGameObject*>& vecAllObjs = pLayer->GetGameObjects();

			for (UINT i = 0; i < vecAllObjs.size(); ++i) {
				CGameObject* pObj = vecAllObjs[i];
				if (!pObj->IsActive())
					continue;
				
				CGameObject* pParentObj = pObj;
				bool isRender = true;
				while (nullptr != pParentObj) {
					if (pParentObj->IsActive())
						pParentObj = pParentObj->GetParentObject();
					else {
						isRender = false;
						break;
					}
				}
				if (!isRender)
					continue;

				E_RenderTimePoint eRenderTimePoint = E_RenderTimePoint::None;

				//MeshRenderer 
				if (pObj->MeshRenderer() &&
					pObj->MeshRenderer()->GetSharedMaterial().Get() &&
					pObj->MeshRenderer()->GetSharedMaterial()->GetShader().Get()) {
					SharedPtr<CGraphicsShader> pShader = pObj->MeshRenderer()->GetSharedMaterial()->GetShader();

					// forward or posteffect
					eRenderTimePoint = pShader->GetRenderTimePoint();
				}
				// SpriteRenderer
				else if (pObj->SpriteRenderer() &&
						pObj->SpriteRenderer()->GetSharedMaterial().Get() &&
						pObj->SpriteRenderer()->GetSharedMaterial()->GetShader().Get()) {
					SharedPtr<CGraphicsShader> pShader = pObj->SpriteRenderer()->GetSharedMaterial()->GetShader();
					
					// forward or posteffect
					eRenderTimePoint = pShader->GetRenderTimePoint();
				}
				else if (pObj->CanvasRenderer()) {
					// FIXED(Jang) : RenderTimePoint를 가져올 부분에 대해 설계해봐야 될 듯
					eRenderTimePoint = E_RenderTimePoint::Canvas;
				}
				else if (pObj->ParticleSystem()) {
					eRenderTimePoint = E_RenderTimePoint::Particle;
				}
				else if (pObj->Collider2D()) {
					eRenderTimePoint = E_RenderTimePoint::Collider;
				}
				else if (pObj->TileMap()) {
					eRenderTimePoint = E_RenderTimePoint::Forward;
				}
				else if (pObj->Skybox() &&
					pObj->Skybox()->GetMesh().Get() &&
					pObj->Skybox()->GetMaterial().Get() &&
					pObj->Skybox()->GetMaterial()->GetShader().Get()) {
					eRenderTimePoint = pObj->Skybox()->GetMaterial()->GetShader()->GetRenderTimePoint();
				}
				else {
					//assert(nullptr);
				}
				
				switch (eRenderTimePoint) {
				case E_RenderTimePoint::Deferred:
					m_vecDeferred.push_back(pObj);
					break;
				case E_RenderTimePoint::Forward:
					m_vecForward.push_back(pObj);
					break;
				case E_RenderTimePoint::Canvas:
					m_vecCanvas.push_back(pObj);
					break;
				case E_RenderTimePoint::Collider:
					m_vecCollider2D.push_back(pObj);
					break;
				case E_RenderTimePoint::Particle:
					m_vecParticle.push_back(pObj);
					break;
				case E_RenderTimePoint::PostEffect:
					m_vecPostEffect.push_back(pObj);
					break;
				default:
					//assert(nullptr);
					break;
				}
			}
		}
	}
}

void CCamera::_RenderDeferred()
{
	/////////////////////////////////////
	// (UpdateData) RenderDeferred가 제일 먼저 실행되어 여기에 넣어줌.
	g_transform.matView = m_matView;
	g_transform.matProjection = m_matProjection;
	g_transform.matViewInv = m_matViewInv;

	for (UINT i = 0; i < m_vecDeferred.size(); ++i)
		m_vecDeferred[i]->Render();
}

void CCamera::_RenderForward()
{
	/////////////////////////////////////
	// (UpdateData) RenderForward가 제일 먼저 실행되어 여기에 넣어줌.
	g_transform.matView = m_matView;
	g_transform.matProjection = m_matProjection;
	g_transform.matViewInv = m_matViewInv;

	for (UINT i = 0; i < m_vecForward.size(); ++i)
		m_vecForward[i]->Render();
}

void CCamera::_RenderParticle()
{
	for (UINT i = 0; i < m_vecParticle.size(); ++i)
		m_vecParticle[i]->Render();
}

void CCamera::_RenderPostEffect()
{
	for (UINT i = 0; i < m_vecPostEffect.size(); ++i) {
		CRenderManager::GetInstance()->_CopyBackBufferToPostEffectBuffer();
		m_vecPostEffect[i]->Render();
	}
}

void CCamera::_RenderCollider2D()
{
	for (UINT i = 0; i < m_vecCollider2D.size(); ++i)
		m_vecCollider2D[i]->Render();
}

void CCamera::_RenderCanvas()
{
	for (UINT i = 0; i < m_vecCanvas.size(); ++i)
		m_vecCanvas[i]->Render();
}
