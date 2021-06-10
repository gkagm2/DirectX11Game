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
	// �� ����
	m_pCurScene = new CScene;


	SharedPtr<CTexture> pPlayerTexture = CResourceManager::GetInstance()->Load<CTexture>(STR_PATH_Player, STR_PATH_Player);
	SharedPtr<CTexture> pEnemyTexture = CResourceManager::GetInstance()->Load<CTexture>(STR_PATH_Enemy1, STR_PATH_Enemy1);

	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdMtrlAlphaBlend_Coverage);

	pMtrl->SetData(E_ShaderParam::Texture_0, pEnemyTexture.Get());

	{
		// ī�޶� ������Ʈ ����
		CGameObject* pCameraObj = new CGameObject();
		pCameraObj->AddComponent<CTransform>();
		pCameraObj->AddComponent<CCamera>();
		pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));

		m_pCurScene->AddGameObject(pCameraObj);
	}
	
	{
		// ���� �Ŵ��� ������Ʈ ����
		CGameObject* pGameMgr = new CGameObject();
		pGameMgr->AddComponent<CGameManagerScript_sh>();
		m_pCurScene->AddGameObject(pGameMgr);
	}

	{
		// �÷��̾� ������Ʈ ����
		CGameObject* pPlayer = new CGameObject();
		pPlayer->AddComponent<CTransform>();
		pPlayer->AddComponent<CMeshRenderer>();
		pPlayer->AddComponent<CPlayerScript_sh>();
		

		pMtrl->SetData(E_ShaderParam::Texture_2, pPlayerTexture.Get());

		pPlayer->MeshRenderer()->SetMaterial(pMtrl);
		pPlayer->MeshRenderer()->SetMesh(pMesh);

		pPlayer->Transform()->SetLocalPosition(Vector3(0.f, -200.f, 0.f));

		Vector2 vTexSize = pPlayerTexture->GetDimension();
		pPlayer->Transform()->SetLocalScale(Vector3(vTexSize.x, vTexSize.y, 1.f));
		pPlayer->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));

		m_pCurScene->AddGameObject(pPlayer, E_Layer::Player);
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
	
	// Scene �ʱ�ȭ
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