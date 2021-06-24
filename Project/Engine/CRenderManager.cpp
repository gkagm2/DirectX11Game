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
	// 1. Ÿ�� Ŭ����
	CDevice::GetInstance()->ClearTarget();

	// 2. ��ü ������
	CSceneManager::GetInstance()->Render();

	// 3. ����� ��ü
	CDevice::GetInstance()->Present();

	m_vecCam.clear();
	m_vecLight2D.clear();
}

void CRenderManager::_RenderInit()
{
	// TODO (Jang) : ���ҽ� ���ε�
}