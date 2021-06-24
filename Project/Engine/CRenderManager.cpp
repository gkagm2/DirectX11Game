#include "pch.h"
#include "CRenderManager.h"
#include "CDevice.h"
#include "CSceneManager.h"

CRenderManager::CRenderManager()
{

}

CRenderManager::~CRenderManager()
{

}
void CRenderManager::Init()
{
}

void CRenderManager::Render()
{
	// Render
	// 1. 타겟 클리어
	CDevice::GetInstance()->ClearTarget();

	// 2. 물체 렌더링
	CSceneManager::GetInstance()->Render();

	// 3. 백버퍼 교체
	CDevice::GetInstance()->Present();

	m_vecCam.clear();
	m_vecLight2D.clear();
}

void CRenderManager::_RenderInit()
{
	// TODO (Jang) : 리소스 바인딩
}