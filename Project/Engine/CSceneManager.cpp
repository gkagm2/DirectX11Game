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

// component
#include "CTransform.h"
#include "CMeshRenderer.h"
#include "CGraphicsShader.h"
#include "CCamera.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CCollider2DRect.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"
#include "Ptr.h"
#include "CLight2D.h"

// GameContents
#include "CGameManagerScript_sh.h"

#include "CPlayerScript_sh.h"
#include "CBulletScript_sh.h"
#include "CEnemyScript_sh.h"
#include "CEnemyRespawnerScript_sh.h"

#include "CTestLight2DScript.h"


CGameObject*  TestCreateObj() {
	SharedPtr<CTexture> pBoxTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Box);
	SharedPtr<CTexture> pPlayerTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Player);
	SharedPtr<CTexture> pEnemyTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Enemy1);

	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMash);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl);


	CGameObject* pObj = new CGameObject();
	pObj->AddComponent<CTransform>();
	pObj->AddComponent<CMeshRenderer>();

	pMtrl->SetData(E_ShaderParam::Texture_0, pBoxTexture.Get());

	pObj->MeshRenderer()->SetMaterial(pMtrl);
	pObj->MeshRenderer()->SetMesh(pMesh);

	pObj->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));

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

	// Prefab 등록
	CGameObject* pBulletPrefab = new CGameObject;
	pBulletPrefab->SetName(_T("Bullet"));
	pBulletPrefab->AddComponent<CTransform>();
	pBulletPrefab->AddComponent<CMeshRenderer>();
	pBulletPrefab->AddComponent<CBulletScript_sh>();

	pBulletPrefab->Transform()->SetLocalScale(Vector3(50.f, 50.f, 1.f));

	pBulletPrefab->MeshRenderer()->SetMesh(CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMash));
	pBulletPrefab->MeshRenderer()->SetMaterial(CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl));
	CBulletScript_sh* pBullet = pBulletPrefab->GetComponent<CBulletScript_sh>();
	pBullet->SetDirection(Vector3(0.f,1.f, 0.f));
	pBullet->SetBulletSpeed(800.f);

	pBulletPrefab->RegisterAsPrefab();
	delete pBulletPrefab;

	// TODO (Jang) : Test code
	// 씬 생성
	m_pCurScene = new CScene;

	SharedPtr<CTexture> pBoxTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Box);
	SharedPtr<CTexture> pPlayerTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Player);
	SharedPtr<CTexture> pEnemyTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Enemy1);
	SharedPtr<CTexture> pAnimTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Anim);

	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMash);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl);

	pMtrl->SetData(E_ShaderParam::Texture_0, pBoxTexture.Get());

	{
		// 카메라 오브젝트 생성
		CGameObject* pCameraObj = new CGameObject();
		pCameraObj->AddComponent<CTransform>();
		pCameraObj->AddComponent<CCamera>();
		pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
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
		CGameObject* pLight2DObj = new CGameObject;
		pLight2DObj->AddComponent<CTransform>();
		pLight2DObj->AddComponent<CMeshRenderer>();
		pLight2DObj->AddComponent<CLight2D>();
		pLight2DObj->AddComponent<CTestLight2DScript>();
		pLight2DObj->Light2D()->SetLightType(E_LightType::Spot);
		//pLight2DObj->Light2D()->SetLightType(E_LightType::Point);
		pLight2DObj->Transform()->SetLocalPosition(Vector3(300.f, 0.f, 0.f));
		pLight2DObj->Light2D()->SetDiffColor(Vector3(0.0f, 0.0f, 1.0f));
		pLight2DObj->Light2D()->SetRange(400.f);
		pLight2DObj->Light2D()->SetAngle(60.f);
		pLight2DObj->Light2D()->SetLightDir(Vector3(1.f, 0.f, 0.f));

		m_pCurScene->AddGameObject(pLight2DObj, E_Layer::Default);

		CGameObject* pLight2 = pLight2DObj->Clone();
		pLight2->Light2D()->SetLightType(E_LightType::Point);
		pLight2->Transform()->SetLocalPosition(Vector3(-300.f, 0.f, 0.f));
		pLight2->Light2D()->SetDiffColor(Vector3(1.0f, 0.0f, 0.0f));

		m_pCurScene->AddGameObject(pLight2);
	}

	CGameObject* pPlayer = TestCreateObj();
	// 플레이어 오브젝트 생성
	{
		pPlayer->AddComponent<CPlayerScript_sh>();
		pPlayer->AddComponent<CAnimator2D>();

		SharedPtr<CMaterial> pLightMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdLight2DMtrl);

		pPlayer->MeshRenderer()->SetMaterial(pLightMtrl);
		
		TAnimation2DDesc tAnimDesc;
		tAnimDesc.fDuration = 0.1f;
		tAnimDesc.iFrameCount = 10;
		tAnimDesc.pAtlas = pAnimTexture;
		tAnimDesc.strName = _T("Player_Walk");
		tAnimDesc.vBaseSize = Vector2{ 150.f,150.f };
		tAnimDesc.vFrameSize = Vector2{ 60.f,65.f };
		tAnimDesc.vLeftTop = Vector2(0.f, 4 * 65.f);

		pPlayer->Animator2D()->CreateAnimation(tAnimDesc);
		
		pPlayer->Animator2D()->Play(_T("Player_Walk"), E_AnimationState::Loop);
		CAnimation2D* pAnim2D = pPlayer->Animator2D()->FindAnimation(_T("Player_Walk"));
		
		//pAnim2D->Save(_T("anim\\"), _T("Player_Walk.anim"));
		
		/*pPlayer->Animator2D()->LoadAnimation(_T("anim\\Player_Walk.anim"));
		pPlayer->Animator2D()->Play(_T("Player_Walk"));*/

		CCollider2DRect* pCollider2D = pPlayer->AddComponent<CCollider2DRect>();
		CGameObject* pOwner = pCollider2D->GetGameObject();

		Vector2 vResolution = CCore::GetInstance()->GetWindowResolution();
		
		pPlayer->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
		pPlayer->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
		pPlayer->Transform()->SetLocalScale(Vector3(200.f, 200.f, 1.f));
		pPlayer->Collider2D()->SetOffsetPosition(Vector2(0.f, 0.f));
		m_pCurScene->AddGameObject(pPlayer, E_Layer::Player);


#pragma region 플레이어를 감싸고 있는 오브젝트
		/*CGameObject* pChild = TestCreateObj();

		pChild->Transform()->SetLocalPosition(Vector3(0.f, 1.f, 0.f));
		pChild->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
		pChild->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
		CObject::AddChildGameObjectEvn(pPlayer, pChild);*/
#pragma endregion
	}
	{
		CGameObject* pEnemy = TestCreateObj();
		CCollider2DRect* pCollider2D = pEnemy->AddComponent<CCollider2DRect>();
		CGameObject* pOwner = pCollider2D->GetGameObject();

		SharedPtr<CMaterial> pLightMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdLight2DMtrl);

		pEnemy->MeshRenderer()->SetMaterial(pLightMtrl);


		Vector2 vResolution = CCore::GetInstance()->GetWindowResolution();
		//pEnemy->AddComponent<CEnemyScript_sh>();
		pEnemy->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
		pEnemy->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
		pEnemy->Transform()->SetLocalScale(Vector3(5000.f, 5000.f, 1.f));
		pEnemy->Collider2D()->SetOffsetPosition(Vector2(0.f, 0.f));
		m_pCurScene->AddGameObject(pEnemy, E_Layer::Enemy);
	}
	{
		/*CGameObject* pEnemyRespawner = new CGameObject();
		pEnemyRespawner->AddComponent<CEnemyRespawnerScript_sh>();
		m_pCurScene->AddGameObject(pEnemyRespawner, E_Layer::Default, false);*/
	}

	// 레이어 충돌 지정
	CCollisionManager::GetInstance()->SetOnOffCollision(E_Layer::Player, E_Layer::Enemy, true);

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