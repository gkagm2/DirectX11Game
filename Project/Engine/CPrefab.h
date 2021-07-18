#pragma once
#include "CResource.h"
class CGameObject;
class CPrefab : public CResource
{
private:
	CGameObject* m_pProtoObj;

public:
	CGameObject* Instantiate();
	virtual bool Save(const tstring& _strRelativePath);
	virtual int Load(const tstring& _strFilePath);

private:
	CLONE_DISABLE(CPrefab);

private:
	CPrefab() :
		m_pProtoObj(nullptr)
	{}
public:
	CPrefab(CGameObject* _pProtoObj);
	virtual ~CPrefab() override;

	friend class CResourceManager;
};