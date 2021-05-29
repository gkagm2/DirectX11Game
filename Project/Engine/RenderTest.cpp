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

// component
#include "CTransform.h"
#include "CMeshRenderer.h"
#include "CGraphicsShader.h"

CGameObject g_Obj;

Vector4 g_vPos;

void Render_Test::TestInit() {
	CTransform* pTransform = g_Obj.AddComponent<CTransform>();
	CMeshRenderer* pRenderer = g_Obj.AddComponent<CMeshRenderer>();
	pRenderer = g_Obj.AddComponent<CMeshRenderer>();
	CGraphicsShader* pShader = CResourceManager::GetInstance()->FindRes<CGraphicsShader>(STR_KEY_StandardShader);
	CMesh* pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash);

	pRenderer->SetMesh(pMesh);
	//pRenderer->SetShader(pShader);

	pTransform->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
}

void Render_Test::TestUpdate()
{
	CMesh* pMesh = dynamic_cast<CMesh*>(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash));
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
}

void Render_Test::TestRender()
{
	////////// 아래와 같은 파이프 라인대로 설정함 //////////
	// Input Assembler -> Vertex Shader -> Rasterizer -> PixelShader -> OutputMerge
	
	CMesh* pMesh = dynamic_cast<CMesh*>(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash));
	CGraphicsShader* pShader = dynamic_cast<CGraphicsShader*>(CResourceManager::GetInstance()->FindRes< CGraphicsShader>(STR_KEY_StandardShader));
	
	pMesh->UpdateData();
	pShader->UpdateData();

	pMesh->Render();

	g_Obj.Render();
}