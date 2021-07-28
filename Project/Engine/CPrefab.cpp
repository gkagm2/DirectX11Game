#include "pch.h"
#include "CPrefab.h"
#include "CGameObject.h"
#include "CPathManager.h"

CPrefab::CPrefab(CGameObject* _pProtoObj) :
	CResource(E_ResourceType::Prefab),
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

bool CPrefab::Save(const tstring& _strRelativePath)
{
	tstring strFilePath = CPathManager::GetInstance()->GetContentPath();
	strFilePath += _strRelativePath;

	errno_t err;
	FILE* pFile = nullptr;
	err = _tfopen_s(&pFile, strFilePath.c_str(), _T("wb"));
	assert(pFile);
	if (err)
		return false;

	m_pProtoObj->SaveToScene(pFile);
	fclose(pFile);

	return true;
}

int CPrefab::Load(const tstring& _strFilePath)
{
	errno_t err;
	FILE* pFile = nullptr;
	err = _tfopen_s(&pFile, _strFilePath.c_str(), _T("rb"));
	assert(pFile);
	if (err)
		return 0;

	m_pProtoObj = new CGameObject;
	m_pProtoObj->LoadFromScene(pFile, 0);
	fclose(pFile);

	return 1;
}