#include "pch.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CCollisionManager.h"
#include "CRenderManager.h"
#include "CEventManager.h"

CSceneManager::CSceneManager() :
	m_pCurScene(nullptr),
	m_pLoadScript{},
	m_pSaveScript{},
	m_eSceneMode(E_SceneMode::Stop)
{
}

CSceneManager::~CSceneManager()
{
	if (nullptr != m_pCurScene)
		delete m_pCurScene;
}

void CSceneManager::Init() {
}

void CSceneManager::Progress()
{
	m_pCurScene->UnRegisterAllObjects();

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
	CRenderManager::GetInstance()->UnRegisterToolCamera();

	// FinalUpdate 도중에 카메라가 등록 될 것임
	m_pCurScene->FinalUpdate();

	if(E_SceneMode::Play == m_eSceneMode)
		CCollisionManager::GetInstance()->Update();
}

void CSceneManager::Render()
{
	m_pCurScene->Render();
}

void CSceneManager::ChangeScene(CScene* _pNextScene)
{
	if (m_pCurScene)
		delete m_pCurScene;
	m_pCurScene = _pNextScene;
}

CGameObject* CSceneManager::FindGameObject(const tstring& _strName, E_Layer _eLayer)
{
	return GetCurScene()->FindGameObject(_strName, _eLayer);
}

void CSceneManager::ChangeSceneModeEvt(E_SceneMode _eSceneMode)
{
	TEvent evn = {};
	evn.eType = E_EventType::Change_SceneMode;
	evn.lparam = (DWORD_PTR)_eSceneMode;
	CEventManager::GetInstance()->AddEvent(evn);
}