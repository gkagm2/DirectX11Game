#include "pch.h"
#include "RenderTest.h"
#include "CResourceManager.h"
#include "CDevice.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CKeyManager.h"
#include "CTimeManager.h"

void Render_Test::TestUpdate()
{
	CMesh* pMesh = dynamic_cast<CMesh*>(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash));
	VTX* pVtx = (VTX*)pMesh->GetVtxSysMem();

	if (InputKeyHold(E_Key::A)) {
		for (int i = 0; i < 4; ++i) {
			VTX vtx = pVtx[i];
			pVtx[i].vPos.x -= DeltaTime * 4.f;
		}
	}
	if (InputKeyHold(E_Key::S)) {
		for (int i = 0; i < 4; ++i) {
			VTX vtx = pVtx[i];
			pVtx[i].vPos.y -= DeltaTime * 4.f;
		}
	}
	if (InputKeyHold(E_Key::D)) {
		for (int i = 0; i < 4; ++i) {
			VTX vtx = pVtx[i];
			pVtx[i].vPos.x += DeltaTime * 4.f;
		}
	}
	if (InputKeyHold(E_Key::W)) {
		for (int i = 0; i < 4; ++i) {
			VTX vtx = pVtx[i];
			pVtx[i].vPos.y += DeltaTime * 4.f;
		}
	}
	pMesh->Reset();
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
}