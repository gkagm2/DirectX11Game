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

#include "CCore.h"

// component
#include "CTransform.h"
#include "CMeshRenderer.h"
#include "CGraphicsShader.h"
#include "CCamera.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CCollider2DRect.h"
#include "Ptr.h"

// GameContents
#include "CGameManagerScript_sh.h"

#include "CPlayerScript_sh.h"
#include "CBulletScript_sh.h"
#include "CEnemyScript_sh.h"
#include "CEnemyRespawnerScript_sh.h"


CGameObject*  TestCreateObj() {
	SharedPtr<CTexture> pBoxTexture = CResourceManager::GetInstance()->FindRes<CTexture>(STR_PATH_Box);
	SharedPtr<CTexture> pPlayerTexture = CResourceManager::GetInstance()->FindRes<CTexture>(STR_PATH_Player);
	SharedPtr<CTexture> pEnemyTexture = CResourceManager::GetInstance()->FindRes<CTexture>(STR_PATH_Enemy1);

	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdMtrlAlphaBlend_Coverage);

	CGameObject* pObj = new CGameObject();
	pObj->AddComponent<CTransform>();
	pObj->AddComponent<CMeshRenderer>();

	pMtrl->SetData(E_ShaderParam::Texture_0, pBoxTexture.Get());

	pObj->MeshRenderer()->SetMaterial(pMtrl);
	pObj->MeshRenderer()->SetMesh(pMesh);

	pObj->Transform()->SetLocalPosition(Vector3(0.f, -200.f, 0.f));

	Vector2 vTexSize = pBoxTexture->GetDimension();
	pObj->Transform()->SetLocalScale(Vector3(vTexSize.x, vTexSize.y, 1.f));
	pObj->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));

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
	// �� ����
	m_pCurScene = new CScene;

	SharedPtr<CTexture> pBoxTexture = CResourceManager::GetInstance()->FindRes<CTexture>(STR_PATH_Box);
	SharedPtr<CTexture> pPlayerTexture = CResourceManager::GetInstance()->FindRes<CTexture>(STR_PATH_Player);
	SharedPtr<CTexture> pEnemyTexture = CResourceManager::GetInstance()->FindRes<CTexture>(STR_PATH_Enemy1);

	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdMtrlAlphaBlend_Coverage);

	pMtrl->SetData(E_ShaderParam::Texture_0, pBoxTexture.Get());

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

	CGameObject* pPlayer = TestCreateObj();
	// �÷��̾� ������Ʈ ����
	{
		pPlayer->AddComponent<CPlayerScript_sh>();
		CCollider2DRect* pCollider2D = pPlayer->AddComponent<CCollider2DRect>();

		Vector2 vResolution = CCore::GetInstance()->GetWindowResolution();
		
		pPlayer->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
		pPlayer->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
		m_pCurScene->AddGameObject(pPlayer, E_Layer::Player);


#pragma region �÷��̾ ���ΰ� �ִ� ������Ʈ
		/*CGameObject* pChild = TestCreateObj();

		pChild->Transform()->SetLocalPosition(Vector3(0.f, 1.f, 0.f));
		pChild->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
		pChild->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
		CObject::AddChildGameObjectEvn(pPlayer, pChild);*/
#pragma endregion
	}
	{
		CGameObject* pEnemyRespawner = new CGameObject();
		pEnemyRespawner->AddComponent<CEnemyRespawnerScript_sh>();
		m_pCurScene->AddGameObject(pEnemyRespawner, E_Layer::Default, false);
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