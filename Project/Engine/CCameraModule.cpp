#include "pch.h"
#include "CCameraModule.h"
#include "CFrustum.h"

CCameraModule::CCameraModule()
{
}

CCameraModule::~CCameraModule()
{
}

void CCameraModule::FinalUpdate()
{
	CalculateViewMatrix();
	CalculateProjectionMatrix();
}