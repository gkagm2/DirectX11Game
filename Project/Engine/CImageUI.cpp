#include "pch.h"
#include "CImageUI.h"
#include "CCanvasRenderer.h"

CImageUI::CImageUI() :
	CUI(E_ComponentType::ImageUI),
	m_iColor{COLOR_RGBA(0,0,0,255)}
{
}

CImageUI::~CImageUI()
{
}

void CImageUI::FinalUpdate()
{
}

void CImageUI::UpdateData()
{
}

void CImageUI::Render()
{
}

void CImageUI::SetImageTex(SharedPtr<CTexture> _pTexture)
{
	CanvasRenderer()->GetSharedMaterial()->SetData(E_ShaderParam::Texture_0, _pTexture.Get());
}

SharedPtr<CTexture> CImageUI::GetImageTex()
{
	SharedPtr<CTexture> pTexture = nullptr;
	CanvasRenderer()->GetSharedMaterial()->GetData(E_ShaderParam::Texture_0, pTexture.GetAddress());
	if (pTexture.Get()) {
		int a;
	}
	else {
		int b;
	}
	return m_pTexture;
}