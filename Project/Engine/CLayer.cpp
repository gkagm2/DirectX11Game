#include "pch.h"
#include "CLayer.h"
#include "CGameObject.h"

CLayer::CLayer() :
	m_iLayerIdx(0)
{
}

CLayer::~CLayer()
{
	Safe_Delete_Vector(m_vecParentObj);
}

void CLayer::Awake()
{
	for (UINT i = 0; i < m_vecParentObj.size(); ++i)
		m_vecParentObj[i]->Awake();
}

void CLayer::Start()
{
	for (UINT i = 0; i < m_vecParentObj.size(); ++i)
		m_vecParentObj[i]->Start();
}

void CLayer::PrevUpdate()
{
	for (UINT i = 0; i < m_vecParentObj.size(); ++i)
		m_vecParentObj[i]->PrevUpdate();
}

void CLayer::Update()
{
	for (UINT i = 0; i < m_vecParentObj.size(); ++i)
		m_vecParentObj[i]->Update();
}

void CLayer::LateUpdate()
{
	for (UINT i = 0; i < m_vecParentObj.size(); ++i)
		m_vecParentObj[i]->LateUpdate();
}

void CLayer::FinalUpdate()
{
	for (UINT i = 0; i < m_vecParentObj.size(); ++i)
		m_vecParentObj[i]->FinalUpdate();
}

void CLayer::Render()
{
	for (UINT i = 0; i < m_vecParentObj.size(); ++i)
		m_vecParentObj[i]->Render();
}