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
	// 원하느 텍스쳐로 CanvasRenderer에서 렌더링 하게 됨.
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