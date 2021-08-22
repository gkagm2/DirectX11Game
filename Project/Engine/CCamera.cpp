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
#include "CCore.h"

// Test
#include "CKeyManager.h"
#include "CTimeManager.h"


CCamera::CCamera() :
	CComponent(E_ComponentType::Camera),
	m_eProjectionType(E_ProjectionType::Perspective),
	m_tFOVAxis{60.f, 67.f},
	m_fSize{ 1.f },
	m_tClippingPlanes{ 1.f, 1000.f },
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

const Vector3& CCamera::GetScreenToWorld2DPosition(const Vector2& _vPosition)
{
	// orthographic일경우에만 사용 가능 
	if (E_ProjectionType::Perspective == GetProjectionType())
		assert(nullptr &&_T("Perspectie type임"));

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

void CCamera::CalculateViewMatrix()
{
	const Vector3& vPos = GetGameObject()->Transform()->GetLocalPosition();
	m_matView = XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);
}

void CCamera::CalculateProjectionMatrix()
{
	const Vector2& vRenderResolution = CDevice::GetInstance()->GetRenderResolution();

	m_matProjection = XMMatrixOrthographicLH(vRenderResolution.x * m_fSize, vRenderResolution.y * m_fSize, m_tClippingPlanes.fNear, m_tClippingPlanes.fFar);
}

bool CCamera::SaveToScene(FILE* _pFile)
{
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
	m_vecForward.clear();
	m_vecParticle.clear();
	m_vecPostEffect.clear();

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();

	for (UINT i = 0; i < MAX_SIZE_LAYER; ++i) {
		if (m_iLayerCheck & (1 << i)) { // 체크된 레이어면
			CLayer* pLayer = pCurScene->GetLayer(i);

			const vector<CGameObject*>& vecAllObjs = pLayer->GetGameObjects();

			for (UINT i = 0; i < vecAllObjs.size(); ++i) {
				CGameObject* pObj = vecAllObjs[i];
				if (!pObj->IsActive())
					continue;

				if (pObj->MeshRenderer() &&
					pObj->MeshRenderer()->GetSharedMaterial().Get() &&
					pObj->MeshRenderer()->GetSharedMaterial()->GetShader().Get()) {
					SharedPtr<CGraphicsShader> pShader = pObj->MeshRenderer()->GetSharedMaterial()->GetShader();

					switch (pShader->GetRenderPov()) {
					case E_RenderPov::Forward:
						m_vecForward.push_back(pObj);
						break;
					case E_RenderPov::PostEffect:
						m_vecPostEffect.push_back(pObj);
						break;
					default:
						assert(nullptr);
						break;
					}
				}
				else if (pObj->ParticleSystem()) {
					m_vecParticle.push_back(pObj);
				}
			}
		}
	}
}

void CCamera::_RenderForward()
{
	g_transform.matView = m_matView;
	g_transform.matProjection = m_matProjection;

	for (UINT i = 0; i < m_vecForward.size(); ++i)
		m_vecForward[i]->Render();
}

void CCamera::_RenderParticle()
{
	g_transform.matView = m_matView;
	g_transform.matProjection = m_matProjection;

	for (UINT i = 0; i < m_vecParticle.size(); ++i)
		m_vecParticle[i]->Render();
}

void CCamera::_RenderPostEffect()
{
	g_transform.matView = m_matView;
	g_transform.matProjection = m_matProjection;

	for (UINT i = 0; i < m_vecPostEffect.size(); ++i) {
		CRenderManager::GetInstance()->_CopyBackBuffer();
		m_vecPostEffect[i]->Render();
	}
}