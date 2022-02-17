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
	// ī�޶� ���̿� ���� Grid ���� ����
	// 1 �ܰ� : ���� 10, ī�޶� ���� 0 ~ 100
	// 2 �ܰ� : ���� 100, ī�޶� ���� 100 ~ 1000
	// 3 �ܰ� : ���� 1000, ī�޶� ���� 1000 ~ 10000
	// 4 �ܰ� : ���� 10000, ī�޶� ���� 10000 ~ 
	SharedPtr<CMaterial> pMtrl = MeshRenderer()->GetSharedMaterial();

	UINT iHeight = (UINT)abs(m_vToolCamPos.y);
	UINT iStep = 1;
	UINT i = 100;
	for (; iStep <= 4; i *= 10, ++iStep) {
		if (iHeight <= i)
			break;
	}

	m_fThickness = ((float)iHeight * 0.01f);
	m_fAlpha = abs(1.f - (float)((double)iHeight / pow(10., (double)iStep + 1.)));

	// Material �� ���� ����	
	pMtrl->SetData(E_ShaderParam::Int_0, &iStep);
	pMtrl->SetData(E_ShaderParam::Float_0, &m_fAlpha);
	pMtrl->SetData(E_ShaderParam::Float_1, &m_fThickness);
	pMtrl->SetData(E_ShaderParam::Vector4_0, &m_vToolCamPos);
	pMtrl->SetData(E_ShaderParam::Vector4_1, &m_vColor);
}