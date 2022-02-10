#include "pch.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CCollisionManager.h"
#include "CRenderManager.h"
#include "CEventManager.h"

#include "CResourceManager.h"
#include "CMeshRenderer.h"
#include "CTransform.h"
#include "CDevice.h"

CSceneManager::CSceneManager() :
	m_pCurScene(nullptr),
	m_pLoadingScene(nullptr),
	m_pLoadScript{},
	m_pSaveScript{},
	m_eSceneMode(E_SceneMode::Stop),
	m_iLoadSync(0),
	m_pNextScene(nullptr),
	m_pLoadSceneFunc(nullptr)
{
}

CSceneManager::~CSceneManager()
{
	if (nullptr != m_pCurScene)
		delete m_pCurScene;

	if (nullptr != m_pLoadingScene)
		delete m_pLoadingScene;
}

void CSceneManager::Init()
{
	// Loading Scene 생성 및 초기화
	m_pLoadingScene = new CScene;
	
	// 카메라 생성
	CGameObject* pCamera = new CGameObject;
	pCamera->SetName(_T("Loading Scene Camera"));
	pCamera->AddComponent<CCamera>();
	pCamera->AddComponent<CTransform>();
	pCamera->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	m_pLoadingScene->_AddGameObject(pCamera);

	CGameObject* pScreenBGObj = new CGameObject;
	pScreenBGObj->SetName(_T("Loading Scene Screen Background Game Object"));
	pScreenBGObj->AddComponent<CTransform>();
	pScreenBGObj->AddComponent<CMeshRenderer>();
	pScreenBGObj->MeshRenderer()->SetMesh(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh));
	pScreenBGObj->MeshRenderer()->SetMaterial(CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl));
	const Vector2& vRenderResolution =  CDevice::GetInstance()->GetRenderResolution();
	pScreenBGObj->Transform()->SetLocalScale(Vector3(vRenderResolution));

	m_pLoadingScene->_AddGameObject(pScreenBGObj);
}

void CSceneManager::Progress()
{
	// Scene Load 완료 시 LoadingScene -> NextScene으로 변경
	if (m_iLoadSync) {
		m_iLoadSync = 0;
		ChangeSceneEvt(m_pNextScene);
	}


	m_pCurScene->_UnRegisterAllObjects();

	switch (m_eSceneMode) {
	case E_SceneMode::Play:
		m_pCurScene->PrevUpdate();
		m_pCurScene->Update();
		m_pCurScene->LateUpdate();
		break;
	case E_SceneMode::Pause:
		break;
	case E_SceneMode::Stop:
		
		break;
	default:
		assert(nullptr);
		break;
	}

	CRenderManager::GetInstance()->UnRegisterCamera();

	// FinalUpdate 도중에 카메라가 등록 될 것임
	m_pCurScene->FinalUpdate();

	if(E_SceneMode::Play == m_eSceneMode)
		CCollisionManager::GetInstance()->Update();
}

void CSceneManager::Render()
{
	m_pCurScene->Render();
}

#include "CUIManager.h"
void CSceneManager::ChangeScene(CScene* _pNextScene)
{
	CUIManager::GetInstance()->Init();

	if (m_pCurScene && m_pCurScene != m_pLoadingScene)
		delete m_pCurScene;
	m_pCurScene = _pNextScene;
}

CGameObject* CSceneManager::FindGameObject(const tstring& _strName, UINT _iLayer)
{
	return GetCurScene()->FindGameObject(_strName, _iLayer);
}

void CSceneManager::ChangeSceneModeEvt(E_SceneMode _eSceneMode)
{
	TEvent evn = {};
	evn.eType = E_EventType::Change_SceneMode;
	evn.lparam = (DWORD_PTR)_eSceneMode;
	CEventManager::GetInstance()->AddEvent(evn);
}

void CSceneManager::ChangeSceneEvt(CScene* _pNextScene)
{
	TEvent evn = {};
	evn.eType = E_EventType::Change_Scene;
	evn.lparam = (DWORD_PTR)_pNextScene;
	CEventManager::GetInstance()->AddEvent(evn);
}

void CSceneManager::ChangeSceneEvt(const tstring& _pNextScenePath, bool _bRelativePath)
{
	TEvent evn = {};
	evn.eType = E_EventType::Change_Scene_InScript;
	evn.lparam = (DWORD_PTR)_pNextScenePath.data();
	evn.wparam = (DWORD_PTR)_bRelativePath;
	CEventManager::GetInstance()->AddEvent(evn);
}

void CSceneManager::SceneLoadStart()
{
	m_iLoadSync = 0; // 씽크 초기화
	// 기존에 있던 Scene 삭제
	if (m_pCurScene)
		delete m_pCurScene;

	// 스레드 로딩중일 때 노출시킬 LoadingScene을 잠시 현재 Scene으로 설정한다.
	m_pCurScene = m_pLoadingScene;
}

CScene* CSceneManager::CreateNewScene()
{
	CScene* pNewScene = new CScene;
	CSceneManager::GetInstance()->ChangeSceneEvt(pNewScene);
	return pNewScene;
}

void CSceneManager::_SetSceneMode(E_SceneMode _eSceneMode) {
	if (E_SceneMode::Play == _eSceneMode && E_SceneMode::Stop == m_eSceneMode) {
		m_pCurScene->Awake();
		m_pCurScene->Start();
	}
		
	m_eSceneMode = _eSceneMode;
}