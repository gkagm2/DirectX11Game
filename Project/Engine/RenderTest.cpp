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

CScene testScene;
CGameObject g_Obj;

Vector4 g_vPos;

void Render_Test::TestInit() {
	CTransform* pTransform = g_Obj.AddComponent<CTransform>();
	CMeshRenderer* pRenderer = g_Obj.AddComponent<CMeshRenderer>();
	CGraphicsShader* pShader = CResourceManager::GetInstance()->FindRes<CGraphicsShader>(STR_KEY_StandardShader);
	CMesh* pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash);

	pRenderer->SetMeshRenderer(pMesh, pShader);
	pTransform->SetLocalPosition(Vector3(0.f, 0.f, 0.5f));

	testScene.Awake();
	testScene.Start();
}

void Render_Test::TestUpdate()
{
	CMesh* pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash);
	VTX* pVtx = (VTX*)pMesh->GetVtxSysMem();

	Vector3 vPos = g_Obj.GetComponent<CTransform>()->GetLocalPosition();


	if (InputKeyHold(E_Key::A)) {
		for (int i = 0; i < 4; ++i) {
			vPos.x -= DeltaTime * 4.f;
		}
	}
	if (InputKeyHold(E_Key::S)) {
		for (int i = 0; i < 4; ++i) {
			vPos.y -= DeltaTime * 4.f;
		}
	} 
	if (InputKeyHold(E_Key::D)) {
		for (int i = 0; i < 4; ++i) {
			vPos.x += DeltaTime * 4.f;
		}
	}
	if (InputKeyHold(E_Key::W)) {
		for (int i = 0; i < 4; ++i) {
			vPos.y += DeltaTime * 4.f;
		}
	}
	g_Obj.GetComponent<CTransform>()->SetLocalPosition(vPos);
	g_Obj.LateUpdate();

	CGameObject* pObj = &g_Obj;
	testScene.AddGameObject(pObj);

	testScene.PrevUpdate();
	testScene.Update();
	testScene.LateUpdate();
}

void Render_Test::TestRender()
{
	testScene.Render();
	g_Obj.Render();
}