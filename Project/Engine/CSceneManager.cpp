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

	// 카메라 오브젝트 생성
	CGameObject* pCameraObj = new CGameObject();
	pCameraObj->AddComponent<CTransform>();
	pCameraObj->AddComponent<CCamera>();
	pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));

	m_pCurScene->AddGameObject(pCameraObj);

	// 물체 생성
	CGameObject* pObj = new CGameObject();

	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash);
	SharedPtr<CMaterial> pStdMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StandardMaterial);
	SharedPtr<CTexture> pTex = CResourceManager::GetInstance()->Load<CTexture>(_T("testTex"), _T("texture\\Player.png"));
	pStdMtrl->m_pTexture = pTex;

	pStdMtrl->SetData(E_ShaderParam::Texture_0, pTex.Get());
	int a = 1;
	pStdMtrl->SetData(E_ShaderParam::Int_0, &a);

	pObj->AddComponent<CTransform>();
	pObj->AddComponent<CMeshRenderer>();

	pObj->GetComponent<CMeshRenderer>()->SetMesh(pMesh);
	pObj->GetComponent<CMeshRenderer>()->SetMaterial(pStdMtrl);

	pObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, 100.f));
	pObj->GetComponent<CTransform>()->SetLocalScale(Vector3(100.f, 100.f, 1.f));
	pObj->GetComponent<CTransform>()->SetLocalRotation(Vector3(0.f, 0.f, 0.f * CMyMath::Deg2Rad()));

	m_pCurScene->AddGameObject(pObj);

	// Scene 초기화
	m_pCurScene->Awake();
	m_pCurScene->Start();
}

void CSceneManager::Progress()
{
	m_pCurScene->PrevUpdate();
	m_pCurScene->Update();
	m_pCurScene->LateUpdate();
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