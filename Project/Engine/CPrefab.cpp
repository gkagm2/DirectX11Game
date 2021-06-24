#include "pch.h"
#include "CPrefab.h"
#include "CGameObject.h"

CPrefab::CPrefab(CGameObject* _pProtoObj) :
	m_pProtoObj(_pProtoObj)
{
	assert(m_pProtoObj);
}

CPrefab::~CPrefab()
{
	if (nullptr != m_pProtoObj)
		delete m_pProtoObj;
}

CGameObject* CPrefab::Instantiate()
{
	assert(m_pProtoObj);
	return m_pProtoObj->Clone();
}

int CPrefab::Load(const tstring& _strFilePath)
{
	return 0;
}