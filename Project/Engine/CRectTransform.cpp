#include "pch.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CRectTransform.h"

#include "CGameObject.h"

CRectTransform::CRectTransform() :
	CTransform(E_ComponentType::RectTransform),
	m_fWidth(100.f),
	m_fHeight(100.f)
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
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::RectTransform);
	CTransform::UpdateData();

	g_rectTransform.matWorld = GetWOrldMatrix_NoParentScale();
	g_rectTransform.matView = g_transform.matView;
	g_rectTransform.matProjection = g_transform.matProjection;


	g_rectTransform.fWidth = m_fWidth;
	g_rectTransform.fHeight = m_fHeight;

	pCB->SetData(&g_rectTransform);
	pCB->UpdateData(E_ShaderStage::NonePixel);
}

bool CRectTransform::SaveToScene(FILE* _pFile)
{
	CTransform::SaveToScene(_pFile);

	FWrite(m_fWidth, _pFile);
	FWrite(m_fHeight, _pFile);
	return true;
}

bool CRectTransform::LoadFromScene(FILE* _pFile)
{
	CTransform::LoadFromScene(_pFile);
	
	FRead(m_fWidth, _pFile);
	FRead(m_fHeight, _pFile);
	return true;
}