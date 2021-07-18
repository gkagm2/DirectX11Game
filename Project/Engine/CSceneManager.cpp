#include "pch.h"
#include "CSceneManager.h"
#include "CScene.h"

#include "CResourceManager.h"
#include "CDevice.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CConstBuffer.h"
#include "CCollisionManager.h"

#include "CGameObject.h"

#include "CCore.h"

#include "CTestShader.h"

// component
#include "CTransform.h"
#include "CMeshRenderer.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CCamera.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CCollider2DRect.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"
#include "Ptr.h"
#include "CLight2D.h"
#include "CTileMap.h"
#include "CParticleSystem.h"

CSceneManager::CSceneManager() :
	m_pCurScene(nullptr),
	m_pLoadScript{},
	m_pSaveScript{}
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
	m_pCurScene->PrevUpdate();
	m_pCurScene->Update();
	m_pCurScene->LateUpdate();
	m_pCurScene->FinalUpdate();
}

void CSceneManager::Render()
{
	m_pCurScene->Render();
}

void CSceneManager::CreateScene(const tstring& _strSceneName) {

}

void CSceneManager::LoadScene(const tstring& _strSceneName)
{
}

void CSceneManager::ChangeScene(CScene* _pNextScene)
{
	if (m_pCurScene)
		delete m_pCurScene;
	m_pCurScene = _pNextScene;
}