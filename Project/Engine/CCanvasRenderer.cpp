#include "pch.h"
#include "CCanvasRenderer.h"
#include "CResourceManager.h"
#include "CTextUI.h"
#include "CButtonUI.h"
#include "CImageUI.h"
#include "CRectTransform.h"
#include "CAnimator2D.h"

CCanvasRenderer::CCanvasRenderer() :
	CRenderer(E_ComponentType::CanvasRenderer)
{
}

CCanvasRenderer::~CCanvasRenderer()
{
}

void CCanvasRenderer::Render()
{
	// 위치정보 세팅
	if (RectTransform())
		RectTransform()->UpdateData();
	else {
		assert(nullptr);
		Transform()->UpdateData();
	}

	if (Animator2D())
		Animator2D()->UpdateData();
	else
		CAnimator2D::Clear();

	CRenderer::Render();
	if (TextUI() && TextUI()->IsActive())
		TextUI()->Render();
	if (ButtonUI() && ButtonUI()->IsActive())
		ButtonUI()->Render();
	if (ImageUI() && ImageUI()->IsActive())
		ImageUI()->Render();
}