#include "pch.h"
#include "CLayer.h"
#include "CGameObject.h"

CLayer::CLayer() :
	m_eLayer(E_Layer::Default)
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
	m_vecObj.clear();
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
	auto iter = m_vecParentObj.begin();
	while (iter != m_vecParentObj.end()) {
		// ��ġ���� ������Ʈ�� ��ü ���� ��� ������ ���� �� ������ ���� ���� ������Ʈ�� FinalUpdate���� ȣ��.
		(*iter)->FinalUpdate();

		if ((*iter)->IsDead())
			iter = m_vecParentObj.erase(iter);
		else
			++iter;
	}
}

void CLayer::Render()
{
	for (UINT i = 0; i < m_vecObj.size(); ++i)
		m_vecObj[i]->Render();
}

void CLayer::AddGameObject(CGameObject* _pObj)
{
	m_vecParentObj.push_back(_pObj);
	_pObj->m_eLayer = m_eLayer;
}
