#include "pch.h"
#include "CCamera.h"
#include "CDevice.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CRenderManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CLayer.h"

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
}

CCamera::~CCamera()
{
}

void CCamera::FinalUpdate()
{
	_CalculateViewMatrix();
	_CalculateProjectionMatrix();

	g_transform.matView = m_matView;
	g_transform.matProjection = m_matProjection;
	CRenderManager::GetInstance()->RegisterCamera(this);
}

void CCamera::Render()
{
	g_transform.matView = m_matView;
	g_transform.matProjection = m_matProjection;

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
	for (UINT i = 0; i < (UINT)E_Layer::End; ++i) {
		CLayer* pLayer = pCurScene->GetLayer(i);
		const vector<CGameObject*>& vecAllObjs = pLayer->GetGameObjects();
		for (UINT j = 0; j < vecAllObjs.size(); ++j)
			vecAllObjs[j]->Render();
	}
}

void CCamera::_CalculateViewMatrix()
{
	const Vector3& vPos = GetGameObject()->Transform()->GetLocalPosition();
	m_matView = XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);
}

void CCamera::_CalculateProjectionMatrix()
{
	const Vector2& vRenderResolution = CDevice::GetInstance()->GetRenderResolution();

	m_matProjection = XMMatrixOrthographicLH(vRenderResolution.x * m_fSize, vRenderResolution.y * m_fSize, m_tClippingPlanes.fNear, m_tClippingPlanes.fFar);
}