#include "pch.h"
#include "CCameraModule.h"

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