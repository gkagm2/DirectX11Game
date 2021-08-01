#include "pch.h"
#include "CCameraEx.h"
#include <Engine\CRenderManager.h>

CCameraEx::CCameraEx()
{

}

CCameraEx::~CCameraEx()
{
}

void CCameraEx::FinalUpdate()
{
	CalculateViewMatrix();
	CalculateProjectionMatrix();

	g_transform.matView = GetViewMatrix();
	g_transform.matProjection = GetProjectionMatrix();
	CRenderManager::GetInstance()->RegisterToolCamera(this);
}