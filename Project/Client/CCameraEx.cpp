#include "pch.h"
#include "CCameraEx.h"
#include <Engine\CCamera.h>
#include <Engine\CRenderManager.h>
#include <Engine\CFrustum.h>
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
	CalculateRay();
	GetFrustum()->FinalUpdate();
	CRenderManager::GetInstance()->RegisterToolCamera(this);
}