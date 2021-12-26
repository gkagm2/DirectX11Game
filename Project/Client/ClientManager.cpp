#include "pch.h"
#include "ClientManager.h"
#include <Engine\CResourceManager.h>
#include <Engine\CPathManager.h>
#include <Engine\CCamera.h>
#include <Engine\CRenderManager.h>
#include <Engine\CCore.h>
#include <Engine\CKeyManager.h>

#include "CGameObjectEx.h"
#include <Engine\CMeshRenderer.h>
#include <Engine\CResourceManager.h>
#include <Engine\CRectTransform.h>
#include "CCursor.h"

ClientManager::ClientManager()
{
}

ClientManager::~ClientManager()
{
}

void ClientManager::SetCurCursorTex(UINT _idx)
{
	if (m_vecTex.size() <=_idx)
		return;
	m_pCurCursorTex = m_vecTex[_idx];
}

void ClientManager::Init()
{
	{
		m_pCam = new CGameObjectEx;
		m_pCam->AddComponent<CTransform>();
		TClippingPlanes tCP = {};
		tCP.fFar = 99999.f;
		tCP.fNear = 1.f;
		m_pCam->Camera()->SetClippingPlanes(tCP);
		m_pCam->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
		m_pCam->Camera()->SetLayerCheckAllUnActive();
		m_pCam->Camera()->SetLayerCheck(NUM_LAYER_CLIENT, true);
		m_pCam->Camera()->SetSize(1.f);
		m_pCam->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
		_AddGameObjectEx(m_pCam);
	}

	{
		m_pCursor = new CGameObjectEx;
		m_pCursor->AddComponent<CMeshRenderer>();
		m_pCursor->AddComponent<CTransform>();
		m_pCursor->AddComponent<CCursor>();
		SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh);
		SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdAlphaBlendMtrl);
		m_pCursor->MeshRenderer()->SetMesh(pMesh);
		m_pCursor->MeshRenderer()->SetMaterial(pMtrl);
		m_pCursor->Transform()->SetLocalScale(Vector3(999.f, 999.f, 1.f));

		_AddGameObjectEx(m_pCursor);

	}

	Init_Butcher();

	for (size_t i = 0; i < m_vecObjs.size(); ++i)
		m_vecObjs[i]->Awake();
	for (size_t i = 0; i < m_vecObjs.size(); ++i)
		m_vecObjs[i]->Start();
}

void ClientManager::Progress()
{
	for (size_t i = 0; i < m_vecObjs.size(); ++i)
		m_vecObjs[i]->PrevUpdate();
	for (size_t i = 0; i < m_vecObjs.size(); ++i)
		m_vecObjs[i]->Update();
	for (size_t i = 0; i < m_vecObjs.size(); ++i)
		m_vecObjs[i]->LateUpdate();


	for (size_t i = 0; i < m_vecObjs.size(); ++i)
		m_vecObjs[i]->FinalUpdate();
}

void ClientManager::Init_Butcher()
{
#ifdef _BUTCHER_GAME

	CTexture* pTex = nullptr;

	tstring strPath{};

	strPath = _T("texture\\Butcher\\Mouse\\Cursor.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Cursor_crosshair"), strPath).Get();
	assert(pTex);
	m_vecTex.push_back(pTex);

	strPath = _T("texture\\Butcher\\Mouse\\Chainsaw.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Chainsaw_crosshair"), strPath).Get();
	assert(pTex);
	m_vecTex.push_back(pTex);


	strPath = _T("texture\\Butcher\\Mouse\\Shotgun.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Shotgun_crosshair"), strPath).Get();
	assert(pTex);
	m_vecTex.push_back(pTex);

	strPath = _T("texture\\Butcher\\Mouse\\Machinegun.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Machinegun_crosshair"), strPath).Get();
	assert(pTex);
	m_vecTex.push_back(pTex);

	strPath = _T("texture\\Butcher\\Mouse\\Flamethrower.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Flamethrower_crosshair"), strPath).Get();
	assert(pTex);
	m_vecTex.push_back(pTex);

	strPath = _T("texture\\Butcher\\Mouse\\GrenadeLauncher.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("GrenadeLauncher_crosshair"), strPath).Get();
	assert(pTex);
	m_vecTex.push_back(pTex);

	strPath = _T("texture\\Butcher\\Mouse\\Railgun.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("Railgun_crosshair"), strPath).Get();
	assert(pTex);
	m_vecTex.push_back(pTex);

	strPath = _T("texture\\Butcher\\Mouse\\PoliceCarRockets.png");
	pTex = CResourceManager::GetInstance()->LoadRes<CTexture>(_T("PoliceCarRockets_crosshair"), strPath).Get();
	assert(pTex);
	m_vecTex.push_back(pTex);

	SharedPtr<CMaterial> pMtrl = m_pCursor->MeshRenderer()->GetSharedMaterial();
	pMtrl->SetData(E_ShaderParam::Texture_0, m_vecTex[0]);

#endif
}