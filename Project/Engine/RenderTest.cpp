#include "pch.h"
#include "RenderTest.h"
#include "CResourceManager.h"
#include "CDevice.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CConstBuffer.h"

#include "CGameObject.h"

#include "CScene.h"

// component
#include "CTransform.h"
#include "CMeshRenderer.h"
#include "CGraphicsShader.h"
#include "CCamera.h"

CScene testScene;
CGameObject* pObj = nullptr;
void Render_Test::TestInit() {
	CGameObject* pCameraObj = new CGameObject();
	pCameraObj->AddComponent<CTransform>();
	pCameraObj->AddComponent<CCamera>();
	pCameraObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(500.f, 0.f, 0.f));
	testScene.AddGameObject(pCameraObj);

	pObj = new CGameObject();
	pObj->AddComponent<CTransform>();
	pObj->AddComponent<CMeshRenderer>();

	CGraphicsShader* pShader = CResourceManager::GetInstance()->FindRes<CGraphicsShader>(STR_KEY_StandardShader);
	CMesh* pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash);
	pObj->GetComponent<CMeshRenderer>()->SetMeshRenderer(pMesh, pShader);

	pObj->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.f, 0.f, 100.f));
	pObj->GetComponent<CTransform>()->SetLocalScale(Vector3(100.f, 100.f, 100.f));
	pObj->GetComponent<CTransform>()->SetLocalRotation(Vector3(60.f, 50.f, 50.f));
	testScene.AddGameObject(pObj);

	testScene.Awake();
	testScene.Start();
}

void Render_Test::TestUpdate()
{
	testScene.PrevUpdate();
	testScene.Update();
	testScene.LateUpdate();
}

void Render_Test::TestRender()
{
	testScene.Render();
}