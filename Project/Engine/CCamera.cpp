#include "pch.h"
#include "CCamera.h"
#include "CDevice.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CRenderManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CLayer.h"

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

void CCamera::Update()
{
	Vector3 vPos = Transform()->GetLocalPosition();
	if (InputKeyHold(E_Key::UP)) {
		vPos.y += DT * 150.f;
	}
	if (InputKeyHold(E_Key::DOWN)) {
		vPos.y -= DT * 150.f;
	}
	if (InputKeyHold(E_Key::LEFT)) {
		vPos.x -= DT * 150.f;
	}
	if (InputKeyHold(E_Key::RIGHT)) {
		vPos.x += DT * 150.f;
	}
	Transform()->SetLocalPosition(vPos);
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