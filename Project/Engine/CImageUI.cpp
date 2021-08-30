#include "pch.h"
#include "CImageUI.h"
#include "CCanvasRenderer.h"

CImageUI::CImageUI() :
	CUI(E_ComponentType::ImageUI)
{
}

CImageUI::~CImageUI()
{
}

void CImageUI::FinalUpdate()
{
	// ���ϴ� �ؽ��ķ� CanvasRenderer���� ������ �ϰ� ��.
	SharedPtr<CMaterial> mtrl = CanvasRenderer()->GetCloneMaterial();
	if (nullptr == mtrl.Get())
		mtrl = CanvasRenderer()->GetSharedMaterial();
	mtrl->SetData(E_ShaderParam::Texture_0, m_pTexture.Get());
	CanvasRenderer()->SetColor();
}

void CImageUI::UpdateData()
{
}

void CImageUI::Render()
{
}