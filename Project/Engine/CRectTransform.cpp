#include "pch.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CRectTransform.h"
#include "CRenderManager.h"
#include "CCamera.h"

#include "CGameObject.h"

CRectTransform::CRectTransform() :
	CTransform(E_ComponentType::RectTransform)
{
}

CRectTransform::~CRectTransform()
{
}

void CRectTransform::FinalUpdate()
{
	CTransform::FinalUpdate();
}

void CRectTransform::UpdateData()
{
	CTransform::UpdateData();
}

bool CRectTransform::SaveToScene(FILE* _pFile)
{
	CTransform::SaveToScene(_pFile);
	return true;
}

bool CRectTransform::LoadFromScene(FILE* _pFile)
{
	CTransform::LoadFromScene(_pFile);
	return true;
}