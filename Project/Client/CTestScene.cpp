#include "pch.h"
#include "CTestScene.h"

#include "Engine\CSceneManager.h"
#include "Engine\CScene.h"

#include "Engine\CResourceManager.h"
#include "Engine\CDevice.h"
#include "Engine\CMesh.h"
#include "Engine\CGraphicsShader.h"
#include "Engine\CKeyManager.h"
#include "Engine\CTimeManager.h"
#include "Engine\CConstBuffer.h"
#include "Engine\CCollisionManager.h"

#include "Engine\CGameObject.h"

#include "Engine\CCore.h"

#include "Engine\CTestShader.h"

// component
#include "Engine\CTransform.h"
#include "Engine\CMeshRenderer.h"
#include "Engine\CGraphicsShader.h"
#include "Engine\CComputeShader.h"
#include "Engine\CCamera.h"
#include "Engine\CTexture.h"
#include "Engine\CMaterial.h"
#include "Engine\CCollider2DRect.h"
#include "Engine\CAnimator2D.h"
#include "Engine\CAnimation2D.h"
#include "Engine\Ptr.h"
#include "Engine\CLight2D.h"
#include "Engine\CTileMap.h"
#include "Engine\CParticleSystem.h"
#include "Engine\CPrefab.h"
#include "Engine\CRigidbody2D.h"

// GameContents
#include "Script\CGameManagerScript_sh.h"

#include "Script\CPlayerScript_sh.h"
#include "Script\CBulletScript_sh.h"
#include "Script\CEnemyScript_sh.h"
#include "Script\CEnemyRespawnerScript_sh.h"

#include "Script\CTestLight2DScript.h"
#include "Script\CRotateZScript.h"

#include "CSceneSaveLoad.h"



CGameObject* TestCreateObj() {
	SharedPtr<CTexture> pBoxTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Box);
	SharedPtr<CTexture> pPlayerTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Player);
	SharedPtr<CTexture> pEnemyTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Enemy1);

	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh);
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

CTestScene::CTestScene()
{
}

CTestScene::~CTestScene()
{
}

void CTestScene::CreateTestScene()
{
	CSceneManager::GetInstance()->ChangeSceneModeEvt(E_SceneMode::Stop);
	ImGuiTest();
	return;
	// TODO (Jang) : Test code
	// �� ����

	CScene* pNewScene = new CScene;


	SharedPtr<CTexture> pBoxTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Box);
	SharedPtr<CTexture> pPlayerTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Player);
	SharedPtr<CTexture> pEnemyTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Enemy1);
	SharedPtr<CTexture> pAnimTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Anim);

	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl);

	pMtrl->SetData(E_ShaderParam::Texture_0, pBoxTexture.Get());

	{
		// ī�޶� ������Ʈ ����
		CGameObject* pCameraObj = new CGameObject();
		pCameraObj->AddComponent<CTransform>();
		pCameraObj->AddComponent<CCamera>();
		pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
		pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));

		CObject::CreateGameObjectEvn(pCameraObj, E_Layer::Default);
	}
	//{
	//	// ���� �Ŵ��� ������Ʈ ����
	//	CGameObject* pGameMgr = new CGameObject();
	//	pGameMgr->AddComponent<CGameManagerScript_sh>();
	//	m_pCurScene->AddGameObject(pGameMgr);
	//}

	CGameObject* pPlayer = TestCreateObj();
	// �÷��̾� ������Ʈ ����
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
		pPlayer->Collider2D()->SetOffsetPosition(Vector2(0.f, 0.f));
		pPlayer->Collider2D()->SetOffsetScale(Vector2(0.5f, 0.5f));

		pPlayer->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
		pPlayer->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
		pPlayer->Transform()->SetLocalScale(Vector3(200.f, 200.f, 1.f));
		
		CObject::CreateGameObjectEvn(pPlayer, E_Layer::Player);
	}
	{
		// ComputeShader Test 
		SharedPtr<CTexture> pTestTexture = CResourceManager::GetInstance()->CreateTexture(_T("Test"), 500, 500,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);

		SharedPtr<CTestShader> pCS = (CTestShader*)(CResourceManager::GetInstance()->FindRes<CComputeShader>(STR_KEY_TestComputeShader).Get());
		pCS->SetClearColor(Vector4(1.f, 0.f, 1.f, 1.f));
		pCS->SetTexture(pTestTexture);
		pCS->Excute();

		CGameObject* pObj = TestCreateObj();
		pObj->MeshRenderer()->SetMesh(pMesh);
		pObj->MeshRenderer()->SetMaterial(CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl));
		pObj->MeshRenderer()->GetSharedMaterial()->SetData(E_ShaderParam::Texture_0, pTestTexture.Get());
		CObject::CreateGameObjectEvn(pObj, E_Layer::Default);
	}


#pragma region �÷��̾ ���ΰ� �ִ� ������Ʈ
		/*CGameObject* pChild = TestCreateObj();

		pChild->Transform()->SetLocalPosition(Vector3(0.f, 1.f, 0.f));
		pChild->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
		pChild->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
		CObject::AddChildGameObjectEvn(pPlayer, pChild);*/
#pragma endregion
		//}
		//{
		//	CGameObject* pEnemy = TestCreateObj();
		//	CCollider2DRect* pCollider2D = pEnemy->AddComponent<CCollider2DRect>();
		//	CGameObject* pOwner = pCollider2D->GetGameObject();

		//	SharedPtr<CMaterial> pLightMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdLight2DMtrl);

		//	pEnemy->MeshRenderer()->SetMaterial(pLightMtrl);


		//	Vector2 vResolution = CCore::GetInstance()->GetWindowResolution();
		//	//pEnemy->AddComponent<CEnemyScript_sh>();
		//	pEnemy->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
		//	pEnemy->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
		//	pEnemy->Transform()->SetLocalScale(Vector3(5000.f, 5000.f, 1.f));
		//	pEnemy->Collider2D()->SetOffsetPosition(Vector2(0.f, 0.f));
		//	pNewScene->AddGameObject(pEnemy, E_Layer::Enemy);
		//}
		//{
		//	/*CGameObject* pEnemyRespawner = new CGameObject();
		//	pEnemyRespawner->AddComponent<CEnemyRespawnerScript_sh>();
		//	m_pCurScene->AddGameObject(pEnemyRespawner, E_Layer::Default, false);*/
		//}


	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::SceneStart()
{
	// �� ����
	CScene* pNewScene = new CScene;

	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::Camera()
{	
	// �� ����
	CScene* pNewScene = new CScene;

	// ī�޶� ������Ʈ ����
	CGameObject* pCameraObj = new CGameObject();
	pCameraObj->AddComponent<CTransform>();
	pCameraObj->AddComponent<CCamera>();
	pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));
	CObject::CreateGameObjectEvn(pCameraObj, E_Layer::Default);


	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::RenderingBoxObject()
{
	CScene* pNewScene = new CScene;

	// ī�޶� ������Ʈ ����
	CGameObject* pCameraObj = new CGameObject();
	pCameraObj->AddComponent<CTransform>();
	pCameraObj->AddComponent<CCamera>();
	pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));
	CObject::CreateGameObjectEvn(pCameraObj, E_Layer::Default);

	// ������Ʈ ����
	SharedPtr<CTexture> pBoxTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Box);
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl);

	CGameObject* pObj = new CGameObject();
	pObj->AddComponent<CTransform>();
	pObj->AddComponent<CMeshRenderer>();

	pMtrl->SetData(E_ShaderParam::Texture_0, pBoxTexture.Get());

	pObj->MeshRenderer()->SetMaterial(pMtrl);
	pObj->MeshRenderer()->SetMesh(pMesh);

	pObj->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));

	// Texture ������� ũ��� ����
	/*Vector2 vTexSize = pBoxTexture->GetDimension();
	pObj->Transform()->SetLocalScale(Vector3(vTexSize.x, vTexSize.y, 1.f));*/

	// ���Ƿ� ũ�� ����
	pObj->Transform()->SetLocalScale(Vector3(100.f, 100.f, 1.f));

	pObj->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
	CObject::CreateGameObjectEvn(pObj, E_Layer::Default);

	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::RenderingAnimationTexture()
{
	CScene* pNewScene = new CScene;

	// ī�޶� ������Ʈ ����
	CGameObject* pCameraObj = new CGameObject();
	pCameraObj->AddComponent<CTransform>();
	pCameraObj->AddComponent<CCamera>();
	pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));
	CObject::CreateGameObjectEvn(pCameraObj, E_Layer::Default);


	// �÷��̾� ������Ʈ ����
	SharedPtr<CTexture> pPlayerTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Player);
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl);



	CGameObject* pPlayer = new CGameObject();
	pPlayer->AddComponent<CTransform>();
	pPlayer->AddComponent<CMeshRenderer>();
	//pPlayer->AddComponent<CPlayerScript_sh>();
	pPlayer->AddComponent<CAnimator2D>();

	pMtrl->SetData(E_ShaderParam::Texture_0, pPlayerTexture.Get());

	pPlayer->MeshRenderer()->SetMaterial(pMtrl);
	pPlayer->MeshRenderer()->SetMesh(pMesh);

	SharedPtr<CTexture> pAnimTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Anim);
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

	pPlayer->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
	pPlayer->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
	pPlayer->Transform()->SetLocalScale(Vector3(200.f, 200.f, 1.f));
	pPlayer->Collider2D()->SetOffsetPosition(Vector2(0.f, 0.f));
	CObject::CreateGameObjectEvn(pPlayer, E_Layer::Player);


	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::SceneSaveLoadTest()
{
	// Load test
	// Scene Save Load Test
	bool isSave = true; // <- true�� false�� �̿��ؼ� save, load�ϱ�

	CScene* pNewScene = nullptr;
	tstring strRelPath = _T("scene\\test.scene");
	if (isSave) {
		// Save scene
		pNewScene = new CScene;
	}
	else {
		// Load scene
		pNewScene = CSceneSaveLoad::LoadScene(strRelPath);
	}

	if (isSave) {
		// ī�޶� ������Ʈ ����
		CGameObject* pCameraObj = new CGameObject();
		pCameraObj->AddComponent<CTransform>();
		pCameraObj->AddComponent<CCamera>();
		pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
		pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));
		CObject::CreateGameObjectEvn(pCameraObj, E_Layer::Default);

		CGameObject* pPlayer = TestCreateObj();
		// �÷��̾� ������Ʈ ����
		{
			pPlayer->AddComponent<CPlayerScript_sh>();
			pPlayer->AddComponent<CAnimator2D>();

			SharedPtr<CMaterial> pLightMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdLight2DMtrl);
			SharedPtr<CTexture> pAnimTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Anim);

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
			Vector2 vResolution = CCore::GetInstance()->GetWindowResolution();
			pPlayer->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
			pPlayer->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
			pPlayer->Transform()->SetLocalScale(Vector3(200.f, 200.f, 1.f));

			// Collider2D
			pPlayer->AddComponent<CCollider2DRect>();
			pPlayer->Collider2D()->SetOffsetScale(Vector2(60.f, 60.f));

			CObject::CreateGameObjectEvn(pPlayer, E_Layer::Default);
		}
	}

	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();

	if (isSave)
		CSceneSaveLoad::SaveScene(pNewScene, strRelPath);

	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::MouseMovementTest()
{
	// �� ����
	CScene* pNewScene = new CScene;

	// ī�޶� ������Ʈ ����
	CGameObject* pCameraObj = new CGameObject();
	pCameraObj->AddComponent<CTransform>();
	pCameraObj->AddComponent<CCamera>();
	pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));
	CObject::CreateGameObjectEvn(pCameraObj, E_Layer::Default);


	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::ImGuiTest()
{
	// �� ����
	CScene* pNewScene = new CScene;

	// ī�޶� ������Ʈ ����
	CGameObject* pCameraObj = new CGameObject();
	pCameraObj->AddComponent<CTransform>();
	pCameraObj->AddComponent<CCamera>();
	pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));
	pCameraObj->SetName(_T("Camera"));
	CObject::CreateGameObjectEvn(pCameraObj, E_Layer::Default);


	// �÷��̾�� �̸��� ���� ������Ʈ ����
	SharedPtr<CTexture> pBoxTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Box);
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl);

	CGameObject* pObj = new CGameObject();
	pObj->AddComponent<CTransform>();
	pObj->AddComponent<CMeshRenderer>();

	pMtrl->SetData(E_ShaderParam::Texture_0, pBoxTexture.Get());

	pObj->MeshRenderer()->SetMaterial(pMtrl);
	pObj->MeshRenderer()->SetMesh(pMesh);

	pObj->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));

	// Texture ������� ũ��� ����
	/*Vector2 vTexSize = pBoxTexture->GetDimension();
	pObj->Transform()->SetLocalScale(Vector3(vTexSize.x, vTexSize.y, 1.f));*/

	// ���Ƿ� ũ�� ����
	pObj->Transform()->SetLocalScale(Vector3(100.f, 100.f, 1.f));
	pObj->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
	pObj->SetName(_T("Player"));
	
	// Collider ����
	pObj->AddComponent<CCollider2DRect>();
	pObj->Collider2D()->SetOffsetScale(Vector2(1.f, 1.f));

	// Rigidobdy2D ����
	pObj->AddComponent<CRigidbody2D>();
	pObj->Rigidbody2D()->UseGravity(false);
	
	CObject::CreateGameObjectEvn(pObj, E_Layer::Default);

	///////////////////
	// �ִϸ��̼� ����� �ִ� ������Ʈ ����
	SharedPtr<CTexture> pPlayerTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Player);
	pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh);
	pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl);

	CGameObject* pPlayer = new CGameObject();
	pPlayer->AddComponent<CTransform>();
	pPlayer->AddComponent<CMeshRenderer>();
	//pPlayer->AddComponent<CPlayerScript_sh>();
	pPlayer->AddComponent<CAnimator2D>();

	pMtrl->SetData(E_ShaderParam::Texture_0, pPlayerTexture.Get());

	pPlayer->MeshRenderer()->SetMaterial(pMtrl);
	pPlayer->MeshRenderer()->SetMesh(pMesh);

	SharedPtr<CTexture> pAnimTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Anim);
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

	pPlayer->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
	pPlayer->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
	pPlayer->Transform()->SetLocalScale(Vector3(200.f, 200.f, 1.f));
	pPlayer->Collider2D()->SetOffsetPosition(Vector2(0.f, 0.f));
	CObject::CreateGameObjectEvn(pPlayer, E_Layer::Player);


	/////////////////////



	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::SceneSaveLoadPrefabTest()
{
	// Load test
	// Scene Save Load Test
	bool isSave = false; // <- true�� false�� �̿��ؼ� save, load�ϱ�

	CScene* pNewScene = nullptr;
	tstring strRelPath = _T("scene\\test.scene");
	if (isSave) {
		// Save scene
		pNewScene = new CScene;
	}
	else {
		// Load scene
		pNewScene = CSceneSaveLoad::LoadScene(strRelPath);
	}

	if (isSave) {
		// Prefab ���
		CGameObject* pBulletPrefab = new CGameObject;
		pBulletPrefab->SetName(_T("Bullet"));
		pBulletPrefab->AddComponent<CTransform>();
		pBulletPrefab->AddComponent<CMeshRenderer>();
		pBulletPrefab->AddComponent<CBulletScript_sh>();

		pBulletPrefab->Transform()->SetLocalScale(Vector3(50.f, 50.f, 1.f));

		pBulletPrefab->MeshRenderer()->SetMesh(CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh));
		pBulletPrefab->MeshRenderer()->SetMaterial(CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl));
		CBulletScript_sh* pBullet = pBulletPrefab->GetComponent<CBulletScript_sh>();
		pBullet->SetDirection(Vector3(0.f, 1.f, 0.f));
		pBullet->SetBulletSpeed(800.f);
		CMaterial* pMaterial = pBulletPrefab->MeshRenderer()->GetSharedMaterial()->Clone();
		pBulletPrefab->MeshRenderer()->SetMaterial(pMaterial);

		SharedPtr<CTexture> pBulletTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Box);
		pBulletPrefab->MeshRenderer()->GetCloneMaterial()->SetData(E_ShaderParam::Texture_0, pBulletTexture.Get());

		pBulletPrefab->RegisterAsPrefab();
		delete pBulletPrefab;

		SharedPtr<CPrefab> pRes = CResourceManager::GetInstance()->FindRes<CPrefab>(_T("Bullet"));
		pRes->Save(_T("prefab\\bullet.pref"));
	}
	else {
		CResourceManager::GetInstance()->LoadRes<CPrefab>(_T("Bullet"), _T("prefab\\bullet.pref"));
	}

	if (isSave) {
		// ī�޶� ������Ʈ ����
		CGameObject* pCameraObj = new CGameObject();
		pCameraObj->AddComponent<CTransform>();
		pCameraObj->AddComponent<CCamera>();
		pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
		pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));
		CObject::CreateGameObjectEvn(pCameraObj, E_Layer::Default);

		CGameObject* pPlayer = TestCreateObj();
		// �÷��̾� ������Ʈ ����
		{
			pPlayer->AddComponent<CPlayerScript_sh>();
			pPlayer->AddComponent<CAnimator2D>();

			SharedPtr<CMaterial> pLightMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdLight2DMtrl);
			SharedPtr<CTexture> pAnimTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Anim);

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
			Vector2 vResolution = CCore::GetInstance()->GetWindowResolution();
			pPlayer->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
			pPlayer->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
			pPlayer->Transform()->SetLocalScale(Vector3(200.f, 200.f, 1.f));

			// Collider2D
			pPlayer->AddComponent<CCollider2DRect>();
			pPlayer->Collider2D()->SetOffsetScale(Vector2(60.f, 60.f));

			CObject::CreateGameObjectEvn(pPlayer, E_Layer::Default);
		}
	}

	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();

	if (isSave)
		CSceneSaveLoad::SaveScene(pNewScene, strRelPath);

	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::FileSaveLoadTest()
{
	CSceneSaveLoad::TestSave();
	CSceneSaveLoad::TestLoad();

	CScene* pNewScene = new CScene;
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::PlayerSaveLoad()
{
	// �� ����
	CScene* pNewScene = new CScene;


	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::AnimationSaveLoad()
{
}

void CTestScene::Light2DTest()
{
	// �� ����
	CScene* pNewScene = new CScene;

	// ī�޶� ������Ʈ ����
	CGameObject* pCameraObj = new CGameObject();
	pCameraObj->AddComponent<CTransform>();
	pCameraObj->AddComponent<CCamera>();
	pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));
	CObject::CreateGameObjectEvn(pCameraObj, E_Layer::Default);

	//// �÷��̾� ������Ʈ ����
	SharedPtr<CTexture> pAnimTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Anim);

	CGameObject* pPlayer = new CGameObject;
	pPlayer->AddComponent<CPlayerScript_sh>();
	pPlayer->AddComponent<CAnimator2D>();
	pPlayer->AddComponent<CMeshRenderer>();
	pPlayer->AddComponent<CTransform>();

	
	// Light�� ���� �� �ִ� ���͸���� �����Ѵ�.
	SharedPtr<CMaterial> pLightMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdLight2DMtrl);
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh);

	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl);


	pPlayer->MeshRenderer()->SetMesh(pMesh);
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

	pPlayer->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
	pPlayer->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
	pPlayer->Transform()->SetLocalScale(Vector3(200.f, 200.f, 1.f));
	CObject::CreateGameObjectEvn(pPlayer, E_Layer::Player);

	// Spot Light ����
	CGameObject* pSpotLight = new CGameObject;
	pSpotLight->AddComponent<CTransform>();
	pSpotLight->AddComponent<CMeshRenderer>();
	pSpotLight->AddComponent<CLight2D>();
	pSpotLight->AddComponent<CTestLight2DScript>();
	pSpotLight->Light2D()->SetLightType(E_LightType::Spot);
	//pLight2DObj->Light2D()->SetLightType(E_LightType::Point);
	pSpotLight->Transform()->SetLocalPosition(Vector3(300.f, 0.f, 0.f));
	pSpotLight->Light2D()->SetDiffColor(Vector3(0.0f, 0.0f, 1.0f));
	pSpotLight->Light2D()->SetRange(400.f);
	pSpotLight->Light2D()->SetAngle(60.f);
	pSpotLight->Light2D()->SetLightDir(Vector3(1.f, 0.f, 0.f));

	CObject::CreateGameObjectEvn(pSpotLight, E_Layer::Default);

	// Point Light ����
	CGameObject* pPointLight = pSpotLight->Clone();
	pPointLight->Light2D()->SetLightType(E_LightType::Point);
	pPointLight->Transform()->SetLocalPosition(Vector3(-300.f, 0.f, 0.f));
	pPointLight->Light2D()->SetDiffColor(Vector3(1.0f, 0.0f, 0.0f));
	CObject::CreateGameObjectEvn(pPointLight, E_Layer::Default);


	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::TileMapTest()
{
	// �� ����
	CScene* pNewScene = new CScene;

	// ī�޶� ������Ʈ ����
	CGameObject* pCameraObj = new CGameObject();
	pCameraObj->AddComponent<CTransform>();
	pCameraObj->AddComponent<CCamera>();
	pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));
	CObject::CreateGameObjectEvn(pCameraObj, E_Layer::Default);

	// TileMap 1 
	CGameObject* pTileMap = new CGameObject();
	pTileMap->SetName(_T("TileMap"));
	pTileMap->AddComponent<CTransform>();
	pTileMap->AddComponent<CTileMap>();
	pTileMap->Transform()->SetLocalScale(Vector3(500.f, 500.f, 1.f));
	pTileMap->Transform()->SetLocalPosition(Vector3(-400.f, 0.f, 0.f));

	pTileMap->TileMap()->SetTileAtlas(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("texture\\WallTile64.bmp")), Vector2(64.f, 64.f));
	CObject::CreateGameObjectEvn(pTileMap, E_Layer::Default);


	// TileMap 2
	CGameObject* pTileMap2 = new CGameObject();
	pTileMap2->SetName(_T("TileMap"));
	pTileMap2->AddComponent<CTransform>();
	pTileMap2->AddComponent<CTileMap>();
	pTileMap2->Transform()->SetLocalScale(Vector3(500.f, 500.f, 1.f));
	pTileMap2->Transform()->SetLocalPosition(Vector3(400.f, 0.f, 0.f));

	pTileMap2->TileMap()->SetTileAtlas(CResourceManager::GetInstance()->LoadRes<CTexture>(_T("texture\\WallTile64.bmp")), Vector2(64.f, 64.f));
	pTileMap2->TileMap()->CreateTile(7, 7);
	CObject::CreateGameObjectEvn(pTileMap2, E_Layer::Default);


	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::ChildObjectTest()
{
}

void CTestScene::TextureTest()
{
}

void CTestScene::PrefabRegisterTest()
{
	CScene* pNewScene = new CScene;
	
	// ī�޶� ������Ʈ ����
	CGameObject* pCameraObj = new CGameObject();
	pCameraObj->AddComponent<CTransform>();
	pCameraObj->AddComponent<CCamera>();
	pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));
	CObject::CreateGameObjectEvn(pCameraObj, E_Layer::Default);

	CGameObject* pPlayer = new CGameObject;
	pPlayer->AddComponent<CTransform>();
	pPlayer->AddComponent<CPlayerScript_sh>();
	pPlayer->AddComponent<CMeshRenderer>();
	pPlayer->MeshRenderer()->SetMaterial(CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl));
	pPlayer->MeshRenderer()->SetMesh(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh));
	CObject::CreateGameObjectEvn(pPlayer, E_Layer::Default);

	SharedPtr<CTexture> pPlayerTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Player);
	pPlayer->MeshRenderer()->GetSharedMaterial()->SetData(E_ShaderParam::Texture_0, pPlayerTexture.Get());

	pPlayer->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
	pPlayer->Transform()->SetLocalScale(Vector3(60.f, 60.f, 1.f));


	// Prefab ���
	CGameObject* pBulletPrefab = new CGameObject;
	pBulletPrefab->SetName(_T("Bullet"));
	pBulletPrefab->AddComponent<CTransform>();
	pBulletPrefab->AddComponent<CMeshRenderer>();
	pBulletPrefab->AddComponent<CBulletScript_sh>();

	pBulletPrefab->Transform()->SetLocalScale(Vector3(50.f, 50.f, 1.f));

	pBulletPrefab->MeshRenderer()->SetMesh(CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh));
	pBulletPrefab->MeshRenderer()->SetMaterial(CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl));
	CBulletScript_sh* pBullet = pBulletPrefab->GetComponent<CBulletScript_sh>();
	pBullet->SetDirection(Vector3(0.f, 1.f, 0.f));
	pBullet->SetBulletSpeed(800.f);
	CMaterial* pMaterial = pBulletPrefab->MeshRenderer()->GetSharedMaterial()->Clone();
	pBulletPrefab->MeshRenderer()->SetMaterial(pMaterial);

	SharedPtr<CTexture> pBulletTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Box);
	pBulletPrefab->MeshRenderer()->GetCloneMaterial()->SetData(E_ShaderParam::Texture_0, pBulletTexture.Get());

	pBulletPrefab->RegisterAsPrefab();
	delete pBulletPrefab;

	// CPlayerScript_sh �ȿ� ���� �̻��� �������� ����.

	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);

}

void CTestScene::ParticleSystemTest()
{
	CScene* pNewScene = new CScene;

	// ī�޶� ������Ʈ ����
	CGameObject* pCameraObj = new CGameObject();
	pCameraObj->AddComponent<CTransform>();
	pCameraObj->AddComponent<CCamera>();
	pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));

	CObject::CreateGameObjectEvn(pCameraObj, E_Layer::Default);

	// Particle System Example
	CGameObject* pParticleObj = new CGameObject();
	pParticleObj->SetName(_T("particle object"));
	pParticleObj->AddComponent<CTransform>();
	pParticleObj->AddComponent<CParticleSystem>();

	pParticleObj->Transform()->SetLocalPosition(Vector3(100.f, 0.f, 100.f));
	pParticleObj->Transform()->SetLocalScale(Vector3(25.f, 25.f, 1.f));
	CObject::CreateGameObjectEvn(pParticleObj, E_Layer::Default);

	// ��ƼŬ ����
	pParticleObj = pParticleObj->Clone();
	pParticleObj->Transform()->SetLocalPosition(Vector3(-100.f, 0.f, 100.f));
	CObject::CreateGameObjectEvn(pParticleObj, E_Layer::Default);

	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}

void CTestScene::Collision2DTest()
{
	CScene* pNewScene = new CScene;

	// ī�޶� ������Ʈ ����
	CGameObject* pCameraObj = new CGameObject();
	pCameraObj->AddComponent<CTransform>();
	pCameraObj->AddComponent<CCamera>();
	pCameraObj->Camera()->SetProjectionType(E_ProjectionType::Orthographic);
	pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, -100.f));
	CObject::CreateGameObjectEvn(pCameraObj, E_Layer::Default);

	// ������Ʈ ����
	SharedPtr<CTexture> pBoxTexture = CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Box);
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectMesh);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl);

	CGameObject* pObj = new CGameObject();
	pObj->AddComponent<CTransform>();
	pObj->AddComponent<CMeshRenderer>();
	pObj->AddComponent<CCollider2DRect>();
	CCollider2DRect* pColRect = (CCollider2DRect*)pObj->GetComponent<CCollider2D>();

	
	pMtrl->SetData(E_ShaderParam::Texture_0, pBoxTexture.Get());

	pObj->MeshRenderer()->SetMaterial(pMtrl);
	pObj->MeshRenderer()->SetMesh(pMesh);

	pObj->Transform()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));

	// ���Ƿ� ũ�� ����
	pObj->Transform()->SetLocalScale(Vector3(100.f, 100.f, 1.f));
	pObj->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));

	// �浹���� ����
	pObj->Collider2D()->SetOffsetPosition(Vector2(0.f, 0.f));
	pObj->Collider2D()->SetOffsetScale(Vector2(1.f, 1.f));
	CObject::CreateGameObjectEvn(pObj, E_Layer::Player);

	
	// �ٸ� ������Ʈ ����
	CGameObject* pObj2 = pObj->Clone();

	pObj2->AddComponent<CRotateZScript>();
	pObj2->Transform()->SetLocalPosition(Vector3(120.f, 0.f, 0.f));
	pObj->Transform()->SetLocalRotation(Vector3(0.f, 0.f, 45.f));
	CObject::CreateGameObjectEvn(pObj2, E_Layer::Enemy);


	// ���̾� �浹 ����
	CCollisionManager::GetInstance()->SetOnOffCollision(E_Layer::Player, E_Layer::Enemy, true);

	// Scene �ʱ�ȭ
	pNewScene->Awake();
	pNewScene->Start();
	CSceneManager::GetInstance()->ChangeScene(pNewScene);
}