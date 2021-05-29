#include "pch.h"
#include "CGameObject.h"
#include "CComponent.h"

CGameObject::CGameObject() :
	m_arrComponent{}
{
}

CGameObject::~CGameObject()
{
	Safe_Delete_Array(m_arrComponent);
}

void CGameObject::Awake()
{
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i])
			m_arrComponent[i]->Awake();
	}
}

void CGameObject::Start()
{
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i])
			m_arrComponent[i]->Awake();
	}
}

void CGameObject::PrevUpdate()
{
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i])
			m_arrComponent[i]->PrevUpdate();
	}
}

void CGameObject::Update()
{
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i])
			m_arrComponent[i]->Update();
	}
}

void CGameObject::LateUpdate()
{
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i])
			m_arrComponent[i]->LateUpdate();
	}
}

void CGameObject::Render()
{
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr != m_arrComponent[i])
			m_arrComponent[i]->Render();
	}
}