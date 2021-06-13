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

#include "CGameObject.h"

// component
#include "CTransform.h"
#include "CMeshRenderer.h"
#include "CGraphicsShader.h"
#include "CCamera.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "Ptr.h"

// GameContents
#include "CGameManagerScript_sh.h"

#include "CPlayerScript_sh.h"
#include "CBulletScript_sh.h"
#include "CEnemyScript_sh.h"


CGameObject*  TestCreateObj() {
	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
	SharedPtr<CTexture> pPlayerTexture = CResourceManager::GetInstance()->FindRes<CTexture>(STR_PATH_Player);
	SharedPtr<CTexture> pEnemyTexture = CResourceManager::GetInstance()->FindRes<CTexture>(STR_PATH_Enemy1);

	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdMtrlAlphaBlend_Coverage);

	CGameObject* pObj = new CGameObject();
	pObj->AddComponent<CTransform>();
	pObj->AddComponent<CMeshRenderer>();

	pMtrl->SetData(E_ShaderParam::Texture_0, pPlayerTexture.Get());

	pObj->MeshRenderer()->SetMaterial(pMtrl);
	pObj->MeshRenderer()->SetMesh(pMesh);

	pObj->Transform()->SetLocalPosition(Vector3(0.f, -200.f, 0.f));

	Vector2 vTexSize = pPlayerTexture->GetDimension();
	pObj->Transform()->SetLocalScale(Vector3(vTexSize.x, vTexSize.y, 1.f));
	pObj->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));

	pCurScene->AddGameObject(pObj);
	return pObj;
}


CSceneManager::CSceneManager() :
	m_pCurScene(nullptr)
{
}

CSceneManager::~CSceneManager()
{
	if (nullptr != m_pCurScene)
		delete m_pCurScene;
}

void CSceneManager::Init() {
	// TODO (Jang) : Test code
	// 씬 생성
	m_pCurScene = new CScene;

	SharedPtr<CTexture> pPlayerTexture = CResourceManager::GetInstance()->Load<CTexture>(STR_PATH_Player, STR_PATH_Player);
	SharedPtr<CTexture> pEnemyTexture = CResourceManager::GetInstance()->Load<CTexture>(STR_PATH_Enemy1, STR_PATH_Enemy1);

	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdMtrlAlphaBlend_Coverage);

	pMtrl->SetData(E_ShaderParam::Texture_0, pEnemyTexture.Get());

	{
		// 카메라 오브젝트 생성
		CGameObject* pCameraObj = new CGameObject();
		pCameraObj->AddComponent<CTransform>();
		pCameraObj->AddComponent<CCamera>();
		pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));

		m_pCurScene->AddGameObject(pCameraObj);
	}
	
	{
		// 게임 매니저 오브젝트 생성
		CGameObject* pGameMgr = new CGameObject();
		pGameMgr->AddComponent<CGameManagerScript_sh>();
		m_pCurScene->AddGameObject(pGameMgr);
	}

	{
		// 플레이어 오브젝트 생성
		CGameObject* pPlayer = new CGameObject();
		pPlayer->AddComponent<CTransform>();
		pPlayer->AddComponent<CMeshRenderer>();
		pPlayer->AddComponent<CPlayerScript_sh>();
		

		pMtrl->SetData(E_ShaderParam::Texture_0, pPlayerTexture.Get());

		pPlayer->MeshRenderer()->SetMaterial(pMtrl);
		pPlayer->MeshRenderer()->SetMesh(pMesh);

		pPlayer->Transform()->SetLocalPosition(Vector3(0.f, -200.f, 0.f));

		Vector2 vTexSize = pPlayerTexture->GetDimension();
		pPlayer->Transform()->SetLocalScale(Vector3(vTexSize.x, vTexSize.y, 1.f));
		pPlayer->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));

		m_pCurScene->AddGameObject(pPlayer, E_Layer::Player);

#pragma region 플레이어를 감싸고 있는 오브젝트
		CGameObject* pChild = TestCreateObj();

		pChild->Transform()->SetLocalPosition(Vector3(0.f, 1.f, 0.f));
		pChild->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
		pChild->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));

#pragma endregion

	}

	{
		CGameObject* pEnemy = new CGameObject();
		pEnemy->AddComponent<CTransform>();
		pEnemy->AddComponent<CMeshRenderer>();
		pEnemy->AddComponent<CEnemyScript_sh>();

		pEnemy->MeshRenderer()->SetMaterial(pMtrl);
		pEnemy->MeshRenderer()->SetMesh(pMesh);

		pEnemy->Transform()->SetLocalPosition(Vector3(0., 200.f, 0));
		Vector2 vTexSize = pEnemyTexture->GetDimension();
		pEnemy->Transform()->SetLocalScale(Vector3(vTexSize.x, -vTexSize.y, 1.f));

		m_pCurScene->AddGameObject(pEnemy, E_Layer::Enemy);

	}
	
	// Scene 초기화
	m_pCurScene->Awake();
	m_pCurScene->Start();
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