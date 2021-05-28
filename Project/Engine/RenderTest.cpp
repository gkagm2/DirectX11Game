#include "pch.h"
#include "RenderTest.h"
#include "CResourceManager.h"
#include "CDevice.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CConstBuffer.h"

Vector4 g_vPos;

void Render_Test::TestInit() {
}


void Render_Test::TestUpdate()
{
	CMesh* pMesh = dynamic_cast<CMesh*>(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash));
	VTX* pVtx = (VTX*)pMesh->GetVtxSysMem();


	if (InputKeyHold(E_Key::A)) {
		for (int i = 0; i < 4; ++i) {
			g_vPos.x -= DeltaTime * 4.f;
		}
	}
	if (InputKeyHold(E_Key::S)) {
		for (int i = 0; i < 4; ++i) {
			g_vPos.y -= DeltaTime * 4.f;
		}
	}
	if (InputKeyHold(E_Key::D)) {
		for (int i = 0; i < 4; ++i) {
			g_vPos.x += DeltaTime * 4.f;
		}
	}
	if (InputKeyHold(E_Key::W)) {
		for (int i = 0; i < 4; ++i) {
			g_vPos.y += DeltaTime * 4.f;
		}
	}
	// b0 �������Ϳ� ������� ���ε�
	const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::transform);
	pCB->SetData(&g_vPos);
	pCB->UpdateData(E_ShaderStage::vertex);

	//pMesh->Reset();
}

void Render_Test::TestRender()
{
	////////// �Ʒ��� ���� ������ ���δ�� ������ //////////
	// Input Assembler -> Vertex Shader -> Rasterizer -> PixelShader -> OutputMerge
	
	CMesh* pMesh = dynamic_cast<CMesh*>(CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash));
	CGraphicsShader* pShader = dynamic_cast<CGraphicsShader*>(CResourceManager::GetInstance()->FindRes< CGraphicsShader>(STR_KEY_StandardShader));
	
	pMesh->UpdateData();
	pShader->UpdateData();

	pMesh->Render();
}