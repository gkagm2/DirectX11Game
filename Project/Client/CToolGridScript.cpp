#include "pch.h"
#include "CToolGridScript.h"
#include <Engine\CRenderManager.h>

CToolGridScript::CToolGridScript() :
	CScript(-1),
	m_pToolCamObj{nullptr},
	m_vToolCamPos{},
	m_fThickness{0.5f},
	m_fDistance{0.f},
	m_iMaxStep{5},
	m_fAlpha{0.f},
	m_vColor{ Vector4(1.f,1.f,1.f,1.f )}
{
}

CToolGridScript::~CToolGridScript()
{
}

void CToolGridScript::Update()
{
	if (CSceneManager::GetInstance()->GetSceneMode() == E_SceneMode::Play)
		return;

	m_pToolCamObj = CRenderManager::GetInstance()->GetToolCamera()->GetGameObject();
	m_vToolCamPos = m_pToolCamObj->Transform()->GetPosition();
	Transform()->SetLocalPosition(Vector3(m_vToolCamPos.x, 0.f, m_vToolCamPos.z));
	CalculateGrid();
}

void CToolGridScript::CalculateGrid()
{
	// 카메라 높이에 따라서 Grid 간격 조절
	// 1 단계 : 간격 10, 카메라 높이 0 ~ 100
	// 2 단계 : 간격 100, 카메라 높이 100 ~ 1000
	// 3 단계 : 간격 1000, 카메라 높이 1000 ~ 10000
	// 4 단계 : 간격 10000, 카메라 높이 10000 ~ 
	SharedPtr<CMaterial> pMtrl = MeshRenderer()->GetSharedMaterial(0);

	UINT iHeight = (UINT)abs(m_vToolCamPos.y);
	UINT iStep = 1;
	UINT i = 100;
	for (; iStep <= 4; i *= 10, ++iStep) {
		if (iHeight <= i)
			break;
	}

	m_fThickness = ((float)iHeight * 0.01f);
	m_fAlpha = abs(1.f - (float)((double)iHeight / pow(10., (double)iStep + 1.)));

	// Material 에 값을 전달	
	pMtrl->SetData(E_ShaderParam::Int_0, &iStep);
	pMtrl->SetData(E_ShaderParam::Float_0, &m_fAlpha);
	pMtrl->SetData(E_ShaderParam::Float_1, &m_fThickness);
	pMtrl->SetData(E_ShaderParam::Vector4_0, &m_vToolCamPos);
	pMtrl->SetData(E_ShaderParam::Vector4_1, &m_vColor);
}